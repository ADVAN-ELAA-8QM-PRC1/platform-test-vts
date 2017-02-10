#include "hardware/interfaces/nfc/1.0/vts/Nfc.vts.h"
#include "hardware/interfaces/nfc/1.0/vts/NfcClientCallback.vts.h"
#include "hardware/interfaces/nfc/1.0/vts/types.vts.h"

using namespace android::hardware::nfc::V1_0;
using namespace android::hardware;

#define TRACEFILEPREFIX "/data/local/tmp"

namespace android {
namespace vts {
namespace vtsINfc {


void HIDL_INSTRUMENTATION_FUNCTION_android_hardware_nfc_V1_0_INfc(
        HidlInstrumentor::InstrumentationEvent event,
        const char* package,
        const char* version,
        const char* interface,
        const char* method,
        std::vector<void *> *args) {
    if (strcmp(package, "android.hardware.nfc") != 0) {
        LOG(WARNING) << "incorrect package.";
        return;
    }
    if (strcmp(version, "1.0") != 0) {
        LOG(WARNING) << "incorrect version.";
        return;
    }
    if (strcmp(interface, "INfc") != 0) {
        LOG(WARNING) << "incorrect interface.";
        return;
    }

    char trace_file[PATH_MAX];
    sprintf(trace_file, "%s/%s@%s", TRACEFILEPREFIX, package, version);
    VtsProfilingInterface& profiler = VtsProfilingInterface::getInstance(trace_file);
    profiler.Init();

    if (strcmp(method, "open") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("open");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 1, actual: " << (*args).size() << ", method name: open, event type: " << event;
                        break;
                    }
                    auto *arg_0 = msg.add_arg();
                    sp<INfcClientCallback> *arg_val_0 = reinterpret_cast<sp<INfcClientCallback>*> ((*args)[0]);
                    arg_0->set_type(TYPE_HIDL_CALLBACK);
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: open, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "write") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("write");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 1, actual: " << (*args).size() << ", method name: write, event type: " << event;
                        break;
                    }
                    auto *arg_0 = msg.add_arg();
                    ::android::hardware::hidl_vec<uint8_t> *arg_val_0 = reinterpret_cast<::android::hardware::hidl_vec<uint8_t>*> ((*args)[0]);
                    for (int i = 0; i < (int)(*arg_val_0).size(); i++) {
                        auto *arg_0_vector_i = arg_0->add_vector_value();
                        arg_0_vector_i->set_type(TYPE_SCALAR);
                        arg_0_vector_i->mutable_scalar_value()->set_uint8_t((*arg_val_0)[i]);
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: write, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    uint32_t *result_val_0 = reinterpret_cast<uint32_t*> ((*args)[0]);
                    result_0->set_type(TYPE_SCALAR);
                    result_0->mutable_scalar_value()->set_uint32_t((*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "coreInitialized") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("coreInitialized");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 1, actual: " << (*args).size() << ", method name: coreInitialized, event type: " << event;
                        break;
                    }
                    auto *arg_0 = msg.add_arg();
                    ::android::hardware::hidl_vec<uint8_t> *arg_val_0 = reinterpret_cast<::android::hardware::hidl_vec<uint8_t>*> ((*args)[0]);
                    for (int i = 0; i < (int)(*arg_val_0).size(); i++) {
                        auto *arg_0_vector_i = arg_0->add_vector_value();
                        arg_0_vector_i->set_type(TYPE_SCALAR);
                        arg_0_vector_i->mutable_scalar_value()->set_uint8_t((*arg_val_0)[i]);
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: coreInitialized, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "prediscover") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("prediscover");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 0) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 0, actual: " << (*args).size() << ", method name: prediscover, event type: " << event;
                        break;
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: prediscover, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "close") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("close");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 0) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 0, actual: " << (*args).size() << ", method name: close, event type: " << event;
                        break;
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: close, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "controlGranted") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("controlGranted");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 0) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 0, actual: " << (*args).size() << ", method name: controlGranted, event type: " << event;
                        break;
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: controlGranted, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
    if (strcmp(method, "powerCycle") == 0) {
        FunctionSpecificationMessage msg;
        msg.set_name("powerCycle");
        if (!args) {
            LOG(WARNING) << "no argument passed";
        } else {
            switch (event) {
                case HidlInstrumentor::CLIENT_API_ENTRY:
                case HidlInstrumentor::SERVER_API_ENTRY:
                case HidlInstrumentor::PASSTHROUGH_ENTRY:
                {
                    if ((*args).size() != 0) {
                        LOG(ERROR) << "Number of arguments does not match. expect: 0, actual: " << (*args).size() << ", method name: powerCycle, event type: " << event;
                        break;
                    }
                    break;
                }
                case HidlInstrumentor::CLIENT_API_EXIT:
                case HidlInstrumentor::SERVER_API_EXIT:
                case HidlInstrumentor::PASSTHROUGH_EXIT:
                {
                    if ((*args).size() != 1) {
                        LOG(ERROR) << "Number of return values does not match. expect: 1, actual: " << (*args).size() << ", method name: powerCycle, event type: " << event;
                        break;
                    }
                    auto *result_0 = msg.add_return_type_hidl();
                    ::android::hardware::nfc::V1_0::NfcStatus *result_val_0 = reinterpret_cast<::android::hardware::nfc::V1_0::NfcStatus*> ((*args)[0]);
                    result_0->set_type(TYPE_ENUM);
                    profile____android__hardware__nfc__V1_0__NfcStatus(result_0, (*result_val_0));
                    break;
                }
                default:
                {
                    LOG(WARNING) << "not supported. ";
                    break;
                }
            }
        }
        profiler.AddTraceEvent(event, package, version, interface, msg);
    }
}

}  // namespace vtsINfc
}  // namespace vts
}  // namespace android
