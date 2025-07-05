#pragma once

#include <Prax/Module/Modules/Visual/ArrayList.h>

#include "ModuleManager.h"
#include "Modules/World/Timer.h"

#include "src/Prax/Module/Modules/Misc/Disabler.h"
#include "src/Prax/Module/Modules/Misc/TestModule.h"

#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Module/Modules/Combat/Aura.h>
#include <Prax/Module/Modules/Combat/AutoBow.h>
#include <Prax/Module/Modules/Combat/AutoZap.h>
#include <Prax/Module/Modules/Combat/InfiniteAura.h>
#include <Prax/Module/Modules/Misc/AutoLootbox.h>
#include <Prax/Module/Modules/Misc/AutoQueue.h>
#include <Prax/Module/Modules/Misc/Desync.h>
#include <Prax/Module/Modules/Misc/DeviceIDSpoof.h>
#include <Prax/Module/Modules/Misc/KickCodes.h>
#include <Prax/Module/Modules/Misc/LeaveNotifs.h>
#include <Prax/Module/Modules/Misc/StreamerMode.h>
#include <Prax/Module/Modules/Movement/AirJump.h>
#include <Prax/Module/Modules/Movement/AntiImmobile.h>
#include <Prax/Module/Modules/Movement/AutoSprint.h>
#include <Prax/Module/Modules/Movement/DamageBoost.h>
#include <Prax/Module/Modules/Movement/FastStop.h>
#include <Prax/Module/Modules/Movement/Fly.h>
#include <Prax/Module/Modules/Movement/LongJump.h>
#include <Prax/Module/Modules/Movement/NoJumpDelay.h>
#include <Prax/Module/Modules/Movement/NoLagBack.h>
#include <Prax/Module/Modules/Movement/NoSlowdown.h>
#include <Prax/Module/Modules/Movement/Phase.h>
#include <Prax/Module/Modules/Movement/Speed.h>
#include <Prax/Module/Modules/Movement/Spider.h>
#include <Prax/Module/Modules/Movement/Velocity.h>
#include <Prax/Module/Modules/Player/AntiVoid.h>
#include <Prax/Module/Modules/Player/AutoSpellbook.h>
#include <Prax/Module/Modules/Player/AutoTool.h>
#include <Prax/Module/Modules/Player/ChestStealer.h>
#include <Prax/Module/Modules/Player/Derp.h>
#include <Prax/Module/Modules/Player/Freecam.h>
#include <Prax/Module/Modules/Player/Friends.h>
#include <Prax/Module/Modules/Player/InstaBreak.h>
#include <Prax/Module/Modules/Player/InventoryManager.h>
#include <Prax/Module/Modules/Player/MidclickPearl.h>
#include <Prax/Module/Modules/Player/NoFall.h>
#include <Prax/Module/Modules/Player/Regen.h>
#include <Prax/Module/Modules/Player/Teams.h>
#include <Prax/Module/Modules/Visual/AntiInvis.h>
#include <Prax/Module/Modules/Visual/BPSDisplay.h>
#include <Prax/Module/Modules/Visual/ClickGui.h>
#include <Prax/Module/Modules/Visual/ESP.h>
#include <Prax/Module/Modules/Visual/HUD.h>
#include <Prax/Module/Modules/Visual/NoCameraClip.h>
#include <Prax/Module/Modules/Visual/OreESP.h>
#include <Prax/Module/Modules/Visual/PlaceHighlights.h>
#include <Prax/Module/Modules/Visual/ReachDisplay.h>
#include <Prax/Module/Modules/Visual/Console.h>
#include <Prax/Module/Modules/Visual/SelectedBlock.h>
#include <Prax/Module/Modules/Visual/SelfCentered.h>
#include <Prax/Module/Modules/Visual/ShowGracePeriod.h>
#include <Prax/Module/Modules/Visual/ShowNametag.h>
#include <Prax/Module/Modules/Visual/ShowRotations.h>
#include <Prax/Module/Modules/Visual/Swing.h>
#include <Prax/Module/Modules/Visual/TargetHUD.h>
#include <Prax/Module/Modules/Visual/Tracers.h>
#include <Prax/Module/Modules/Visual/Trail.h>
#include <Prax/Module/Modules/Visual/Watermark.h>
#include <Prax/Module/Modules/World/Ambience.h>
#include <Prax/Module/Modules/World/TimerSlider.h>
#include <Prax/Utils/String.h>
#include <src/Prax/Module/Modules/Misc/Killsults.h>
#include <src/Prax/Module/Modules/Misc/NetSkip.h>
#include <src/Prax/Module/Modules/Misc/ToggleSounds.h>
#include <src/Prax/Module/Modules/Misc/AutoSnipe.h>
#include <src/Prax/Module/Modules/World/Breaker.h>
#include <src/Prax/Module/Modules/World/NoPacket.h>
#include <src/Prax/Module/Modules/World/Scaffold.h>
#include <Prax/Module/Modules/Movement/FollowBot.h>
#include <Prax/Module/Modules/Misc/Anticheat.h>
#include <Prax/Module/Modules/Misc/OreMiner.h>
#include <Prax/Module/Modules/Visual/ItemESP.h>
#include <Prax/Module/Modules/Misc/AntiCheatDetector.h>
#include <Prax/Module/Modules/Visual/LevelTransitions.h>
#include <Prax/Module/Modules/Visual/ViewModel.h>
#include <Prax/Module/Modules/Visual/SmallItems.h>

