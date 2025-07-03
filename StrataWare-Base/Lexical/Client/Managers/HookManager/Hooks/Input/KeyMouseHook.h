#pragma once
#include "../FuncHook.h"
#include <iostream>
class KeyMouseHook : public FuncHook {
private:
    using func_t = void(__fastcall*)(__int64, char, char, __int16, __int16, __int16, __int16, char);
    static inline func_t oFunc;

    static void Log(const std::string& message) {
        std::cout << message << std::endl;
    }

    static void mouseInputCallback(__int64 a1, char mouseButton, char isDown, __int16 mouseX, __int16 mouseY, __int16 relativeMovementX, __int16 relativeMovementY, char a8) {
        if (!a1) {
            Log("Invalid pointer detected in mouseInputCallback.");
            return;
        }

        try {
            static ClickGUI* clickGuiMod = ModuleManager::getModule<ClickGUI>();
            if (clickGuiMod && clickGuiMod->isEnabled()) {
                Log("ClickGUI module is enabled. Handling mouse input.");
                clickGuiMod->onMouseUpdate(Vec2<float>((float)mouseX, (float)mouseY), mouseButton, isDown);
                return;
            }

            Log("Mouse Input - Button: " + std::to_string((int)mouseButton) +
                " | Down: " + std::to_string((int)isDown) +
                " | X: " + std::to_string((int)mouseX) +
                " | Y: " + std::to_string((int)mouseY));
            if (oFunc) {
                oFunc(a1, mouseButton, isDown, mouseX, mouseY, relativeMovementX, relativeMovementY, a8);
            }
        }
        catch (const std::exception& e) {
            Log("Exception caught in mouseInputCallback: " + std::string(e.what()));
        }
        catch (...) {
            Log("Unknown exception caught in mouseInputCallback.");
        }
    }

public:
    KeyMouseHook() {
        OriginFunc = reinterpret_cast<void*>(&oFunc);
        func = reinterpret_cast<void*>(&mouseInputCallback);
    }
};
