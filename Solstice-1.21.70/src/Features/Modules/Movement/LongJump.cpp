//
// Created by vastrakai on 8/4/2024.
//

#include "LongJump.hpp"

#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void LongJump::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &LongJump::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &LongJump::onPacketOutEvent>(this);
}

void LongJump::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &LongJump::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &LongJump::onPacketOutEvent>(this);
    ClientInstance::get()->getMinecraftSim()->setSimTimer(20);

    if (mDisableModules.mValue)
    {
        for (const auto& mod : gFeatureManager->mModuleManager->getModules())
        {
            if (mod.get() == this) continue;
            if (mod->mEnabled && mod->mKey != 0 && mod->mKey == mKey) mod->setEnabled(false);
        }
    }

    mHasJumped = false;
}

void LongJump::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;

    glm::vec2 motion = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, mSpeed.mValue / 10);
    glm::vec3 vel = glm::vec3(motion.x, mHeight.mValue, motion.y);

    if (mTimerBoost.mValue) ClientInstance::get()->getMinecraftSim()->setSimTimer(mTimer.mValue);

    if (player->isOnGround() && !mHasJumped)
    {
        player->getStateVectorComponent()->mVelocity = vel;
        mHasJumped = true;
    }

    if (player->isOnGround() && !player->wasOnGround()) setEnabled(false);
}

void LongJump::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput && mApplyJumpFlags.mValue)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        auto paip = event.getPacket<PlayerAuthInputPacket>();

        if (Keyboard::isUsingMoveKeys()) paip->mInputData |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN;
        if (!player->isOnGround() && player->wasOnGround() && Keyboard::isUsingMoveKeys()) {
            paip->mInputData |= AuthInputAction::START_JUMPING;
        }
    }
}
