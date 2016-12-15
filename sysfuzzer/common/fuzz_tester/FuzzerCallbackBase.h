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

#ifndef __VTS_SYSFUZZER_COMMON_FUZZER_CALLBACK_BASE_H__
#define __VTS_SYSFUZZER_COMMON_FUZZER_CALLBACK_BASE_H__

#include "component_loader/DllLoader.h"

#include <map>
#include <string>

#include "test/vts/runners/host/proto/InterfaceSpecificationMessage.pb.h"


using namespace std;

namespace android {
namespace vts {

class FuzzerCallbackBase {
 public:
  FuzzerCallbackBase();
  virtual ~FuzzerCallbackBase();

  static bool Register(const VariableSpecificationMessage& message);

 protected:
  static const char* GetCallbackID(const string& name);

  static void RpcCallToAgent(const char* id, int agent_port);
};

}  // namespace vts
}  // namespace android

#endif  // __VTS_SYSFUZZER_COMMON_FUZZER_CALLBACK_BASE_H__
