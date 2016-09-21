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

#include <limits.h>
#include <stdlib.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

#include <cstdint>
#include <fstream>
#include <iostream>
#include <sstream>

#include "specification_parser/InterfaceSpecificationParser.h"
#include "utils/InterfaceSpecUtil.h"

#include "test/vts/proto/ComponentSpecificationMessage.pb.h"

#include "VtsCompilerUtils.h"
#include "code_gen/CodeGenBase.h"
#include "code_gen/driver/HalCodeGen.h"
#include "code_gen/driver/HalSubmoduleCodeGen.h"
#include "code_gen/driver/HalHidlCodeGen.h"
#include "code_gen/driver/LegacyHalCodeGen.h"
#include "code_gen/driver/LibSharedCodeGen.h"

using namespace std;

namespace android {
namespace vts {

enum VtsCompileMode {
  kDriver = 0,
  kProfiler
};

// Translates the vts proto file to C/C++ code and header files.
void Translate(VtsCompileMode mode,
               const char* input_vts_file_path,
               const char* output_header_dir_path,
               const char* output_cpp_file_path) {
  string output_cpp_file_path_str = string(output_cpp_file_path);

  size_t found;
  found = output_cpp_file_path_str.find_last_of("/");
  string vts_name = output_cpp_file_path_str
      .substr(found + 1, output_cpp_file_path_str.length() - found - 5);

  cout << "vts_name: " << vts_name << endl;

  ComponentSpecificationMessage message;
  if (InterfaceSpecificationParser::parse(input_vts_file_path, &message)) {
    cout << message.component_class();
  } else {
    cerr << "can't parse " << input_vts_file_path << endl;
  }

  unique_ptr<CodeGenBase> code_generator;
  if (mode == kProfiler) {
    switch (message.component_class()) {
      default:
        cerr << "not yet supported component_class " << message.component_class();
        exit(-1);
    }
  } else if (mode == kDriver) {
    switch (message.component_class()) {
      case HAL_CONVENTIONAL:
        code_generator.reset(new HalCodeGen(input_vts_file_path, vts_name));
        break;
      case HAL_CONVENTIONAL_SUBMODULE:
        code_generator.reset(
            new HalSubmoduleCodeGen(input_vts_file_path, vts_name));
        break;
      case HAL_LEGACY:
        code_generator.reset(new LegacyHalCodeGen(input_vts_file_path, vts_name));
        break;
      case LIB_SHARED:
        code_generator.reset(new LibSharedCodeGen(input_vts_file_path, vts_name));
        break;
      case HAL_HIDL:
        code_generator.reset(new HalHidlCodeGen(input_vts_file_path, vts_name));
        break;
      default:
        cerr << "not yet supported component_class " << message.component_class();
        exit(-1);
    }
  } else {
    cerr << "unknown compile mode " << mode << endl;
    exit(-1);
  }

  std::stringstream cpp_ss;
  std::stringstream h_ss;
  code_generator->GenerateAll(cpp_ss, h_ss, message);

  // Creates a C/C++ file and its header file.
  cout << "write to " << output_cpp_file_path << endl;
  ofstream cpp_out_file(output_cpp_file_path);
  if (!cpp_out_file.is_open()) {
    cerr << "Unable to open file" << endl;
  } else {
    cpp_out_file << cpp_ss.str();
    cpp_out_file.close();
  }

  string output_header_file_path =
      string(output_header_dir_path) + "/" + string(input_vts_file_path);

  output_header_file_path = output_header_file_path + ".h";

  cout << "header: " << output_header_file_path << endl;
  vts_fs_mkdirs(&output_header_file_path[0], 0777);

  ofstream header_out_file(output_header_file_path.c_str());
  if (!header_out_file.is_open()) {
    cerr << "Unable to open file" << endl;
  } else {
    header_out_file << h_ss.str();
    header_out_file.close();
  }
}

}  // namespace vts
}  // namespace android

int main(int argc, char* argv[]) {
#ifdef VTS_DEBUG
  cout << "Android VTS Compiler (AVTSC)" << endl;
#endif
  int opt_count = 0;
  android::vts::VtsCompileMode mode = android::vts::kDriver;
  for (int i = 0; i < argc; i++) {
#ifdef VTS_DEBUG
    cout << "- args[" << i << "] " << argv[i] << endl;
#endif
    if (argv[i] && strlen(argv[i]) > 1 && argv[i][0] == '-') {
      opt_count++;
      if (argv[i][1] == 'm') {
        if (!strcmp(&argv[i][2], "PROFILER")) {
          mode = android::vts::kProfiler;
#ifdef VTS_DEBUG
          cout << "- mode: PROFILER" << endl;
#endif
        }
      }
    }
  }
  if (argc < 5) {
    cerr << "argc " << argc << " < 5" << endl;
    return -1;
  }
  android::vts::Translate(
      mode, argv[opt_count + 1], argv[opt_count + 2], argv[opt_count + 3]);
  return 0;
}
