//
// Created by vastrakai on 11/12/2023.
//

#include "GamemodeAttack.h"

#include <Includes/magic_vtable/include/magic_vtable.hpp>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/ReachDisplay.h>
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Entity/GameMode.h>

std::unique_ptr<Detour> GamemodeAttackDetour = nullptr;

void* attackDetourFunc(void* _this, Actor* actor) {
    auto oFunc = GamemodeAttackDetour->GetFastcall<void*, void*, Actor*>();
    auto result = oFunc(_this, actor);



    return result;
}

void GamemodeAttack::Init()
{
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    auto vtable = Minecraft::ClientInstance->ThePlayer()->gamemode->vTable;
    if (!vtable)
    {
        Logger::Write("GamemodeAttack", "Failed to get vtable", Logger::LogType::Error);
        return;
    }
    GamemodeAttackDetour = std::make_unique<Detour>("GameMode::attack", vtable[magic_vft::vtable_index<&GameMode::attack>()], &attackDetourFunc);
    GamemodeAttackDetour->Enable();
    this->Detours.emplace_back(GamemodeAttackDetour.get());
}

void GamemodeAttack::UnHook()
{

}
