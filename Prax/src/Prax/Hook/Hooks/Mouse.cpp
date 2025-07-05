#pragma once

#include "Mouse.h"

#include <Prax/Utils/System/Memory.h>
#include <imgui.h>
#include <Prax/Hook/Detour.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/ClickGui.h>
#include <Prax/UI/ClickGui.h>
#include <Prax/UI/GUIManager.h>


std::unique_ptr<Detour> mouseCallbackHook = nullptr;

void mouseDetourFunc(void* Instance, int mouseButton, int isDown, __int16 mouseX, __int16 mouseY, __int64 a1, __int64 a2, __int64 a3) {

    auto oFunc = mouseCallbackHook.get()->GetFastcall<void, void*, int, int, __int16, __int16, __int64, __int64, __int64>();

    bool cancel = false;

    for (auto& module : ModuleManager::Modules) {
        if (module->Enabled) {
            module->OnMouseInput(Instance, mouseButton, isDown, mouseX, mouseY, a1, a2, a3, &cancel);
        }
    }

    if (ImGui::GetCurrentContext() != nullptr) {
        ImGuiIO& io = ImGui::GetIO();

        if (mouseX != 0 && mouseY != 0) {
            io.MousePos = ImVec2(mouseX, mouseY);
        }


        switch (mouseButton) {
            case 1:
                io.MouseDown[0] = isDown;
                break;
            case 2:
                io.MouseDown[1] = isDown;
                break;
            case 3:
                io.MouseDown[2] = isDown;
                break;
            case 4:
                if(isDown == 0x78 || isDown == 0x7F)
                    io.AddMouseWheelEvent(0, 0.5);
                else if (isDown == 0x88 || isDown == 0x80)
                    io.AddMouseWheelEvent(0, -0.5);
                break;
            default:
                break;
        }

        if (!io.WantCaptureMouse && !cancel && !GUIManager::Visible)
            return oFunc(Instance, mouseButton, isDown, mouseX, mouseY, a1, a2, a3);

    }
    else if (!cancel) return oFunc(Instance, mouseButton, isDown, mouseX, mouseY, a1, a2, a3);
}

void Mouse::Init() {

    uintptr_t mouseFuncAddr = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Mouse));
    mouseCallbackHook = std::make_unique<Detour>("Mouse", mouseFuncAddr, &mouseDetourFunc);
    mouseCallbackHook->Enable();
    this->Detours.emplace_back(mouseCallbackHook.get());
}

void Mouse::UnHook() {
    mouseCallbackHook->Restore();
}
