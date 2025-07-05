#pragma once

#include "HookManager.h"

#include <Prax/Hook/Hooks/ActorIsInvisible.h>
#include <Prax/Hook/Hooks/ActorRenderDispatcher.h>
#include <Prax/Hook/Hooks/ActorSetNametag.h>
#include <Prax/Hook/Hooks/ActorSwing.h>
#include <Prax/Hook/Hooks/GamemodeBuildBlock.h>
#include <Prax/Hook/Hooks/PacketHandler.h>
#include <Prax/Hook/Hooks/Social.h>
#include <Prax/Hook/Hooks/SplashTextRenderer.h>
#include <Prax/Hook/Hooks/SwingDurationHook.h>

#include "Hooks/BaseTick.h"
#include "Hooks/BlockSourceGetBlock.h"
#include "Hooks/ContainerScreenControllerHook.h"
#include "Hooks/D3D.h"
#include "Hooks/DimensionH.h"
#include "Hooks/GamemodeAttack.h"
#include "Hooks/GamemodeGetPickRange.h"
#include "Hooks/ItemUseSlowdownSystem.h"
#include "Hooks/KeyMap.h"
#include "Hooks/LevelRendererPlayerHooks.h"
#include "Hooks/Mouse.h"
#include "Hooks/NoRenderHooks.h"
#include "Hooks/OverworldDimensionHooks.h"
#include "Hooks/PacketSender.h"
#include "Hooks/RakPeerHooks.h"
#include "Hooks/RenderItemInHandDescriptionHook.h"
#include "Hooks/UIRenderCtx.h"


std::vector<Hook*> HookManager::Hooks = std::vector<Hook*>();

void HookManager::Init() {
    //Hooks.emplace_back(new DimensionH());
    //Hooks.emplace_back(new ClientInstanceTick());

    uint64_t start = Misc::GetCurrentMs();

    Hooks.emplace_back(new UIRenderCtx());
    Hooks[0]->Init();

    while (!Minecraft::ClientInstance) {
        Sleep(1);
    }

    Hooks.emplace_back(new Social());
    Hooks.emplace_back(new ContainerScreenControllerHook());
    Hooks.emplace_back(new KeyMap());
    Hooks.emplace_back(new D3D());
    Hooks.emplace_back(new Mouse());
    Hooks.emplace_back(new PacketSender());
    Hooks.emplace_back(new PacketHandler());
    Hooks.emplace_back(new SplashTextRenderer());
    Hooks.emplace_back(new ActorRenderDispatcher());

    Hooks.emplace_back(new ItemUseSlowdownSystem());
    Hooks.emplace_back(new RenderItemInHandDescriptionHook());
    Hooks.emplace_back(new BaseTick());
    Hooks.emplace_back(new ActorIsInvisible());
    Hooks.emplace_back(new SwingDurationHook());
    Hooks.emplace_back(new ActorSetNametag());
    Hooks.emplace_back(new ActorSwing());
    Hooks.emplace_back(new GamemodeAttack());
    Hooks.emplace_back(new GamemodeBuildBlock());
    Hooks.emplace_back(new GamemodeGetPickRange());
    Hooks.emplace_back(new LevelRendererPlayerHooks());
    Hooks.emplace_back(new OverworldDimensionHooks());
    Hooks.emplace_back(new RakPeerHooks());
    /*Hooks.emplace_back(new BlockSourceGetBlock());
    Hooks.emplace_back(new NoRenderHooks());
    */

    for (Hook* hook : Hooks) {
        if (hook->InitOnConstruct && hook->Name != "UIRenderCtx")
        {
            Logger::Write("HookManager", "Initializing: " + hook->Name, Logger::Debug);
            hook->Init();
        }
    }

    Logger::Write("HookManager", "Initialized " + std::to_string(Hooks.size()) + " hooks in " + std::to_string(Misc::GetCurrentMs() - start) + "ms", Logger::Debug);
}

Hook* HookManager::GetHookByName(std::string name) {
    for (Hook* hook : Hooks) {
        if (hook->Name == name)
            return hook;
    }
    return nullptr;
}

void HookManager::Shutdown() {

    Logger::Write("HookManager", "Unhooking all hooks...");

    for (Hook* hook : Hooks) {
        Logger::Write("Hook Manager", "Unhooked, " + hook->Name, Logger::Debug);
        hook->UnHook();
    }
}