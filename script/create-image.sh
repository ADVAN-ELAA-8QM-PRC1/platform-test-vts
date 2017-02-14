#!/bin/bash
#
# Copyright 2016 Google Inc. All Rights Reserved.
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

function vts_multidevice_create_image {
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/nfc/ -rf
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/sensors/ -rf
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/vehicle/ -rf
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/vibrator/ -rf
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/vr/ -rf
  rm ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/tv_cec/ -rf
  . ${ANDROID_BUILD_TOP}/build/make/envsetup.sh
  cd ${ANDROID_BUILD_TOP}; lunch $1 $2
  cd ${ANDROID_BUILD_TOP}/test/vts; mma -j 32 && cd ${ANDROID_BUILD_TOP}; make vts adb -j 32
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/nfc ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/sensors ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/vehicle ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/vibrator ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/vr ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
  cp ${ANDROID_BUILD_TOP}/test/vts-testcase/hal/tv ${ANDROID_BUILD_TOP}/test/vts/testcases/hal/ -rf
}

vts_multidevice_create_image $1 $2

