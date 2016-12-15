LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := KernelLtpTest
LOCAL_MODULE_CLASS := FAKE
LOCAL_IS_HOST_MODULE := true
LOCAL_COMPATIBILITY_SUITE := vts

include $(BUILD_SYSTEM)/base_rules.mk

$(LOCAL_BUILT_MODULE):
	@echo "VTS host-driven test target: $(LOCAL_MODULE)"
	$(hide) touch $@

VTS_CONFIG_SRC_DIR := testcases/kernel/ltp
include test/vts/tools/build/Android.host_config.mk

