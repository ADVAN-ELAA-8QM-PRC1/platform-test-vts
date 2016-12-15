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

LOCAL_PATH := $(call my-dir)
include $(LOCAL_PATH)/list/vts_bin_package_list.mk
include $(LOCAL_PATH)/list/vts_lib_package_list.mk
include $(LOCAL_PATH)/list/vts_test_bin_package_list.mk
include $(LOCAL_PATH)/list/vts_test_lib_hal_package_list.mk
include $(LOCAL_PATH)/list/vts_test_lib_hidl_package_list.mk

my_modules := \
    $(vts_bin_packages) \
    $(vts_lib_packages) \
    $(vts_test_bin_packages) \
    $(vts_test_lib_hal_packages) \
    $(vts_test_lib_hidl_packages) \

my_copy_pairs :=
  $(foreach m,$(my_modules),\
    $(eval _built_files := $(strip $(ALL_MODULES.$(m).BUILT_INSTALLED)\
    $(ALL_MODULES.$(m)$(TARGET_2ND_ARCH_MODULE_SUFFIX).BUILT_INSTALLED)))\
    $(foreach i, $(_built_files),\
      $(eval bui_ins := $(subst :,$(space),$(i)))\
      $(eval ins := $(word 2,$(bui_ins)))\
      $(if $(filter $(TARGET_OUT_ROOT)/%,$(ins)),\
        $(eval bui := $(word 1,$(bui_ins)))\
        $(eval my_built_modules += $(bui))\
        $(eval my_copy_dest := $(patsubst data/%,DATA/%,\
                                 $(patsubst system/%,DATA/%,\
                                   $(patsubst $(PRODUCT_OUT)/%,%,$(ins)))))\
        $(eval my_copy_pairs += $(bui):$(VTS_TESTCASES_OUT)/$(my_copy_dest)))\
    ))

.PHONY: vts
vts: $(call copy-many-files,$(my_copy_pairs))
	@echo "vts_native_packages $(my_modules)"
	@echo "vts_native_packages copy_pairs $(my_copy_pairs)"
