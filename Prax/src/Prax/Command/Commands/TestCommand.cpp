//
// Created by Flash on 7/30/2023.
//

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/String.h>
#include "TestCommand.h"

#include <iostream>
#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/SDK/Game/Entity/Components/FlagComponents.h>

#include "Prax/SDK/Game/Entity/Components/DepenetrationComponent.h"
#include "Prax/SDK/Game/Entity/Components/MobHurtTimeComponent.h"

int lmfao = 0;

void testFunc() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    auto* hurtTimeComponent = player->entityCtx.getComponent<MobHurtTimeComponent>();

    if (hurtTimeComponent) {
        player->addChatMessage("&aFound MobHurtTimeComponent: &b" + String::ToHex((uint64_t)hurtTimeComponent) + "&a(Copied to clipboard)", true);
        Misc::SetClipboard(String::ToHex((uint64_t)hurtTimeComponent));
    } else
    {
        player->addChatMessage("&cMobHurtTimeComponent not found", true);
    }
}

void TestCommand::OnRun(std::vector<std::string> args) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    Logger::Write(__FUNCTION__, Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->serverIp);
    Logger::Write(__FUNCTION__, Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->realIp);

    auto supplies = player->supplies;

    auto item1 = supplies->inventory->getItem(0);
    auto item2 = supplies->inventory->getItem(1);

    if (!item1->item) return;
    if (!item2->item) return;

    item2->item = item1->item;
}


