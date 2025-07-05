//
// Created by vastrakai on 8/10/2023.
//

#include "BaseTick.h"
#include "src/Prax/SDK/Game/Minecraft.h"
#include <memory>
#include <Includes/magic_vtable/include/magic_vtable.hpp>
#include <Prax/Module/Modules/Misc/TestModule.h>
#include <Prax/Module/Modules/World/Scaffold.h>
#include <Prax/Module/Modules/World/Timer.h>
#include "Prax/Module/ModuleManager.h"

std::unique_ptr<Detour> baseTickDetour = nullptr;

void BaseTickDetourFunc(void* _this)
{
    auto oFunc = baseTickDetour.get()->GetFastcall<void, void*>();
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player || !ModuleManager::initialized)
    {
        return oFunc(_this);
    }

    if (player != _this) return oFunc(_this);


    // TODO: Unfuck this
    static std::vector<std::string> calledSeparately = {
            "Timer",
        "Scaffold",
    };
    static auto timer = ModuleManager::GetModule<Timer>();
    static auto scaffold = ModuleManager::GetModule<Scaffold>();

    for (auto & module : ModuleManager::Modules) {
        if (module->Name == "ClickGui" && !module->Enabled) Minecraft::ClientInstance->DisableInput(false); // ??

        if (!module->TickOnDisabled && module->Enabled && std::ranges::find(calledSeparately, module->Name) == calledSeparately.end())
            module->OnTick();

        if (module->TickOnDisabled) module->OnTick();

    }

    if ( timer->Enabled) {
        timer->OnTick();
    }
    if (scaffold->Enabled) {
        scaffold->OnTick();
    }

    return oFunc(_this);
}

void BaseTick::Init() {
    if (!Minecraft::ClientInstance->ThePlayer()) return;
    auto vtable = Minecraft::ClientInstance->ThePlayer()->vTable();
    baseTickDetour = std::make_unique<Detour>("BaseTick", vtable[magic_vft::vtable_index<&Actor::baseTick>()], &BaseTickDetourFunc);
    baseTickDetour->Enable();

    this->Detours.emplace_back(baseTickDetour.get());
}

void BaseTick::UnHook() {

}
