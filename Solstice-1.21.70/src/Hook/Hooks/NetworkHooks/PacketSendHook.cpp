//
// Created by vastrakai on 6/28/2024.
//
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include "PacketSendHook.hpp"
#include <Features/FeatureManager.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

#include <SDK/Minecraft/World/Level.hpp>

#include "Features/Events/ChatEvent.hpp"
#include "Features/Events/PacketOutEvent.hpp"

class ChatEvent;
std::unique_ptr<Detour> PacketSendHook::mDetour = nullptr;

void* PacketSendHook::onPacketSend(void* _this, Packet *packet) {
    // Call the original function
    auto original = mDetour->getOriginal<&onPacketSend>();

    if (packet->getId() == PacketID::Text)
    {
        auto textPacket = reinterpret_cast<TextPacket*>(packet);
        if (gFeatureManager && gFeatureManager->mCommandManager)
        {
            std::string message = textPacket->mMessage;
            auto event = nes::make_holder<ChatEvent>(message);
            gFeatureManager->mDispatcher->trigger(event);
            if (event->isCancelled()) return nullptr;
        }
    }

    if (packet->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        if (mApplyJump)
        {
            auto player = ClientInstance::get()->getLocalPlayer();
            if (Keyboard::isUsingMoveKeys()) paip->mInputData |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN;
            if (!player->isOnGround() && player->wasOnGround() && Keyboard::isUsingMoveKeys()) {
                paip->mInputData |= AuthInputAction::START_JUMPING;
            }

            mApplyJump = false;
        }
    }

    auto holda = nes::make_holder<PacketOutEvent>(packet);
    gFeatureManager->mDispatcher->trigger(holda);
    if (holda->isCancelled()) return nullptr;

    if (packet->getId() == PacketID::PlayerAuthInput)
    {
        auto authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        authPacket->mInteractRots = authPacket->mRot;
    }

    /*if (packet->getId() == PacketID::PlayerAuthInput) {
        auto input = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        sendPacket(input);
        return nullptr;
    }*/

    return original(_this, packet);
}

void PacketSendHook::sendInputPacket(std::shared_ptr<PlayerAuthInputPacket> packet) {
    sendPacket(packet.get());
}

void PacketSendHook::sendPacket(PlayerAuthInputPacket* packet) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    auto sender = ClientInstance::get()->getPacketSender();

    if (mCurrentTick == 0 && packet->mClientTick > mCurrentTick) mCurrentTick = packet->mClientTick;
    mCurrentTick++;

    packet->mClientTick = mCurrentTick;
    if (auto player = ClientInstance::get()->getLocalPlayer()) {
        player->getLevel()->getLevelData()->mTick = mCurrentTick;
    }

    sender->sendToServer(packet);
}

void PacketSendHook::init() {
    mDetour = std::make_unique<Detour>("LoopbackPacketSender::send", reinterpret_cast<void*>(ClientInstance::get()->getPacketSender()->vtable[2]), &PacketSendHook::onPacketSend);
}
