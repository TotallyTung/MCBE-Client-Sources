//
// Created by vastrakai on 12/31/2023.
//

#include "DamageTest.h"

#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Movement/DamageFly.h>

void DamageTest::OnRun(std::vector<std::string> args)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    static auto damageFly = ModuleManager::GetModule<DamageFly>();
    if (!damageFly) return;

    DamageFly::LastDamageTime = Misc::GetCurrentMs() + (uintptr_t)damageFly->FlyMs.Value;

    player->addChatMessage("DamageFlying for " + std::to_string(damageFly->FlyMs.Value + 1000) + "ms", 0);
}
