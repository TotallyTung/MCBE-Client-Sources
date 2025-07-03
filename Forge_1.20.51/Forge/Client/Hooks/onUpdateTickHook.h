#pragma once

void* oUpdateTick;

void onUpdate(GameMode* gm) {

    for (auto module : modules) {
        module->onTick();
    }

    CallFunc<void*, GameMode*>(oUpdateTick, gm);
}

class onUpdateHook : public FuncHook
{
public:
    static onUpdateHook& Instance() {
        static onUpdateHook instance;
        return instance;
    }

    bool Initialize() override
    {
        uintptr_t gamemodeAddr = findSig(xorstr_("48 8B 89 ? ? ? ? 48 8B 01 48 8B 40 18 FF 15 ? ? ? ? 8B 43 24 89 43 20 48 83 C4 20 5B"), "Gamemode");

        if (!HookFunction((void*)gamemodeAddr, &onUpdate, &oUpdateTick, xorstr_("GamemodeHook")))
            return false;

        return true;
    }
};