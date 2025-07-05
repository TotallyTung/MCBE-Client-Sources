#pragma once

#include <Prax/SDK/Game/Network/Network.h>
#include "TestModule.h"

#include <imgui_internal.h>
#include <Prax/Hook/Hooks/RakPeerHooks.h>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/Modules/Player/Regen.h>
#include "Prax/SDK/Game/Entity/Components/ActorTypeComponent.h"
#include <Prax/Utils/Render/Render.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Utils/Math.h>
#include <vector>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>
#include <Prax/SDK/Game/Network/Other/InventoryContentPacket.h>

#include <windows.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.Foundation.Collections.h>
#include <winrt/Windows.System.h>

#include "Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h"
#include "Prax/SDK/Game/Network/Other/MobEffectPacket.h"
#include "Prax/SDK/Game/Network/Other/SetScorePacket.h"

bool AddPointerToGui(std::string name, uintptr_t address) {
    ImGui::Text("%s, %s", name.c_str(), String::ToHex(address).c_str());


    if (ImGui::Button(std::string("Copy, " + name).c_str())) {
        ImGui::SetClipboardText(String::ToHex(address).c_str());
    }

    return address != 0;
}
int TestModule::Tick = 0;


#define NOW Misc::GetCurrentMs();

void TestModule::OnEnable()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->logAllFieldAddresses();

}

void TestModule::OnDisable()
{
    /*auto player = Minecraft::ClientInstance->ThePlayer();
    player->GetStateVectorComponent()->velocity = Vector3(0, 0, 0);
    */
    Minecraft::ClientInstance->SetTimerMultiplier(1.0f);
}

void TestModule::OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled)
{

}


void TestModule::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
}


void TestModule::OnTick()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto hitRes = player->level->getHitResult();

    if (hitRes->type == RaycastTypes::ENTITY)
    {
        Actor* actor = nullptr;
        for (auto&& [id, moduleOwner, type, ridc] : player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each())
        {
            if (moduleOwner.actor->entityCtx.entityId == hitRes->entity.id) {
                actor = moduleOwner.actor;
                break;
            }
        }

        if (actor) player->addChatMessage("&6[TestModule] &7Entity: " + actor->entityIdString + " [hitboxSize: " + std::to_string(actor->GetAABBShapeComponent()->hitboxWidth) + "x" + std::to_string(actor->GetAABBShapeComponent()->hitboxHeight));
    }
}

auto modules = std::vector<Module*>();

