#include "android/hardware/tests/msgq/1.0/TestMsgQ.vts.h"
#include "vts_measurement.h"
#include <iostream>
#include <hidl/HidlSupport.h>
#include <android/hardware/tests/msgq/1.0/ITestMsgQ.h>
#include <android/hidl/base/1.0/types.h>
#include <android/hidl/allocator/1.0/IAllocator.h>
#include <fmq/MessageQueue.h>
#include <sys/stat.h>
#include <unistd.h>


using namespace android::hardware::tests::msgq::V1_0;
namespace android {
namespace vts {
::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits EnumValue__android__hardware__tests__msgq__V1_0__ITestMsgQ__EventFlagBits(const ScalarDataValueMessage& arg) {
    return (::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits) arg.uint32_t();
}
::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits Random__android__hardware__tests__msgq__V1_0__ITestMsgQ__EventFlagBits() {
    uint32_t choice = (uint32_t) rand() / 2;
    if (choice == (uint32_t) 1) return ::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY;
    if (choice == (uint32_t) 2) return ::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits::FMQ_NOT_FULL;
    return ::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits::FMQ_NOT_EMPTY;
}
bool Verify__android__hardware__tests__msgq__V1_0__ITestMsgQ__EventFlagBits(const VariableSpecificationMessage& expected_result, const VariableSpecificationMessage& actual_result){
    if (actual_result.scalar_value().uint32_t() != expected_result.scalar_value().uint32_t()) { return false; }
    return true;
}

void SetResult__android__hardware__tests__msgq__V1_0__ITestMsgQ__EventFlagBits(VariableSpecificationMessage* result_msg, ::android::hardware::tests::msgq::V1_0::ITestMsgQ::EventFlagBits result_value){
    result_msg->set_type(TYPE_ENUM);
    result_msg->set_scalar_type("uint32_t");
    result_msg->mutable_scalar_value()->set_uint32_t(static_cast<uint32_t>(result_value));
}

bool FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ::GetService(bool get_stub, const char* service_name) {
    static bool initialized = false;
    if (!initialized) {
        cout << "[agent:hal] HIDL getService" << endl;
        if (service_name) {
          cout << "  - service name: " << service_name << endl;
        }
        hw_binder_proxy_ = ::android::hardware::tests::msgq::V1_0::ITestMsgQ::getService(service_name, get_stub);
        if (hw_binder_proxy_ == nullptr) {
            cerr << "getService() returned a null pointer." << endl;
            return false;
        }
        cout << "[agent:hal] hw_binder_proxy_ = " << hw_binder_proxy_.get() << endl;
        initialized = true;
    }
    return true;
}

bool FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ::Fuzz(
    FunctionSpecificationMessage* /*func_msg*/,
    void** /*result*/, const string& /*callback_socket_name*/) {
    return true;
}
bool FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ::GetAttribute(
    FunctionSpecificationMessage* /*func_msg*/,
    void** /*result*/) {
    cerr << "attribute not found" << endl;
    return false;
}
bool FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ::CallFunction(const FunctionSpecificationMessage& func_msg, const string& callback_socket_name, FunctionSpecificationMessage* result_msg) {
    const char* func_name = func_msg.name().c_str();
    cout << "Function: " << __func__ << " " << func_name << endl;
    cout << "Callback socket name: " << callback_socket_name << endl;
    if (!strcmp(func_name, "configureFmqSyncReadWrite")) {
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        std::unique_ptr<::android::hardware::MQDescriptorSync<uint16_t>> result1;
        hw_binder_proxy_->configureFmqSyncReadWrite([&](bool arg0,const ::android::hardware::MQDescriptorSync<uint16_t>& arg1){
            cout << "callback configureFmqSyncReadWrite called" << endl;
            result0 = arg0;
            result1.reset(new (std::nothrow) ::android::hardware::MQDescriptorSync<uint16_t>(arg1));
        });
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("configureFmqSyncReadWrite");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        VariableSpecificationMessage* result_val_1 = result_msg->add_return_type_hidl();
        result_val_1->set_type(TYPE_FMQ_SYNC);
        /* ERROR: TYPE_FMQ_SYNC is not supported yet. */
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "getFmqUnsyncWrite")) {
        bool arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().bool_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        std::unique_ptr<::android::hardware::MQDescriptorUnsync<uint16_t>> result1;
        hw_binder_proxy_->getFmqUnsyncWrite(arg0, [&](bool arg0,const ::android::hardware::MQDescriptorUnsync<uint16_t>& arg1){
            cout << "callback getFmqUnsyncWrite called" << endl;
            result0 = arg0;
            result1.reset(new (std::nothrow) ::android::hardware::MQDescriptorUnsync<uint16_t>(arg1));
        });
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("getFmqUnsyncWrite");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        VariableSpecificationMessage* result_val_1 = result_msg->add_return_type_hidl();
        result_val_1->set_type(TYPE_FMQ_UNSYNC);
        /* ERROR: TYPE_FMQ_UNSYNC is not supported yet. */
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestWriteFmqSync")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        result0 = hw_binder_proxy_->requestWriteFmqSync(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestWriteFmqSync");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestReadFmqSync")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        result0 = hw_binder_proxy_->requestReadFmqSync(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestReadFmqSync");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestWriteFmqUnsync")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        result0 = hw_binder_proxy_->requestWriteFmqUnsync(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestWriteFmqUnsync");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestReadFmqUnsync")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        bool result0;
        result0 = hw_binder_proxy_->requestReadFmqUnsync(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestReadFmqUnsync");
        VariableSpecificationMessage* result_val_0 = result_msg->add_return_type_hidl();
        result_val_0->set_type(TYPE_SCALAR);
        result_val_0->set_scalar_type("bool_t");
        result_val_0->mutable_scalar_value()->set_bool_t(result0);
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestBlockingRead")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        hw_binder_proxy_->requestBlockingRead(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestBlockingRead");
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestBlockingReadDefaultEventFlagBits")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        hw_binder_proxy_->requestBlockingReadDefaultEventFlagBits(arg0);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestBlockingReadDefaultEventFlagBits");
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "requestBlockingReadRepeat")) {
        int32_t arg0 = 0;
        arg0 = func_msg.arg(0).scalar_value().int32_t();
        int32_t arg1 = 0;
        arg1 = func_msg.arg(1).scalar_value().int32_t();
        VtsMeasurement vts_measurement;
        vts_measurement.Start();
        cout << "Call an API" << endl;
        cout << "local_device = " << hw_binder_proxy_.get() << endl;
        hw_binder_proxy_->requestBlockingReadRepeat(arg0, arg1);
        vector<float>* measured = vts_measurement.Stop();
        cout << "time " << (*measured)[0] << endl;
        result_msg->set_name("requestBlockingReadRepeat");
        cout << "called" << endl;
        return true;
    }
    if (!strcmp(func_name, "notifySyspropsChanged")) {
        cout << "Call notifySyspropsChanged" << endl;
        hw_binder_proxy_->notifySyspropsChanged();
        result_msg->set_name("notifySyspropsChanged");
        cout << "called" << endl;
        return true;
    }
    return false;
}

