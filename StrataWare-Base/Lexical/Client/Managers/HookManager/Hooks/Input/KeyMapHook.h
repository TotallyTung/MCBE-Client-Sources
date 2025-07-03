#pragma once
#include "../FuncHook.h"
#include "../../../../Client.h"
#include <iostream> 

class KeyMapHook : public FuncHook {
private:
    using func_t = __int64(__fastcall*)(uint64_t, bool);
    static inline func_t oFunc;

    static void Log(const std::string& message) {
        std::cout << message << std::endl;
    }

    static __int64 KeyInputCallback(uint64_t key, bool isDown) {
        if (!oFunc)
            return 0;

        try {
            Log("Key input detected. Key: " + std::to_string(key) + " | IsDown: " + std::to_string(isDown));

            auto* clickGuiMod = ModuleManager::getModule<ClickGUI>();
            if (clickGuiMod && clickGuiMod->isEnabled()) {
                clickGuiMod->onKeyUpdate(static_cast<int>(key), isDown);
                Log("ClickGUI module is active. Key handled by ClickGUI.");
                return 0;
            }

            if (Game::canUseMoveKeys()) {
                ModuleManager::onKeyUpdate(static_cast<int>(key), isDown);
                Log("Key handled by other modules (movement allowed).");
            }
        }
        catch (const std::exception& e) {
            Log("Exception caught in KeyInputCallback: " + std::string(e.what()));
            return 0;
        }
        catch (...) {
            Log("Unknown exception caught in KeyInputCallback.");
            return 0;
        }
        return oFunc(key, isDown);
    }

public:
    static void createKeyPress(uint64_t key, bool down) {
        if (oFunc) oFunc(key, down);
    }

    KeyMapHook() {
        OriginFunc = reinterpret_cast<void*>(&oFunc);
        func = reinterpret_cast<void*>(&KeyInputCallback);
    }

    void onHookRequest() override {
        uintptr_t sigOffset = this->address + 7;
        if (!sigOffset) return;

        int offset = *reinterpret_cast<int*>(sigOffset + 3);
        uintptr_t keyMapOffset = sigOffset + offset + 7;

        Game::keyMapPtr = reinterpret_cast<void*>(keyMapOffset);
        Log("KeyMap pointer set in Game::keyMapPtr.");
    }
};
