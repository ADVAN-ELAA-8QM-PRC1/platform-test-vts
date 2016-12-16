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

#ifndef __VTS_SYSFUZZER_COMPILER_DRIVER_CODEGENBASE_H__
#define __VTS_SYSFUZZER_COMPILER_DRIVER_CODEGENBASE_H__

#include <hidl-util/Formatter.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "code_gen/CodeGenBase.h"

#include "test/vts/proto/ComponentSpecificationMessage.pb.h"

using namespace std;

namespace android {
namespace vts {

class DriverCodeGenBase : public CodeGenBase {
 public:
  explicit DriverCodeGenBase(
      const char* input_vts_file_path, const string& vts_name) :
      CodeGenBase(input_vts_file_path, vts_name) {}

  // Generate both a C/C++ file and its header file.
  virtual void GenerateAll(Formatter& header_out, Formatter& source_out,
                           const ComponentSpecificationMessage& message);

 protected:
  // Generates source file.
  virtual void GenerateSourceFile(
      Formatter& out, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name);

  // Generates header file.
  virtual void GenerateHeaderFile(
      Formatter& out, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name);

  // Generates code for Fuzz(...) function body.
  virtual void GenerateCppBodyFuzzFunction(
      Formatter& out, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name) = 0;

  // Generates code for GetAttribute(...) function body.
  virtual void GenerateCppBodyGetAttributeFunction(
      Formatter& out, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name) = 0;

  // Generates header code to declare the C/C++ global functions.
  virtual void GenerateHeaderGlobalFunctionDeclarations(
      Formatter& out, const string& function_prototype) = 0;

  // Generates C/C++ code for callback functions.
  virtual void GenerateCppBodyCallbackFunction(
      Formatter& out, const ComponentSpecificationMessage& message,
      const string& fuzzer_extended_class_name);

  // Generates code for the bodies of the C/C++ global functions.
  virtual void GenerateCppBodyGlobalFunctions(
      Formatter& out, const string& function_prototype,
      const string& fuzzer_extended_class_name) = 0;

  // Generates the namespace name of a HIDL component, crashes otherwise.
  void GenerateNamespaceName(
      Formatter& out, const ComponentSpecificationMessage& message);

  // Generates code that opens the default namespaces.
  void GenerateOpenNameSpaces(
      Formatter& out, const ComponentSpecificationMessage& message);

  // Generates code that closes the default namespaces.
  void GenerateCloseNameSpaces(Formatter& out);

  // Generates code that starts the measurement.
  void GenerateCodeToStartMeasurement(Formatter& out);

  // Generates code that stops the measurement.
  void GenerateCodeToStopMeasurement(Formatter& out);

  // Generate header code for a specific class.
  void GenerateClassHeader(const string& fuzzer_extended_class_name,
                           Formatter& out,
                           const ComponentSpecificationMessage& message);

  string GetComponentName(const ComponentSpecificationMessage& message);

  void GenerateFuzzFunctionForSubStruct(
      Formatter& out, const StructSpecificationMessage& message,
      const string& parent_path);
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_SYSFUZZER_COMPILER_CODEGENBASE_H__