bool FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ::VerifyResults(const FunctionSpecificationMessage& expected_result, const FunctionSpecificationMessage& actual_result) {
    if (!strcmp(actual_result.name().c_str(), "configureFmqSyncReadWrite")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        /* ERROR: TYPE_FMQ_SYNC is not supported yet. */
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "getFmqUnsyncWrite")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        /* ERROR: TYPE_FMQ_UNSYNC is not supported yet. */
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestWriteFmqSync")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestReadFmqSync")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestWriteFmqUnsync")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestReadFmqUnsync")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        if (actual_result.return_type_hidl(0).scalar_value().bool_t() != expected_result.return_type_hidl(0).scalar_value().bool_t()) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestBlockingRead")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestBlockingReadDefaultEventFlagBits")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        return true;
    }
    if (!strcmp(actual_result.name().c_str(), "requestBlockingReadRepeat")) {
        if (actual_result.return_type_hidl_size() != expected_result.return_type_hidl_size() ) { return false; }
        return true;
    }
    return false;
}

extern "C" {
android::vts::FuzzerBase* vts_func_4_android_hardware_tests_msgq_V1_0_ITestMsgQ_() {
    return (android::vts::FuzzerBase*) new android::vts::FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ();
}

android::vts::FuzzerBase* vts_func_4_android_hardware_tests_msgq_V1_0_ITestMsgQ_with_arg(uint64_t hw_binder_proxy) {
    ::android::hardware::tests::msgq::V1_0::ITestMsgQ* arg = reinterpret_cast<::android::hardware::tests::msgq::V1_0::ITestMsgQ*>(hw_binder_proxy);
    android::vts::FuzzerBase* result =
        new android::vts::FuzzerExtended_android_hardware_tests_msgq_V1_0_ITestMsgQ(
            arg);
    arg->decStrong(arg);
    return result;
}

}
}  // namespace vts
}  // namespace android
