//
// Created by alteik on 04/09/2024.
//

#include "AirJump.hpp"
#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

void AirJump::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AirJump::onBaseTickEvent>(this);
}

void AirJump::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AirJump::onBaseTickEvent>(this);
}

bool oldSpacePressed = false;

void AirJump::onBaseTickEvent(class BaseTickEvent &event) {

    auto player = event.mActor;

    if (!player->isOnGround()) {
        bool spacePressed = player->getMoveInputComponent()->mIsJumping;
        if (spacePressed && !oldSpacePressed) {
            player->setOnGround(true);
            player->jumpFromGround();
            player->setOnGround(false);
        }

        oldSpacePressed = spacePressed;
    }
}