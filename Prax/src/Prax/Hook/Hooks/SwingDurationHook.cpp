//
// Created by vastrakai on 9/23/2023.
//

#include "SwingDurationHook.h"

#include <mat4x4.hpp>
#include <imgui_internal.h>
#include <trigonometric.hpp>
#include <Prax/Utils/String.h>
#include <Prax/SDK/SigManager.h>
#include <ext/matrix_transform.hpp>
#include <Prax/Utils/System/Memory.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/Swing.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/Module/Modules/Visual/ViewModel.h>

std::shared_ptr<Detour> swingDurationDetour = nullptr;

int swingDurationDetourFunc(void* _this) {
    if (_this != Minecraft::ClientInstance->ThePlayer()) return 6;
    auto mod = ModuleManager::GetModule<Swing>();
    if (mod == nullptr) return 6;
    auto swing = (Swing*)mod;
    return swing->Enabled ? (float) swing->SwingSpeed.Value : 6;
}


std::shared_ptr<Detour> bobHurtDetour = nullptr;
// glm::mat4 is from the glm library, it's a 4x4 matrix
void* bobHurtCallbackFunc(void* _this, glm::mat4* matrix) {
    auto oFunc = bobHurtDetour->GetFastcall<void *, void *, glm::mat4 *>(); // Get the original function
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return oFunc(_this, matrix);

    static auto mod = ModuleManager::GetModule<Swing>();

    if (!Minecraft::ClientInstance->mcGame->canUseKeys() || mod == nullptr || !mod->Enabled) {
        for (auto& module : ModuleManager::Modules) {
            if (module->Enabled) module->OnRenderMatrix(matrix);
        }

        return oFunc(_this, matrix);
    }



    for (auto& module : ModuleManager::Modules) {
        if (module->Enabled) module->OnRenderMatrix(matrix);
    }

    ItemStack* itemStack = player->GetItemInHand();
    if (!itemStack || !itemStack->item) return oFunc(_this, matrix);
    auto* item = *itemStack->item;
    if (!String::ContainsIgnoreCase(item->GetName(), "sword")) {
        return oFunc(_this, matrix);
    }

    if (ImGui::IsMouseDown(ImGuiMouseButton_Right) || Aura::HasTarget) {
        // Blocking
        if (mod->AnimationMode.SelectedIndex == (int)Swing::AnimationModes::BlockHit) {
            *matrix = glm::translate(*matrix, glm::vec3(0.4, 0.0, -0.15));
            *matrix = glm::translate(*matrix, glm::vec3(-0.1f, 0.15f, -0.2f));
            *matrix = glm::translate(*matrix, glm::vec3(-0.24F, 0.25f, -0.20F));
            *matrix = glm::rotate(*matrix, -1.98F, glm::vec3(0.0F, 1.0F, 0.0F));
            *matrix = glm::rotate(*matrix, 1.30F, glm::vec3(4.0F, 0.0F, 0.0F));
            *matrix = glm::rotate(*matrix, 59.9F, glm::vec3(0.0F, 1.0F, 0.0F));
            *matrix = glm::translate(*matrix, glm::vec3(0.0f, -0.1f, 0.15f));
            *matrix = glm::translate(*matrix, glm::vec3(0.08f, 0.0f, 0.0f));
            *matrix = glm::scale(*matrix, glm::vec3(1.05f, 1.05f, 1.05f));

            static float oldSwingProgress = 0.f;
            static float swingProgress = 0.f;
            oldSwingProgress = swingProgress;
            swingProgress = player->timeAlongSwing;

            static float swingProgressPercent = 0.f;
            swingProgressPercent = std::lerp(swingProgressPercent, swingProgress, ImGui::GetIO().DeltaTime * 10);


            float adjustedSwingProgressPercent = swingProgressPercent;

            adjustedSwingProgressPercent *= 2;
            // make the swing progress percent start going down once it reaches 1
            if (adjustedSwingProgressPercent > 1) {
                adjustedSwingProgressPercent = 2 - adjustedSwingProgressPercent;
            }

            // fix the animation for blocking to look more like java

            //*matrix = glm::translate(*matrix, glm::vec3(0.0f, 0.0f, mod->TestNumber.Value * adjustedSwingProgressPercent));




        } else if (mod->AnimationMode.SelectedIndex == (int)Swing::AnimationModes::Spin) {
            float maxYawDeg = 179.9;
            float minYawDeg = -179.9;

            auto yawDeg = (float) (Misc::GetCurrentMs() / (uint64_t)mod->SpinSpeed.Value % (int) (maxYawDeg - minYawDeg) + minYawDeg);
            float yawRad = glm::radians(yawDeg);

            glm::mat4 oldMatrix = *matrix;

            glm::vec3 pivotPoint = glm::vec3(0, -0.20f, -0.5f);

            // Translate to the pivot point, rotate, and then translate back
            *matrix = glm::translate(*matrix, pivotPoint);
            *matrix = glm::rotate(*matrix, yawRad, glm::vec3(1, 0, 0));
            *matrix = glm::translate(*matrix, -pivotPoint);
        }
    }


    return oFunc(_this, matrix);
}

void SwingDurationHook::Init() {

    uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::Mob_getCurrentSwingDuration));
    swingDurationDetour = std::make_shared<Detour>("swingDurationDetour", func, &swingDurationDetourFunc);
    swingDurationDetour->Enable();
    this->Detours.emplace_back(swingDurationDetour.get());

    uintptr_t func2 = SigManager::GetSignatureAddress(SignatureEnum::bobHurt);
    bobHurtDetour = std::make_shared<Detour>("bobHurtDetour", func2, &bobHurtCallbackFunc);
    bobHurtDetour->Enable();
    this->Detours.emplace_back(bobHurtDetour.get());

}

void SwingDurationHook::UnHook() {

}
