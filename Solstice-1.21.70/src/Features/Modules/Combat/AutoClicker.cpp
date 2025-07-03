//
// Created by alteik on 04/09/2024.
//

#include "AutoClicker.hpp"
#include <Features/Events/BaseTickEvent.hpp>
#include <Utils/GameUtils/ActorUtils.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <Features/Modules/Misc/AntiBot.hpp>
#include <Hook/Hooks/MiscHooks/MouseHook.hpp>
#include <SDK/Minecraft/Actor/Components/RuntimeIDComponent.hpp>
#include <SDK/Minecraft/Inventory/Item.hpp>

void AutoClicker::onEnable() {
    gFeatureManager->mDispatcher->listen<RenderEvent, &AutoClicker::onRenderEvent>(this);

    randomizeCPS();
}

void AutoClicker::onDisable() {
    gFeatureManager->mDispatcher->deafen<RenderEvent, &AutoClicker::onRenderEvent>(this);
}

void AutoClicker::onRenderEvent(RenderEvent& event)
{
    auto ci = ClientInstance::get();
    auto player = ci->getLocalPlayer();

    if (!ci->getLocalPlayer() || ci->getScreenName() != "hud_screen") return;

    auto hitres = ci->getLocalPlayer()->getLevel()->getHitResult();

    if (mWeaponsOnly.mValue)
    {
        int slot = player->getSupplies()->mSelectedSlot;
        auto item = player->getSupplies()->getContainer()->getItem(slot);
        if (!item->mItem) return;
        auto type = item->getItem()->getItemType();
        // if type isnt Sword
        if (type != SItemType::Sword) return;
    }


    if (mRandomCPSMin.as<int>() > mRandomCPSMax.as<int>())
        mRandomCPSMin.mValue = mRandomCPSMax.mValue;


    if(mClickMode.mValue == ClickMode::Both)
    {
        bool lmb = ImGui::IsMouseDown(0);
        bool rmb = ImGui::IsMouseDown(1);

        if (mHold.mValue && (!lmb  && !rmb)) return;

        static uint64_t lastAction = 0;
        if (NOW - lastAction < 1000 / mCurrentCPS) return;
        lastAction = NOW;

        if(lmb)
        {
            if(mAllowBlockBreaking.mValue)
            {
                MouseHook::simulateMouseInput(1, 0, 0, 0, 0, 0);
                MouseHook::simulateMouseInput(1, 1, 0, 0, 0, 0);
            }
            else
            {
                MouseHook::simulateMouseInput(1, 1, 0, 0, 0, 0);
                MouseHook::simulateMouseInput(1, 0, 0, 0, 0, 0);
            }
        }

        if(rmb)
        {
            MouseHook::simulateMouseInput(2, 1, 0, 0, 0, 0);
            MouseHook::simulateMouseInput(2, 0, 0, 0, 0, 0);
        }
    }
    else
    {
        int button = mClickMode.as<int>();

        if (mHold.mValue && !ImGui::IsMouseDown(button)) return;

        static uint64_t lastAction = 0;

        if (NOW - lastAction < 1000 / mCurrentCPS) return;

        lastAction = NOW;

        if(button == 0)
        {
            if(mAllowBlockBreaking.mValue)
            {
                MouseHook::simulateMouseInput(1, 0, 0, 0, 0, 0);
                MouseHook::simulateMouseInput(1, 1, 0, 0, 0, 0);
            }
            else
            {
                MouseHook::simulateMouseInput(1, 1, 0, 0, 0, 0);
                MouseHook::simulateMouseInput(1, 0, 0, 0, 0, 0);
            }
        }
        else if(button == 1)
        {
            MouseHook::simulateMouseInput(2, 1, 0, 0, 0, 0);
            MouseHook::simulateMouseInput(2, 0, 0, 0, 0, 0);
        }
    }

    randomizeCPS();
}
