#ifndef __VTS_DRIVER__android_hardware_tests_memory_V1_0_IMemoryTest__
#define __VTS_DRIVER__android_hardware_tests_memory_V1_0_IMemoryTest__

#undef LOG_TAG
#define LOG_TAG "FuzzerExtended_android_hardware_tests_memory_V1_0_IMemoryTest"

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <utils/Log.h>

#include <fuzz_tester/FuzzerBase.h>
#include <fuzz_tester/FuzzerCallbackBase.h>

#include <android/hardware/tests/memory/1.0/IMemoryTest.h>
#include <hidl/HidlSupport.h>
#include <android/hidl/base/1.0/types.h>


using namespace android::hardware::tests::memory::V1_0;
namespace android {
namespace vts {
class FuzzerExtended_android_hardware_tests_memory_V1_0_IMemoryTest : public FuzzerBase {
 public:
    FuzzerExtended_android_hardware_tests_memory_V1_0_IMemoryTest() : FuzzerBase(HAL_HIDL), hw_binder_proxy_() {}

    explicit FuzzerExtended_android_hardware_tests_memory_V1_0_IMemoryTest(::android::hardware::tests::memory::V1_0::IMemoryTest* hw_binder_proxy) : FuzzerBase(HAL_HIDL), hw_binder_proxy_(hw_binder_proxy) {}
    uint64_t GetHidlInterfaceProxy() const {
        return reinterpret_cast<uintptr_t>(hw_binder_proxy_.get());
    }
 protected:
    bool Fuzz(FunctionSpecificationMessage* func_msg, void** result, const string& callback_socket_name);
    bool CallFunction(const FunctionSpecificationMessage& func_msg, const string& callback_socket_name, FunctionSpecificationMessage* result_msg);
    bool VerifyResults(const FunctionSpecificationMessage& expected_result, const FunctionSpecificationMessage& actual_result);
    bool GetAttribute(FunctionSpecificationMessage* func_msg, void** result);
    bool GetService(bool get_stub, const char* service_name);

 private:
    sp<::android::hardware::tests::memory::V1_0::IMemoryTest> hw_binder_proxy_;
};


extern "C" {
extern android::vts::FuzzerBase* vts_func_4_android_hardware_tests_memory_V1_0_IMemoryTest_();
extern android::vts::FuzzerBase* vts_func_4_android_hardware_tests_memory_V1_0_IMemoryTest_with_arg(uint64_t hw_binder_proxy);
}
}  // namespace vts
}  // namespace android
#endif
