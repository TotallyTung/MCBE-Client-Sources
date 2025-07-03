//
// Created by alteik on 02/09/2024.
//

#include "HiveFly.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/Network/Packets/SetActorMotionPacket.hpp>

void HiveFly::Reset()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;
    mCanFly = false;
    mVeloTick = 1;
    mTicksToStay = 0;
    mShouldStay = false;
    player->setStatusFlag(ActorFlags::Noai, false);
    ClientInstance::get()->getMinecraftSim()->setSimTimer(20.f);
}

void HiveFly::onEnable()
{
    Reset();
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &HiveFly::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &HiveFly::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &HiveFly::onPacketInEvent>(this);
}

void HiveFly::onDisable()
{
    Reset();
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &HiveFly::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &HiveFly::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &HiveFly::onPacketInEvent>(this);
}

void HiveFly::onBaseTickEvent(class BaseTickEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if(!mCanFly && mShouldStay && mTicksToStay)
    {
        mTicksToStay--;
        ChatUtils::displayClientMessage("bipass x" +  std::to_string(mTicksToStay));
        player->setStatusFlag(ActorFlags::Noai, true);
    }
    else if(!mCanFly && mShouldStay && !mTicksToStay)
    {
        Reset();
        ChatUtils::displayClientMessage("successful!");
        HiveFly::setEnabled(false);
    }

    if(mTimer.mValue > 0 && mCanFly)
    {
        ClientInstance::get()->getMinecraftSim()->setSimTimer(mTimer.mValue);
    }

    if(mCanFly && !mShouldStay) {
        glm::vec3 motion = glm::vec3(0, 0, 0);

        if (Keyboard::isUsingMoveKeys(true)) {
            glm::vec2 calc = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, mSpeed.mValue / 10);
            motion.x = calc.x;
            motion.z = calc.y;
            motion.y -= 0.1;
        }

        player->getStateVectorComponent()->mVelocity = motion;
    }
}

void HiveFly::onPacketOutEvent(class PacketOutEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if(mCanFly && event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        switch (mVeloTick)
        {
            case 1:
                paip->mPosDelta.y = 0.300000;
                ChatUtils::displayClientMessage("Gaming x1");
                mVeloTick++;
                break;
            case 2:
                paip->mPosDelta.y = 0.215600;
                ChatUtils::displayClientMessage("Gaming x2");
                mVeloTick++;
                break;
            case 3:
                paip->mPosDelta.y = 0.132888;
                ChatUtils::displayClientMessage("Gaming x3");
                mVeloTick++;
                break;
            case 4:
                paip->mPosDelta.y = 0.051830;
                ChatUtils::displayClientMessage("Gaming x4");
                mVeloTick++;
                mShouldStay = true;
                mTicksToStay = 2;
                break;
            case 5:
                paip->mPosDelta.y = -0.027606;
                ChatUtils::displayClientMessage("Gaming x5");
                mVeloTick = 1;
                ChatUtils::displayClientMessage("Reset!");
                mCanFly = false;
                break;
            default:
                break;
        }
    }
}

void HiveFly::onPacketInEvent(class PacketInEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if(event.mPacket->getId() == PacketID::SetActorMotion)
    {
        auto sam = event.getPacket<SetActorMotionPacket>();
        if (sam->mRuntimeID == player->getRuntimeID()) {
            mCanFly = true;
            ChatUtils::displayClientMessage("Damage taken!");
        }
    }
}