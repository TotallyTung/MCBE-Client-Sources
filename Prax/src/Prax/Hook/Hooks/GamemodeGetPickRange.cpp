//
// Created by vastrakai on 1/9/2024.
//

#include "GamemodeGetPickRange.h"

#include <Includes/magic_vtable/include/magic_vtable.hpp>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Combat/Reach.h>
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Entity/GameMode.h>

std::unique_ptr<Detour> GamemodeGetPickRange::detour;

float GamemodeGetPickRange::detourFunc(void* _this, void* inputMode, bool a3)
{
    auto oFunc = detour->GetFastcall<float, void*, void*, bool>();

    auto result = oFunc(_this, inputMode, a3);

    if (!ModuleManager::initialized) return result;



    static auto mod = ModuleManager::GetModule<Reach>();

    if (mod->Enabled && mod->BlockReachEnabled.Enabled)
    {
        return mod->BlockReach.Value;
    }

    return result;
}

void GamemodeGetPickRange::Init()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    auto vtable = player->gamemode->vTable;

    if (!vtable)
    {
        Logger::Write("GamemodeGetPickRange", "Failed to get vtable!", Logger::LogType::Error);
        return;
    }

    detour = std::make_unique<Detour>("GameMode::getPickRange", vtable[magic_vft::vtable_index<&GameMode::getPickRange>()], &detourFunc); // 10 is the index of the function in the vtable
    detour->Enable();
    this->Detours.emplace_back(detour.get());


}

void GamemodeGetPickRange::UnHook()
{

}