void TestModule::OnRender() {
 // Smoothly vertically scrolling numbers example

    if (NaClHud.Enabled && Minecraft::ClientInstance->GetScreenName() != "start_screen") {
        ImFont *font = Font::Fonts["sarabun_light"];

        ImGui::PushFont(font);

        std::string watermark = Prax::ClientName + " | " + Minecraft::ClientInstance->ThePlayer()->localName + " | do not redistribute";

        ImVec2 textSize = ImGui::GetFont()->CalcTextSizeA(23, FLT_MAX, 0.0f, watermark.c_str());

        ImColor textColor = ImColor(255, 255, 255);

        ImVec2 pos = ImVec2(10, 10);

        Render::DrawRect(pos, ImVec2(pos.x + textSize.x + 10, pos.y + textSize.y + 5), ImColor(0, 0, 0, 150));

        Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(pos.x + textSize.x + 10, pos.y),
                         textColor, 1.0f);

        Render::RenderText(watermark, ImVec2(pos.x + 5, pos.y + 2), 23, textColor, true);

        Render::FillAreaWithBlur(2.0f, ImVec4(pos.x, pos.y, pos.x + textSize.x + 10, pos.y + textSize.y + 5));

        Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(pos.x + textSize.x + 10, pos.y),
                             textColor, 1.0f);

        Render::RenderText(watermark, ImVec2(pos.x + 5, pos.y + 2), 23, textColor, false);

        pos = ImVec2(10, 10 + textSize.y + 5 + 10);

        modules = ModuleManager::Modules;

        std::ranges::sort(modules, [font](Module* a, Module* b) {


            float aSize = font->CalcTextSizeA(23, FLT_MAX, 0.0f, a->Name.c_str()).x;
            float bSize = font->CalcTextSizeA(23, FLT_MAX, 0.0f, b->Name.c_str()).x;

            std::string aSettingText = a->GetSettingDisplayText();
            std::string bSettingText = b->GetSettingDisplayText();
            if (!aSettingText.empty()) {
                aSize += font->CalcTextSizeA(23,
                                             FLT_MAX,
                                             0,
                                             (" [" + aSettingText + "]").c_str()).x;
            }
            if (!bSettingText.empty()) {
                bSize += font->CalcTextSizeA(23,
                                             FLT_MAX,
                                             0,
                                             (" [" + b->GetSettingDisplayText() + "]").c_str()).x;
            }


            return aSize > bSize;
        });

        bool top = false;
        float offset = 0;

        const float delta = ImGui::GetIO().DeltaTime;
        const auto fgDrawList = ImGui::GetForegroundDrawList();

        const auto fullOpacityColor = ImColor(textColor);
        auto lastWidth = 0.f;

        for (auto &module: modules) {
            if (module->HideInArraylist) continue;
            if (!module->Enabled && module->UIInfo.arrayListPercent <= 0.001f) continue;

            Render::PopDrawList();

            module->UIInfo.arrayListPercent = Math::Animate(module->UIInfo.arrayListPercent, module->Enabled ? 1.f : 0.f,delta * 12.f);
            module->UIInfo.arrayListPercent = std::clamp(module->UIInfo.arrayListPercent, 0.f, 1.f);

            const float percent = module->UIInfo.arrayListPercent;
            const float opacity = percent < 0.2f ? 0.f : (percent - 0.2f) / 0.8f;

            std::string str = module->Name;

            std::string settingText = module->GetSettingDisplayText();

            if (!settingText.empty()) {
                str += " [" + settingText + "]";
            }

            textSize = ImGui::GetFont()->CalcTextSizeA(23, FLT_MAX, 0.0f, str.c_str());
            textColor = ImColor(255, 255, 255, (int) (opacity * 255));

            const float ySize = (textSize.y + 3) * percent;

            float x = pos.x + textSize.x + 11;

            if (!top) {
                Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(pos.x + textSize.x + 10, pos.y),
                                     fullOpacityColor, 1.0f);
   //         } else {
   //             Render::DrawGlowLine(ImVec2(lastWidth, pos.y), ImVec2(pos.x + textSize.x + 11, pos.y),
   //                                  fullOpacityColor, 1.0f);
            }

    //        Render::DrawGlowLine(ImVec2(x, pos.y), ImVec2(x, pos.y + ySize), fullOpacityColor, 1.0f);

            Render::DrawRect(pos, ImVec2(x, pos.y + ySize), ImColor(0, 0, 0, 150));

            Render::RenderText(str, ImVec2(pos.x + 5, pos.y + (1 * percent)), 23, textColor, true);

            Render::PushDrawList(fgDrawList);

            Render::FillAreaWithBlur(2.0f, ImVec4(pos.x, pos.y, x, pos.y + ySize));

            if (!top) {
                Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(pos.x + textSize.x + 10, pos.y),
                                     fullOpacityColor, 1.0f);
                top = true;
     //       } else {
     //           Render::DrawGlowLine(ImVec2(lastWidth, pos.y), ImVec2(pos.x + textSize.x + 11, pos.y),
     //                                fullOpacityColor, 1.0f);
            }

            Render::RenderText(str, ImVec2(pos.x + 5, pos.y + (1 * percent)), 23, textColor, false);

            offset += ySize;
            pos = ImVec2(10, 10 + textSize.y + 5 + 10 + offset);
            lastWidth = x;

        }

        Render::DrawGlowLine(ImVec2(pos.x, pos.y - offset), ImVec2(pos.x, pos.y),
                             fullOpacityColor, 1.0f);

 //       Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(lastWidth, pos.y),
 //                                    fullOpacityColor, 1.0f);

        Render::PopDrawList();

        Render::DrawGlowLine(ImVec2(pos.x, pos.y - offset), ImVec2(pos.x, pos.y),
                             fullOpacityColor, 1.0f);

  //      Render::DrawGlowLine(ImVec2(pos.x, pos.y), ImVec2(lastWidth, pos.y),
  //                                   fullOpacityColor, 1.0f);

        ImGui::PopFont();
    }

#ifdef __DEBUG__
    if (DebugUi.Enabled) {
        auto player = Minecraft::ClientInstance->ThePlayer();

        ImGui::Begin("Debug UI");

        if (AddPointerToGui("ClientInstance", (uintptr_t) Minecraft::ClientInstance)) {

            // Add all subclasses
            AddPointerToGui("Player", (uintptr_t) player);
            if (player) {
                // Add some important player components
                AddPointerToGui("JumpControlComponent", (uintptr_t) player->GetJumpComponent());
                AddPointerToGui("StateVectorComponent", (uintptr_t) player->GetStateVectorComponent());
                AddPointerToGui("MoveInputComponent", (uintptr_t) player->GetMoveInputComponent());
                AddPointerToGui("MobEffectsComponent", (uintptr_t) player->GetMobEffectsComponent());
            }

            AddPointerToGui("ClientInstance (from MinecraftGame)", (uintptr_t) MinecraftGame::GetInstance()->getPrimaryClientInstance());
            AddPointerToGui("MinecraftGame", (uintptr_t) Minecraft::ClientInstance->mcGame);
            AddPointerToGui("guiData", (uintptr_t) Minecraft::ClientInstance->guiData);
            AddPointerToGui("minecraft", (uintptr_t) Minecraft::ClientInstance->minecraft);
            AddPointerToGui("levelRenderer", (uintptr_t) Minecraft::ClientInstance->levelRenderer);
           // if (Minecraft::ClientInstance->levelRenderer) {
           //     AddPointerToGui("levelRendererPlayer",
           //                     (uintptr_t) Minecraft::ClientInstance->levelRenderer->levelRendererPlayer);
           // }
            AddPointerToGui("gfx_field_of_view",
                            (uintptr_t) Minecraft::ClientInstance->GetOptions()->gfx_field_of_view);
            AddPointerToGui("getOptions", (uintptr_t) Minecraft::ClientInstance->GetOptions());
        }

        ImGui::End();
    }
#endif
}
