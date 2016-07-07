/*
 * Copyright 2016 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include "ShellDriver.h"

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <sys/types.h>
#include <unistd.h>

#include <sstream>
#include <iostream>

#include "test/vts/proto/VtsDriverControlMessage.pb.h"
#include <VtsDriverCommUtil.h>

using namespace std;

namespace android {
namespace vts {

int VtsShellDriver::Close() {
  cout << __func__ << endl;
  int result = 0;

  if (!this->socket_address_.empty()) {
    result = unlink(this->socket_address_.c_str());
    if (result != 0) {
      cerr <<  __func__ << ":" << __LINE__
          << " ERROR closing socket (errno = "
          << errno << ")"<< endl;
    }
    this->socket_address_.clear();
  }

  return result;
}


int VtsShellDriver::ExecShellCommand(const string& command,
                     VtsDriverControlResponseMessage* responseMessage) {
  // TODO(yuexima): handle no output case.
  FILE* output_fp;

  // execute the command.
  output_fp = popen(command.c_str(), "r");
  if (output_fp == NULL) {
    cerr << "Failed to run command: " << command << endl;
    int no = errno;
    return no;
  }

  cout << "[Handler] Running command: " << command << endl << endl;

  char buff[4096];
  stringstream ss;

  int bytes_read;
  while (!feof(output_fp)) {
    bytes_read = fread(buff, 1, sizeof(buff) - 1, output_fp);
    // TODO(yuexima) catch stderr
    if (ferror(output_fp)) {
      cerr <<  __func__ << ":" << __LINE__
          << "ERROR reading shell output" << endl;
      return -1;
    }

    cout << "[Handler] bytes read from output: " << bytes_read << endl;
    buff[bytes_read] = '\0';
    ss << buff;
  }

  cout << "[Handler] Returning output: " << ss.str() << endl << endl;
  responseMessage->add_stdout(ss.str());

  for (auto const& out : responseMessage->stdout()) {
    cout << "[Handler] Loop output: " << out << endl << endl;
  }

  return 0;
}


int VtsShellDriver::HandleShellCommandConnection(int connection_fd) {
  VtsDriverCommUtil driverUtil(connection_fd);
  // TODO(yuexima): handle multiple commands in a while loop
  VtsDriverControlCommandMessage cmd_msg;
  bool success;
  int numberOfFailure = 0;

  if (!driverUtil.VtsSocketRecvMessage(
       static_cast<google::protobuf::Message*>(&cmd_msg))) {
    return -1;
  }

  cout << "[Shell driver] received " << cmd_msg.shell_command_size()
      << " command(s). Processing... " << endl;

  // execute command and write back output
  VtsDriverControlResponseMessage responseMessage;

  for (const auto& command : cmd_msg.shell_command()) {
    if (this->ExecShellCommand(command, &responseMessage) != 0) {
      cerr << "[Shell driver] error during executing command ["
          << command << "]" << endl;
      --numberOfFailure;
    }
  }

  if (!driverUtil.VtsSocketSendMessage(responseMessage)) {
    fprintf(stderr, "Driver: write output to socket error.\n");
    --numberOfFailure;
  }

  cout << "[Shell driver] finished processing commands." << endl;

  if (driverUtil.Close() != 0) {
    cerr << "[Driver] failed to close connection. errno: " << errno << endl;
    --numberOfFailure;
  }

  return numberOfFailure;
}


int VtsShellDriver::StartListen() {
  if (this->socket_address_.empty()) {
    cerr << "[Driver] NULL socket address." << endl;
    return -1;
  }

  cout << "[Driver] start listening on " << this->socket_address_ << endl;

  struct sockaddr_un address;
  int socket_fd, connection_fd;
  socklen_t address_length;
  pid_t child;

  socket_fd = socket(PF_UNIX, SOCK_STREAM, 0);
  if (socket_fd < 0) {
    cerr << "Driver: socket() failed: " << strerror(errno) << endl;
    return socket_fd;
  }

  unlink(this->socket_address_.c_str());
  memset(&address, 0, sizeof(struct sockaddr_un));
  address.sun_family = AF_UNIX;
  strncpy(address.sun_path,
          this->socket_address_.c_str(),
          sizeof(address.sun_path) - 1);

  if (::bind(socket_fd,
             (struct sockaddr *) &address,
             sizeof(struct sockaddr_un)) != 0) {
    cerr << "Driver: bind() failed: " << strerror(errno) << endl;
    return 1;
  }

  if (listen(socket_fd, 5) != 0) {
    cerr << "Driver: listen() failed: " << strerror(errno) << endl;

    return errno;
  }

  while (1) {
    address_length = sizeof(address);

    // TODO(yuexima) exit message to break loop
    connection_fd = accept(socket_fd,
                           (struct sockaddr *) &address, &address_length);
    if (connection_fd == -1) {
      cerr << "Driver: accept error: " << strerror(errno) << endl;
      break;
    }

    child = fork();
    if (child == 0) {
      close(socket_fd);
      // now inside newly created connection handling process
      if (this->HandleShellCommandConnection(connection_fd) != 0) {
        cerr << "[Driver] failed to handle connection." << endl;
        close(connection_fd);
        exit(1);
      }
      close(connection_fd);
      exit(0);
    } else if (child > 0) {
      close(connection_fd);
    } else {
      cerr << "[Driver] create child process failed. Exiting..." << endl;
      return(errno);
    }
  }

  close(socket_fd);

  return 0;
}

}  // namespace vts
}  // namespace android
