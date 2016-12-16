#include "hardware/interfaces/nfc/1.0/vts/Nfc.vts.h"
#include "hardware/interfaces/nfc/1.0/vts/types.vts.h"

using namespace android::hardware;
using namespace android::hardware::nfc::V1_0;

namespace android {
namespace vts {


void HIDL_INSTRUMENTATION_FUNCTION(
        InstrumentationEvent event,
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
    if (strcmp(method, "open") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                auto *arg_0 = msg.add_arg();
                INfcClientCallback *arg_val_0 = reinterpret_cast<INfcClientCallback*> ((*args)[0]);
                arg_0->set_type(TYPE_HIDL_CALLBACK);
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "write") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                auto *arg_0 = msg.add_arg();
                hidl_vec<uint8_t> *arg_val_0 = reinterpret_cast<hidl_vec<uint8_t>*> ((*args)[0]);
                for (int i = 0; i < (int)(*arg_val_0).size(); i++) {
                    auto *arg_0_vector_i = arg_0->add_vector_value();
                    arg_0_vector_i->set_type(TYPE_SCALAR);
                    arg_0_vector_i->mutable_scalar_value()->set_uint8_t((*arg_val_0)[i]);
                }
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "coreInitialized") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                auto *arg_0 = msg.add_arg();
                hidl_vec<uint8_t> *arg_val_0 = reinterpret_cast<hidl_vec<uint8_t>*> ((*args)[0]);
                for (int i = 0; i < (int)(*arg_val_0).size(); i++) {
                    auto *arg_0_vector_i = arg_0->add_vector_value();
                    arg_0_vector_i->set_type(TYPE_SCALAR);
                    arg_0_vector_i->mutable_scalar_value()->set_uint8_t((*arg_val_0)[i]);
                }
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "prediscover") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "close") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "controlGranted") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
    if (strcmp(method, "powerCycle") == 0) {
        FunctionSpecificationMessage msg;
        switch (event) {
            case SERVER_API_ENTRY:
            {
                break;
            }
            case SERVER_API_EXIT:
            {
                auto *result_0 = msg.add_return_type_hidl();
                int32_t *result_val_0 = reinterpret_cast<int32_t*> ((*args)[0]);
                result_0->set_type(TYPE_SCALAR);
                result_0->mutable_scalar_value()->set_int32_t((*result_val_0));
                break;
            }
            default:
            {
                LOG(WARNING) << "not supported. ";
                break;
            }
        }
        VtsProfilingInterface::getInstance().AddTraceEvent(msg);
    }
}

}  // namespace vts
}  // namespace android
