#pragma once
#include "../FuncHook.h"
#include <libhat/Access.hpp>

class GetCameraComponentHook : public FuncHook {
private:
    using func_t = void* (__thiscall*)(__int64, __int64);
    static inline func_t oFunc;
    static void* GetCameraComponentCallback(__int64 a1, __int64 a2) {
        void* result = oFunc(a1, a2);
        static void* cameraDistanceCall = reinterpret_cast<void*>(MemoryUtil::findSignature("48 85 C0 74 06 F3 44 0F"));
        if (cameraDistanceCall == nullptr || result == nullptr) {
            return result;
        }
        if (_ReturnAddress() == cameraDistanceCall) {
            try {
                float& cameraDistance = hat::member_at<float>(result, 0x14);
                cameraDistance = 4.f;
            }
            catch (const std::exception& e) {
            }
        }

        return result;
    }

public:
    GetCameraComponentHook() {
        OriginFunc = (void*)&oFunc;
        func = (void*)&GetCameraComponentCallback;
    }
};