#include "Modules/Combat/AimAssist.h"
#include "Modules/Combat/Reach.h"
#include "Modules/Combat/TriggerBot.h"
#include "Modules/Misc/AutoDefend.h"
#include "Modules/Misc/AutoDodge.h"
#include "Modules/Misc/AutoHvH.h"
#include "Modules/Misc/Franky.h"
#include "Modules/Misc/KickSounds.h"
#include "Modules/Misc/NoFilter.h"
#include "Modules/Misc/PacketLogger.h"
#include "Modules/Misc/Surround.h"
#include "Modules/Movement/AutoJump.h"
#include "Modules/Movement/DamageFly.h"
#include "Modules/Movement/InvMove.h"
#include "Modules/Movement/SafeWalk.h"
#include "Modules/Movement/ServerSneak.h"
#include "Modules/Movement/Step.h"
#include "Modules/Movement/TargetStrafe.h"
#include "Modules/Movement/TestFly.h"
#include "Modules/Player/AntiRegen.h"
#include "Modules/Player/AutoWeb.h"
#include "Modules/Player/ChestAura.h"
#include "Modules/Player/Extinguisher.h"
#include "Modules/Player/HealthAlert.h"
#include "Modules/Player/HvHRegen.h"
#include "Modules/Player/NoDebuff.h"
#include "Modules/Player/PacketMine.h"
#include "Modules/Player/ParticleSpam.h"
#include "Modules/Visual/AutoScale.h"
#include "Modules/Visual/ChinaHat.h"
#include "Modules/Visual/DamageEffects.h"
#include "Modules/Visual/FullBright.h"
#include "Modules/Visual/Glint.h"
#include "Modules/Visual/LevelInfo.h"
#include "Modules/Visual/Nametags.h"
#include "Modules/Visual/NoHurtCam.h"
#include "Modules/Visual/NoRender.h"
#include "Modules/Visual/NoScoreboard.h"
#include "Modules/Visual/SelectionColor.h"
#include "Modules/Visual/Statistics.h"
#include "Modules/Visual/Zoom.h"
#include "Modules/World/Nuker.h"




bool ModuleManager::initialized = false;

