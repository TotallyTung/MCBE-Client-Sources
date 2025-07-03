#pragma once
#include "../../../../SDK/Classes/Actor.h"
#include "../../../../Utils/MemoryUtils.h"

class ActorShouldRenderHook {
protected:
    using func_t = bool(__fastcall*)(Actor*);
    static inline func_t func;
    static inline CrystalChams* chams = nullptr;

    static bool Actor_ShouldRender(Actor* _this) {
        if (chams && chams->isEnabled() && _this->getEntityTypeId() == 71) {
            return false; 
        }
        return func(_this);
    }

public:
    static void init() {
        uintptr_t address = findSig(Sigs::hook::ShouldRender);
        MemoryUtils::CreateHook("Actor_shouldRenderHook", address, (void*)&ActorShouldRenderHook::Actor_ShouldRender, (void*)&func);
        chams = (CrystalChams*)client->moduleMgr->getModule("CrystalChams");
    }
};