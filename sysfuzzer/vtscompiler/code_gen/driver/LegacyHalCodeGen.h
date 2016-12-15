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

#ifndef __VTS_SYSFUZZER_COMPILER_LEGACYHALCODEGEN_H__
#define __VTS_SYSFUZZER_COMPILER_LEGACYHALCODEGEN_H__

#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "test/vts/proto/ComponentSpecificationMessage.pb.h"

#include "code_gen/driver/HalCodeGen.h"

using namespace std;

namespace android {
namespace vts {

class LegacyHalCodeGen : public HalCodeGen {
 public:
  LegacyHalCodeGen(const char* input_vts_file_path, const string& vts_name)
      : HalCodeGen(input_vts_file_path, vts_name) {}

 protected:
  void GenerateCppBodyFuzzFunction(std::stringstream& cpp_ss,
                                   const ComponentSpecificationMessage& message,
                                   const string& fuzzer_extended_class_name);

  void GenerateCppBodyGetAttributeFunction(
      std::stringstream& cpp_ss, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name);

  void GenerateHeaderGlobalFunctionDeclarations(
      std::stringstream& h_ss, const string& function_prototype);

  // instance variable name (e.g., submodule_);
  static const char* const kInstanceVariableName;
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_SYSFUZZER_COMPILER_LEGACYHALCODEGEN_H__
