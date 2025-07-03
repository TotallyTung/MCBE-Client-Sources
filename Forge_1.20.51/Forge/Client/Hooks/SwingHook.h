#pragma once
//#include "../SDK/GameMode.h"
//#include <inttypes.h>
void* oLerpMotion;
void* oSwingSpeed;
void* oFieldHook;

bool ActorLerpMotion(void* _this, Vector3<float> motion) {
    if (getModuleByName("velocity")->isEnabled()) {
        return false;
    }
    else {
        return true;
    }

    return CallFunc<bool, void*, Vector3<float>>(
        oLerpMotion,
        _this, motion
    );
}

int getCurrentSwingDurationDetour() {
    float ReturnValue = Game::Core::SwingSpeed;

    return ReturnValue;
}

void* FieldDetour(void* a1, void* a2, void* a3, void* a4, float* a5)
{
    float* fieldMulti = reinterpret_cast<float*>((uintptr_t)a1 + 0x1350);

    *fieldMulti = 0.3f; // default is 1.1f (idky lol)

    return CallFunc<void*, void*, void*, void*, void*, float*>(
        oFieldHook,
        a1,
        a2,
        a3,
        a4,
        a5
    );
}

class ActorHook : public FuncHook
{
public:
    static ActorHook& Instance() {
        static ActorHook instance;
        return instance;
    }

    bool Initialize() override {
        uintptr_t lerpMotionAddr = findSig(xorstr_("48 83 EC 28 4C 8B 81 ? ? ? ? 4D 85 C0 74 19 8B 02 41 89 40 18"), "LerpMotion");
        uintptr_t swingSpeedAddr = findSig(xorstr_("48 89 5C 24 ?? 57 48 83 EC ?? 48 8B 15 ?? ?? ?? ?? 48 8B F9"), "SwingDuration");
        uintptr_t fieldAddr = findSig(xorstr_("48 89 5C 24 18 56 48 83 EC 60 ? ? 74"), "FieldHook");

        if (!HookFunction((void*)lerpMotionAddr, (void*)&ActorLerpMotion, &oLerpMotion, xorstr_("LerpMotion"))) {
            return false;
        }

        if (!HookFunction((void*)swingSpeedAddr, (void*)&getCurrentSwingDurationDetour, &oSwingSpeed, xorstr_("SwingDuration"))) {
            return false;
        }
        if (!HookFunction((void*)fieldAddr, (void*)&FieldDetour, &oFieldHook, xorstr_("FeildHook"))) {
            return false;
        }
        return true;
    }
};
