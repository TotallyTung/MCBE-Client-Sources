//
// Created by vastrakai on 7/22/2024.
//

#include "Derp.hpp"

#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void Derp::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Derp::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Derp::onPacketOutEvent, nes::event_priority::FIRST>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Derp::onPacketOutEvent2, nes::event_priority::VERY_LAST>(this);
}

void Derp::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Derp::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Derp::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Derp::onPacketOutEvent2>(this);
}

void Derp::onBaseTickEvent(BaseTickEvent& event)
{

}

void Derp::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mMode.mValue == Mode::Spin)
        {
            auto player = ClientInstance::get()->getLocalPlayer();
            if (player->isDestroying() || (mOffGroundOnly.mValue && player->isOnGround() && player->wasOnGround())) return;

            int64_t now = NOW;
            float maxYaw = 179.9f;
            float minYaw = -179.9f;
            // Determine the yaw from (-180, 180) based on the current time and mSpeed
            const auto yaw = (float)(static_cast<uint64_t>(static_cast<double>(NOW) * mSpeed.as<double>()) % static_cast<int>(maxYaw - minYaw) + minYaw);
            if (!mHeadOnly.mValue) {
                paip->mRot.y = yaw;
                paip->mRot.x = 89.99f; // Max look down angle
            }
            paip->mYHeadRot = yaw;
        } else if (mMode.mValue == Mode::Random)
        {
            // pitch: -90 to 90, yaw: -180 to 180
            paip->mRot.x = (float)(rand() % 180 - 90);
            paip->mRot.y = (float)(rand() % 360 - 180);
            paip->mVehicleRotation = paip->mRot;
            paip->mYHeadRot = paip->mRot.y;
        } else if (mMode.mValue == Mode::Headroll)
        {
            // just like spin but only the head
            auto player = ClientInstance::get()->getLocalPlayer();
            if (player->isDestroying()) return;

            int64_t now = NOW;
            float maxPitch = -179.9f;
            float minPitch = 179.9f;
            // Determine the pitch from (-90, 90) based on the current time and mSpeed
            const auto pitch = (float)(static_cast<uint64_t>(static_cast<double>(NOW) * mSpeed.as<double>()) % static_cast<int>(maxPitch - minPitch) + minPitch);
            paip->mRot.x = pitch;
        }
    }
}
// Used for Head Yaw Desync, gets called after most other packet events
void Derp::onPacketOutEvent2(PacketOutEvent& event)
{
    static int tick = 0;
    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mHeadYawDesync)
        {
            static bool direction = false; // true = +90, false = -90
            paip->mYHeadRot = paip->mRot.y;
            if (mHeadYawFlip.mValue)
            {
                if (tick % mFlipTick.as<int>() == 0) direction = !direction;
                if (direction) paip->mYHeadRot += 90.0f;
                else paip->mYHeadRot -= 90.0f;
                tick++;
            } else paip->mYHeadRot += 90.0f;
        }
    } else if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto mpp = event.getPacket<MovePlayerPacket>();
        if (mHeadYawDesync)
        {
            static bool direction = false; // true = +90, false = -90
            mpp->mYHeadRot = mpp->mRot.y;
            if (mHeadYawFlip.mValue)
            {
                if (tick % mFlipTick.as<int>() == 0) direction = !direction;
                if (direction) mpp->mYHeadRot += 90.0f;
                else mpp->mYHeadRot -= 90.0f;
                tick++;
            } else mpp->mYHeadRot += 90.0f;
        }
    }
}

