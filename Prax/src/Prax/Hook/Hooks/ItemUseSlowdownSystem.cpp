//
// Created by vastrakai on 10/13/2023.
//

#include <Prax/SDK/SigManager.h>
#include <Prax/Module/Modules/Movement/NoSlowDown.h>
#include <Prax/Module/ModuleManager.h>
#include "ItemUseSlowdownSystem.h"

// ItemUseSlowdownSystem::isSlowedByItemUse

std::unique_ptr<Detour> isSlowedByItemUseDetour = nullptr;



void* isSlowedByItemUseDetourFunc(void* a1, void* a2, void* a3) {
    auto oFunc = isSlowedByItemUseDetour->GetFastcall<void*, void*, void*, void*>();
    static auto slowMod = ModuleManager::GetModule<NoSlowDown>();

    if (slowMod->Enabled) {
        return nullptr;
    }

    return oFunc(a1, a2, a3);
}

void ItemUseSlowdownSystem::Init() {
    uintptr_t func = SigManager::GetSignatureAddress(SignatureEnum::ItemUseSlowdownSystem_isSlowedByItemUse);
    if (!func)
    {
        Logger::Write("ItemUseSlowdownSystem::Init: Failed to find signature!");
    }

    isSlowedByItemUseDetour = std::make_unique<Detour>("ItemUseSlowdownSystem::isSlowedByItemUse", func, &isSlowedByItemUseDetourFunc);
    isSlowedByItemUseDetour->Enable();
    this->Detours.emplace_back(isSlowedByItemUseDetour.get());
}

void ItemUseSlowdownSystem::UnHook() {}
