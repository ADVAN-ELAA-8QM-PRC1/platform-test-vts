#
# Copyright (C) 2016 The Android Open Source Project
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#

LOCAL_PATH:= $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := libvts_measurement
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
  vts_measurement.cpp \

LOCAL_C_INCLUDES := \
  bionic \
  libcore \
  external/protobuf/src \
  test/vts/sysfuzzer/common/proto \

LOCAL_SHARED_LIBRARIES := \
  libcutils \
  libvts_common \

LOCAL_PROTOC_FLAGS := \
  --proto_path=$(LOCAL_PATH)/../common/proto \

LOCAL_PROTOC_OPTIMIZE_TYPE := full

include $(BUILD_SHARED_LIBRARY)
include $(LOCAL_PATH)/../../tools/build/Android.packaging_sharedlib.mk
