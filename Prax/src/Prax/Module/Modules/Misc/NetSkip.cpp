//
// Created by vastrakai on 9/1/2023.
//

#include <map>
#include "NetSkip.h"
#include <Prax/Utils/String.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/World/Scaffold.h>

#include "Prax/Hook/Hooks/RakPeerHooks.h"

void NetSkip::OnDisable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    RakPeerHooks::TicksToSkip = 0;
    RakPeerHooks::MsToSkip = 0;
    RakPeerHooks::Ticking = false;
    RakPeerHooks::RenderCurrentAndLastPos = false;
}

void NetSkip::OnEnable() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    RakPeerHooks::LastPos = *Minecraft::ClientInstance->ThePlayer()->getPos();

    if (WeakEnable.Enabled) {
        if (Mode.SelectedIndex == 0) RakPeerHooks::TicksToSkip = static_cast<int>(Ticks.Value);
        else RakPeerHooks::MsToSkip = static_cast<int>(Delay.Value);
        RakPeerHooks::Ticking = true;
        RakPeerHooks::Visualize = VisualizeSetting.Enabled;
        RakPeerHooks::RenderCurrentAndLastPos = RenderCurrentAndLastPosSetting.Enabled;
    }
}

void NetSkip::OnTick() {
    static auto scaffold = ModuleManager::GetModule<Scaffold>();

    if (WeakEnable.Enabled) {
        return;
    }

    if (DisableWhenScaffolding.Enabled && scaffold && scaffold->Enabled) {
        if (RakPeerHooks::Ticking) {
            RakPeerHooks::TicksToSkip = 0;
            RakPeerHooks::MsToSkip = 0;
            RakPeerHooks::Ticking = false;
            RakPeerHooks::RenderCurrentAndLastPos = false;
        }
        return;
    }

    if (Mode.SelectedIndex == 0) RakPeerHooks::TicksToSkip = (int)Ticks.Value;
    else RakPeerHooks::MsToSkip = (int)Delay.Value;
    RakPeerHooks::Ticking = true;
    RakPeerHooks::Visualize = VisualizeSetting.Enabled;
    RakPeerHooks::RenderCurrentAndLastPos = RenderCurrentAndLastPosSetting.Enabled;
}