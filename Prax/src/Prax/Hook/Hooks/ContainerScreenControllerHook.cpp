//
// Created by Flash on 9/5/2023.
//

#include "ContainerScreenControllerHook.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Module/Modules/Player/ChestStealer.h>
#include <Prax/Module/ModuleManager.h>

std::unique_ptr<Detour> TickDetour = nullptr;




uint32_t __fastcall Tick(ContainerScreenController* _this) {
    auto oFunc = TickDetour->GetFastcall<uint32_t, ContainerScreenController*>();
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!ModuleManager::initialized) {
        return oFunc(_this);
    }

    ContainerManagerModel* containerManagerModel = player->containerManagerModel;

    const std::string screenName = Minecraft::ClientInstance->GetScreenName();

    if (_this == nullptr) {
        return 0;
    }

    if (containerManagerModel == nullptr) {
        return 0;
    }



    if (containerManagerModel->getContainerType() != ContainerType::Container) return oFunc(_this);

    // Screen is a container screen
    static auto stealerMod = ModuleManager::GetModule<ChestStealer>();

    if (stealerMod->Enabled) stealerMod->OnContainerTick(_this);

    return oFunc(_this);
}

void ContainerScreenControllerHook::Init() {

    auto address = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ContainerScreenController_tick));

    TickDetour = std::make_unique<Detour>(xorstr_("ContainerScreenController::Tick"), address, Tick);
    TickDetour->Enable();
}

void ContainerScreenControllerHook::UnHook() {
    TickDetour->Restore();
}

void ContainerScreenController::handleAutoPlace(const std::string& name, int slot) {
    static uintptr_t func = Memory::ResolveRef(SigManager::GetSignatureAddress(SignatureEnum::ContainerScreenController_handleAutoPlace));
    // params: this, uint64_t, std::string, int

    using ContainerScreenController_autoPlace = void(__fastcall*)(ContainerScreenController*, uintptr_t, std::string, int);
    static auto autoPlaceFunc = reinterpret_cast<ContainerScreenController_autoPlace>(func);
    autoPlaceFunc(this, 0x7FFFFFFF, name, slot);
}

/*ItemStack *ContainerScreenController::getItemStack(const std::string& name, int slot) {
    return Memory::CallVFunc<64, ItemStack*>(this, name, slot);
}*/

void *ContainerScreenController::_tryExit() {
    // index: 12. use Memory::CallVFunc
    return Memory::CallVFunc<12, void*>(this);
}
