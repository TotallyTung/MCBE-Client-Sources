//
// Created by vastrakai on 9/24/2023.
//

#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Misc/StreamerMode.h>
#include <Prax/Utils/String.h>
#include "ActorSetNametag.h"

std::unique_ptr<Detour> actorSetNametagDetour;

void setNametagDetourFunc(Actor* actor, std::string const& name) {
    auto oFunc = actorSetNametagDetour.get()->GetFastcall<void, Actor*, std::string const&>();
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto streamerModeModule = ModuleManager::GetModule<StreamerMode>();
    if (streamerModeModule && streamerModeModule->Enabled) {
        oFunc(actor, String::ReplaceAll(name, StreamerMode::nameToReplace.c_str(), StreamerMode::NewName.c_str()));
    } else {
        oFunc(actor, name);
    }
}

void ActorSetNametag::Init() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    uintptr_t func = SigManager::GetSignatureAddress(SignatureEnum::Actor_setNameTag);

    actorSetNametagDetour = std::make_unique<Detour>("SetNametagDetour", func, &setNametagDetourFunc);
    actorSetNametagDetour->Enable();
    this->Detours.emplace_back(actorSetNametagDetour.get());
}

void ActorSetNametag::UnHook() {

}