void ModuleManager::Init() {
    // TODO: Replace ADD_MODULE with REGISTER_MODULE in each module class
#ifndef __RELEASEFREE__
    ADD_MODULE(Franky);
    ADD_MODULE(TestModule);
    ADD_MODULE(AntiRegen);
    ADD_MODULE(ChestAura);
    ADD_MODULE(KickCodes);
    ADD_MODULE(Anticheat);
    ADD_MODULE(Surround);
#endif

// Debug only
#ifdef __DEBUG__
    ADD_MODULE(PacketLogger);
    ADD_MODULE(AimAssist);
    ADD_MODULE(ConsoleMod);
#endif

    ADD_MODULE(ToggleSounds);
    ADD_MODULE(KickSounds);
    ADD_MODULE(Disabler);
    ADD_MODULE(NetSkip);
    ADD_MODULE(AutoQueue);
    ADD_MODULE(Breaker);
    ADD_MODULE(AutoHvH);

// ADD_MODULE(IRC);
    ADD_MODULE(AutoLootbox);
    ADD_MODULE(StreamerMode);
    ADD_MODULE(DeviceIDSpoof);
    ADD_MODULE(LeaveNotifs);
    ADD_MODULE(Desync);
    ADD_MODULE(NoFilter);
    ADD_MODULE(AutoDefend);
    ADD_MODULE(AutoSnipe);
    ADD_MODULE(AntiCheatDetector);
    ADD_MODULE(OreMiner);
    ADD_MODULE(AutoDodge);

// Visual
    ADD_MODULE(Tracers);
    ADD_MODULE(ArrayList);
    ADD_MODULE(ClickGuiMod);
    ADD_MODULE(ESP);
    ADD_MODULE(HUD);
    ADD_MODULE(OreESP);
    ADD_MODULE(SelectedBlock);
    ADD_MODULE(ShowRotations);
    ADD_MODULE(ShowNametag);
    ADD_MODULE(NoCameraClip);
    ADD_MODULE(Watermark);
    ADD_MODULE(Trail);
    ADD_MODULE(Swing);
    ADD_MODULE(SelfCentered);
    ADD_MODULE(AntiInvis);
    ADD_MODULE(PlaceHighlights);
    ADD_MODULE(TargetHUD);
    ADD_MODULE(ShowGracePeriod);
    ADD_MODULE(Glint);
    ADD_MODULE(Zoom);
    ADD_MODULE(FullBright);
    ADD_MODULE(NoHurtCam);
    ADD_MODULE(Nametags);
    ADD_MODULE(ChinaHat);
    ADD_MODULE(AutoScale);
    ADD_MODULE(LevelInfo);
    ADD_MODULE(NoRender);
    ADD_MODULE(SelectionColor);
    ADD_MODULE(ItemESP);
    ADD_MODULE(LevelTransitions);
    ADD_MODULE(ViewModel);
    ADD_MODULE(SmallItems);
    ADD_MODULE(DamageEffects);
    ADD_MODULE(Statistics);
    ADD_MODULE(NoScoreboard);

// World
    ADD_MODULE(Timer);
    ADD_MODULE(TimerSlider);
    ADD_MODULE(NoPacket);
    ADD_MODULE(Ambience);
    ADD_MODULE(Nuker);

// Movement
    ADD_MODULE(TestFly);
    ADD_MODULE(Speed);
    ADD_MODULE(Fly);
    ADD_MODULE(Velocity);
    ADD_MODULE(AutoSprint);
    ADD_MODULE(AirJump);
    ADD_MODULE(Spider);
    ADD_MODULE(FastStop);
    ADD_MODULE(LongJump);
    ADD_MODULE(NoLagBack);
    ADD_MODULE(DamageBoost);
    ADD_MODULE(ServerSneak);
    ADD_MODULE(NoSlowDown);
    ADD_MODULE(Phase);
    ADD_MODULE(AntiImmobile);
    ADD_MODULE(DamageFly);
    ADD_MODULE(NoJumpDelay);
    ADD_MODULE(SafeWalk);
    ADD_MODULE(Step);
    ADD_MODULE(InvMove);
    ADD_MODULE(FollowBot);
    ADD_MODULE(TargetStrafe);
    ADD_MODULE(AutoJump);

// Player
    ADD_MODULE(Scaffold);
    ADD_MODULE(InventoryManager);
    ADD_MODULE(AutoTool);
    ADD_MODULE(Killsults);
    ADD_MODULE(Friends);
    ADD_MODULE(InstaBreak);
    ADD_MODULE(Teams);
    ADD_MODULE(Derp);
    ADD_MODULE(NoFall);
    ADD_MODULE(AutoSpellbook);
    ADD_MODULE(ChestStealer);
    ADD_MODULE(Freecam);
    ADD_MODULE(AntiVoid);
    ADD_MODULE(MidclickPearl);
    ADD_MODULE(Extinguisher);
    ADD_MODULE(PacketMine);
    ADD_MODULE(Regen);
    ADD_MODULE(HvHRegen);
    ADD_MODULE(HealthAlert);
    ADD_MODULE(AutoWeb);
    ADD_MODULE(NoDebuff);

// Combat
    ADD_MODULE(AutoBow);
    ADD_MODULE(Aura);
    ADD_MODULE(InfiniteAura);
    ADD_MODULE(Antibot);
    ADD_MODULE(AutoZap);
    ADD_MODULE(Reach);
    ADD_MODULE(TriggerBot);



    for (auto module : Modules) {
        module->OnInitialize();
    };

    initialized = true;
}

void ModuleManager::Shutdown() {

    for (auto module : Modules) {
        module->OnEject();
    };


    for (auto module : Modules) {
        if (module->Enabled) {
            module->Toggle();
        }
    }

    initialized = false;
}

Module* ModuleManager::FindModuleByName(std::string name) {
    for (auto module : Modules) {
        if (String::ToLower(module->Name) == String::ToLower(name)) {
            return module;
        }
    }
    return nullptr;
}

void ModuleManager::EnableModule(const std::string &name) {
    auto mod = FindModuleByName(name);
    if (mod && !mod->Enabled) {
        mod->Toggle();
    }
}

void ModuleManager::DisableModule(const std::string &name) {
    auto mod = FindModuleByName(name);
    if (mod && mod->Enabled) {
        mod->Toggle();
    }
}

void ModuleManager::ToggleModule(const std::string &name) {
    auto mod = FindModuleByName(name);
    if (mod) {
        mod->Toggle();
    }
}
