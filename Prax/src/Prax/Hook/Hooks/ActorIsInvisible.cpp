//
// Created by vastrakai on 9/25/2023.
//

#include <Prax/Module/ModuleManager.h>
#include "ActorIsInvisible.h"

#include <Includes/magic_vtable/include/magic_vtable.hpp>
#include <Prax/Module/Modules/Visual/AntiInvis.h>

std::unique_ptr<Detour> ActorIsInvisibleDetour = nullptr;

bool isInvisDetour(void* _this) {
    auto oFunc = ActorIsInvisibleDetour->GetFastcall<bool, void*>();
    static AntiInvis* antiInvis = nullptr;
    if (!antiInvis) {
        antiInvis = ModuleManager::GetModule<AntiInvis>();
    }
    if (!antiInvis->Enabled) return oFunc(_this);

    return false;
};

void ActorIsInvisible::Init() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    auto vtable = Minecraft::ClientInstance->ThePlayer()->vTable();

    ActorIsInvisibleDetour = std::make_unique<Detour>("ActorIsInvisible", vtable[magic_vft::vtable_index<&Actor::isInvisible>()], &isInvisDetour);
    ActorIsInvisibleDetour->Enable();
    this->Detours.emplace_back(ActorIsInvisibleDetour.get());
}

void ActorIsInvisible::UnHook() {

}
