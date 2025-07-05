//
// Created by vastrakai on 9/28/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/PlaceHighlights.h>
#include "GamemodeBuildBlock.h"
#include <Prax/SDK/Game/Entity/GameMode.h>
#include <Includes/magic_vtable/include/magic_vtable.hpp>

std::unique_ptr<Detour> GamemodeBuildBlockDetour = nullptr;
// BlockPos const&, unsigned char blockSide, bool shouldCrash);
void* buildBlockDetourFunc(void* _this, BlockPos const& pos, int blockSide, bool shouldCrash) {
    auto oFunc = GamemodeBuildBlockDetour->GetFastcall<void*, void*, BlockPos const&, int, bool>();
    auto result = oFunc(_this, pos, blockSide, shouldCrash);
    auto mod = ModuleManager::GetModule<PlaceHighlights>();



    if (!mod || !mod->Enabled) return result;

    if (Misc::IsAirBlock(pos - Misc::GetSide(blockSide))) return result;
    PlaceHighlights::LastPlaceMap[Misc::GetCurrentMs()] = (pos - Misc::GetSide(blockSide));
    return result;
}

void GamemodeBuildBlock::Init() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    auto vtable = Minecraft::ClientInstance->ThePlayer()->gamemode->vTable;

    if (!vtable) {
        Logger::Write("GamemodeBuildBlock", "Failed to get vtable", Logger::LogType::Error);
        return;
    }

    GamemodeBuildBlockDetour = std::make_unique<Detour>("GameMode::buildBlock", vtable[magic_vft::vtable_index<&GameMode::buildBlock>()], &buildBlockDetourFunc);
    GamemodeBuildBlockDetour->Enable();
    this->Detours.emplace_back(GamemodeBuildBlockDetour.get());
}

void GamemodeBuildBlock::UnHook() {

}
