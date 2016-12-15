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

LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := vtssysfuzzer
LOCAL_MODULE_STEM_64 := fuzzer64
LOCAL_MODULE_STEM_32 := fuzzer32
LOCAL_MODULE_TAGS := optional

LOCAL_SRC_FILES := \
  VtsFuzzerMain.cpp \
  BinderServer.cpp \
  SocketServer.cpp \

LOCAL_C_INCLUDES := \
  bionic \
  libcore \
  device/google/gce/include \
  external/protobuf/src \
  frameworks/native/include \
  system/core/include \
  system/extras \
  test/vts/drivers/libdrivercomm \
  test/vts/sysfuzzer/framework \
  test/vts/sysfuzzer/common \

LOCAL_SHARED_LIBRARIES := \
  libutils \
  libcutils \
  liblog \
  libbinder \
  libdl \
  libandroid_runtime \
  libvts_common \
  libvts_drivercomm \
  libvts_multidevice_proto \
  libprotobuf-cpp-full \

LOCAL_STATIC_LIBRARIES := \
  libelf \

LOCAL_MULTILIB := both

LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_EXECUTABLE)

ifneq ($(TARGET_ARCH),x86)

VTS_TESTCASES_OUT := $(HOST_OUT)/vts/android-vts/testcases
vts_framework_file64 := $(VTS_TESTCASES_OUT)/$(LOCAL_MODULE_STEM_64)

$(vts_framework_file64): $(call intermediates-dir-for,EXECUTABLES,$(LOCAL_MODULE))/$(LOCAL_MODULE_STEM_64) | $(ACP)
	$(hide) mkdir -p $(VTS_TESTCASES_OUT)
	$(hide) $(ACP) -fp $< $@

vts: $(vts_framework_file64)

endif

