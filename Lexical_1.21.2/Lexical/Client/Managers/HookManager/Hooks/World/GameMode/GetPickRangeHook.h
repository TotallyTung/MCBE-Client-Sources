#pragma once
#include "../../FuncHook.h"

class GetPickRangeHook : public FuncHook {
private:
    using func_t = float(__thiscall*)(GameMode*, InputMode const&, bool);
    static inline func_t oFunc;

    static float GameModeGetPickRangeCallback(GameMode* _this, InputMode const& currentInputMode, bool isVR) {
        static BlockReach* blockReachMod = ModuleManager::getModule<BlockReach>();
        if (blockReachMod->isEnabled())
            return blockReachMod->blockReach;
        return oFunc(_this, currentInputMode, isVR);
    }

public:
    GetPickRangeHook() {
        OriginFunc = (void*)&oFunc;
        func = (void*)&GameModeGetPickRangeCallback;
    }
};
