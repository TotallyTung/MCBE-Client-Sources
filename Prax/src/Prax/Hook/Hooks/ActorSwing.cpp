//
// Created by vastrakai on 10/13/2023.
//

#include <Prax/Module/Modules/Visual/Swing.h>
#include "ActorSwing.h"
#include <Prax/Module/ModuleManager.h>
#include <Includes/magic_vtable/include/magic_vtable.hpp>

std::unique_ptr<Detour> ActorSwingDetour = nullptr;
Swing* swMod = nullptr;

void actorSwingDetourFunc(void* _this){
    auto oFunc = ActorSwingDetour->GetFastcall<void, void*>();
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player || _this != player) return oFunc(_this);

    bool cancel = false;

    if (!swMod) {
        swMod = ModuleManager::GetModule<Swing>();
        return oFunc(_this);
    }
    if (swMod->Enabled) {
        swMod->OnActorSwing(&cancel);
    }

    if (!cancel) return oFunc(_this);
}

void ActorSwing::Init() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    auto vtable = player->vTable();

    // swing index is
    auto swingFunc = vtable[magic_vft::vtable_index<&Actor::swing>()];
    ActorSwingDetour = std::make_unique<Detour>("Actor::swing", swingFunc, &actorSwingDetourFunc);
    ActorSwingDetour->Enable();
    this->Detours.emplace_back(ActorSwingDetour.get());

}

void ActorSwing::UnHook() {

}
