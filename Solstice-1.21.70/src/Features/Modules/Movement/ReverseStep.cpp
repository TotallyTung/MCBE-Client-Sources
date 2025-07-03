//
// Created by alteik on 09/10/2024.
//

#include "ReverseStep.hpp"

#include "Speed.hpp"
#include "LongJump.hpp"
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/StateVectorComponent.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>

bool ReverseStep::canFallDown() {
    auto pos = *ClientInstance::get()->getLocalPlayer()->getPos();

    for (int checkOffsetY = 1; checkOffsetY <= (mMaxFallDistance.mValue + 1); ++checkOffsetY) {
        glm::vec3 blockPos = {pos.x, pos.y - (float)checkOffsetY, pos.z};
        if (!BlockUtils::isAirBlock(blockPos)) {
            return true;
        }
    }

    return false;
}

bool ReverseStep::isVoid() {
    auto pos = *ClientInstance::get()->getLocalPlayer()->getPos();

    for (int checkOffsetY = 1; checkOffsetY <= 255; ++checkOffsetY) {
        glm::vec3 blockPos = {pos.x, pos.y - (float)checkOffsetY, pos.z};
        Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
        if (block->mLegacy->mSolid) {
            return false;
        }
    }

    return true;
}

void ReverseStep::onEnable() {
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &ReverseStep::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &ReverseStep::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
}

void ReverseStep::onDisable() {
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &ReverseStep::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &ReverseStep::onPacketOutEvent>(this);
}

void ReverseStep::onBaseTickEvent(BaseTickEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto speed = gFeatureManager->mModuleManager->getModule<Speed>();
    auto longJump = gFeatureManager->mModuleManager->getModule<LongJump>();

    if(!canFallDown()) return;
    if(mVoidCheck.mValue && isVoid()) return;
    if(mDontUseIfSpeed.mValue && speed->mEnabled) return;
    if(mDontUseIfLongJump.mValue && longJump->mEnabled) return;
    if(Keyboard::isUsingMoveKeys(true) && player->getMoveInputComponent()->mIsJumping || !player->wasOnGround() || mJumped)return;


    if (!player->isOnGround())
        player->getStateVectorComponent()->mVelocity.y -= 50;
}

void ReverseStep::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        mJumped = paip->hasInputData(AuthInputAction::START_JUMPING);
    }
}
