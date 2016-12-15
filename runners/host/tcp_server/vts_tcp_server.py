#!/usr/bin/env python
#
# Copyright 2016 - The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#     http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.

import SocketServer
import logging
import threading
from socket import error as socket_error

from vts.runners.host.errors import TcpServerCreationError
from vts.runners.host.errors import TcpServerShutdownError
from vts.runners.host.proto import AndroidSystemControlMessage_pb2 as SysMsg_pb2

_functions = dict()  # Dictionary to hold function pointers

class TCPRequestHandler(SocketServer.StreamRequestHandler):
    """The request handler class for our server."""

    def handle(self):
        """Receives requests from clients.

        This function receives the request in the form of Serialized String
        and converts it to the form AndroidSystemCallbackRequestMessage.
        Then finally returns the serialized string version of object
        AndroidSystemCallbackResponseMessage to the client.

        """
        # self.request is the TCP socket connected to the client
        header = self.rfile.readline()
        try:
            len = int(header.strip("\n"))
        except ValueError as e:
            logging.exception(e)
            return

        received_data = self.rfile.read(len)

        request_message = SysMsg_pb2.AndroidSystemCallbackRequestMessage()
        request_message.ParseFromString(received_data)
        response_message = SysMsg_pb2.AndroidSystemCallbackResponseMessage()

        logging.info('client address: %s', self.client_address[0])
        logging.info('received_data ID: %s', request_message.id)
        logging.info('Current Thread: %s', threading.current_thread())

        if request_message.id in _functions:
            _functions[request_message.id]()  # call the function pointer
            response_message.response_code = SysMsg_pb2.SUCCESS
        else:
            response_message.response_code = SysMsg_pb2.FAIL
            logging.info("no registered callback handler found")

        # send the response back to client
        message = response_message.SerializeToString()
        self.request.sendall(message)


class VtsTcpServer(object):
    """This class creates TCPServer in separate thread.

    Attributes:
        _server:   an instance of SocketServer.TCPServer.
        _port_used: this variable maintains the port number used in creating
                  the server connection.
        _IP_address: variable to hold the IP Address of the host.
        _HOST: IP Address to which initial connection is made.
    """
    _server = None
    _port_used = 0  # Port 0 means to select an arbitrary unused port
    _IP_address = ""  # Used to store the IP address for the server
    _HOST = "localhost"  # IP address to which initial connection is made

    def RegisterCallback(self, func_id, callback_func):
        """To register the function pointer.

        This method registers the callback for the _functions by storing the
        function pointer in set.

        Args:
            func_id: Refers to the func_id of function pointers that is maintained in
                     the dictionary.
            callback_func:  Refers to the callback_func that we need to
                            register.

        Returns:
            False if func_id is already present in the dictionary, func_id is
            None or callback_func is None; else returns True after inserting
            the key-func_id and value-callback_func.
        """
        if func_id is None or func_id in _functions or callback_func is None:
            return False
        else:
            _functions[func_id] = callback_func  # update the dictionary
            return True

    def UnregisterCallback(self, func_id):
        """Removes the function pointer from the dict corresponding to the key.

        Args:
            func_id: The function id against which the function pointer
                is stored.

        Returns:
            Returns false if func_id is not present in the dictionary -
                _functions, else return True after removing it from dict.
        """
        if func_id is None or func_id not in _functions:
            return False
        else:
            _functions.pop(func_id, None)  #  Remove the key from dictionary
            return True

    def Start(self, port=0):
        """This function starts the TCP server.

        Args:
            port: The port at which connection will be made. Default value
                  is zero, in which case a free port will be chosen
                  automatically.

        Returns:
            IP Address, port number

        Raises:
            TcpServerCreationError: Error occurred while starting server.
        """
        # Start the sever
        try:
            self._server = SocketServer.TCPServer((self._HOST, port),
                                                  TCPRequestHandler)
            self._IP_address, self._port_used = self._server.server_address

            # Start a thread with the server -- that thread will then start one
            # more thread for each request
            server_thread = threading.Thread(target=self._server.serve_forever)

            # Exit the server thread when the main thread terminates
            server_thread.daemon = True
            server_thread.start()
            logging.info('TcpServer %s started (%s:%s)',
                         server_thread.name, self._IP_address, self._port_used)
            return self._IP_address, self._port_used
        except (RuntimeError, IOError, socket_error) as e:
            logging.exception(e)
            raise TcpServerCreationError('TcpServerCreationError occurred.')

    def Stop(self):
        """This function calls stop server to stop the server instance."""
        self._server.shutdown
        self._server.server_close()

    def GetIPAddress(self):
        """Returns the IP Address used in creating the server."""
        return self._IP_address

    def GetPortUsed(self):
        """Returns the port number used in creating the server."""
        return self._port_used
