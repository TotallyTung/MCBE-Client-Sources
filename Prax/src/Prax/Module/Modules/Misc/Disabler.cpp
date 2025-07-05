//
// Created by vastrakai on 8/27/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <imgui_internal.h>
#include <Prax/Utils/System/KeyManager.h>
#include "Disabler.h"

#include <Prax/Hook/Hooks/PacketSender.h>
#include <Prax/SDK/Game/Network/Other/CompletedUsingItemPacket.h>
#include <Prax/Utils/Math.h>

void Disabler::OnEnable() {

}

void Disabler::OnDisable() {

}

#define NOW Misc::GetCurrentMs()

Vector3 lastDelta = Vector3(0, 0, 0);

void Disabler::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (DisablerMode.SelectedIndex == NukkitLagback) // MoveInput
    {
        static uint64_t lastSpin = 0;
        if (player->wasOnGround() && !player->isOnGround()) {
            player->startSpinAttack();
            player->stopSpinAttack();
            player->startSwimming();
        }


        /*auto paip= player->getInputPacket();
        paip->rotation = Vector2(player->GetActorRotationComponent()->pitch, player->GetActorRotationComponent()->yaw);
        paip->headYaw = player->GetActorRotationComponent()->yaw;
        paip->ticksAlive = 5;
        Minecraft::ClientInstance->packetSender->sendToServer(paip.get());*/




    }
    else if (DisablerMode.SelectedIndex == Sentinel) { // Sentinel

        // Check the current server ip and make sure it contains cubecraft.net
        std::string serverIp = Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->serverIp;
        if (serverIp.find(xorstr_("cubecraft.net")) == std::string::npos) return;

        // If the delta is 0 dont do anything
        if (lastDelta == Vector3(0, 0, 0)) return;

        auto paip = player->getInputPacket();
        paip->position = Vector3(0, 0, 0);
        paip->moveVector = Vector2(0, 0);
        paip->inputMode = AuthInputMode::TOUCH;
        bool t = false;
        PacketSender::HandleAuthInputTick(paip.get(), &t);
        Minecraft::ClientInstance->packetSender->sendToServer(paip.get());
    }
}

Vector3 oldAuthPos = Vector3(0, 0, 0);


static uint64_t LastDimensionChange = 0;
static uint64_t LastTeleport = 0;


void Disabler::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (DisablerMode.SelectedIndex == Sentinel && packet->getId() == (int) PacketID::PlayerAuthInput) {
        std::string serverIp = Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->serverIp;
        if (serverIp.find(xorstr_("cubecraft.net")) == std::string::npos) return;
        auto *paip = (PlayerAuthInputPacket *) packet;

        if (paip->position == Vector3(0.f, 0.f, 0.f)) return;
        lastDelta = paip->position - *player->getPosPrev();

    }

    if (DisablerMode.SelectedIndex == NetherGames && packet->getId() == (int)PacketID::PlayerAuthInput){ // most dangerous nethergames bypass
        auto *paip = (PlayerAuthInputPacket *) packet;

        paip->moveVector = Vector2(0.f, 1.f);
        if (KeyManager::Keys['S']) paip->moveVector = Vector2(0.f, -1.f);
        paip->velocity = Vector3(0.f, -0.0784000015258789f, 0.f);
        paip->inputMode = (AuthInputMode)Math::RandomInt(1, 6);
    }

    if (DisablerMode.SelectedIndex == BdsPredictions && packet->getId() == (int)PacketID::PlayerAuthInput){
        auto *paip = (PlayerAuthInputPacket *) packet;

        paip->moveVector = Vector2(0.f, 1.f);
        paip->inputFlags |= AuthInputAction::START_GLIDING;
        paip->inputFlags &= ~AuthInputAction::STOP_GLIDING;
    }

    if (DisablerMode.SelectedIndex == Custom && packet->getId() == (int)PacketID::PlayerAuthInput){
        auto *paip = (PlayerAuthInputPacket *) packet;

        paip->velocity = Vector3(VelX.Value, VelY.Value, VelZ.Value);
    }
}


static uint64_t newLatencyTimestamp = 0;


void Disabler::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (packet->getId() == (int)PacketID::ChangeDimension)
    {
        LastDimensionChange = NOW;
    }
    else if (packet->getId() == (int)PacketID::MovePlayer)
    {
        LastTeleport = NOW;
    }

    if (DisablerMode.SelectedIndex == Sentinel)
    {
        // Check server ip
        std::string serverIp = Minecraft::ClientInstance->packetSender->networkSystem->getConnectionInfo()->serverIp;
        if (serverIp.find(xorstr_("cubecraft.net")) == std::string::npos) return;
        if (packet->getId() == (int)PacketID::MovePlayer && DisablerMode.SelectedIndex == 4) {
            std::shared_ptr <MovePlayerPacket> mpp = std::reinterpret_pointer_cast<MovePlayerPacket>(packet);
            if (mpp->position.Distance(*player->getPos()) < 6.f) {
                *cancel = true;
            }
        }

        /*if (packet->getId() == (int)PacketID::NetworkStackLatency)
        {
            auto pkt = std::reinterpret_pointer_cast<NetworkStackLatencyPacket>(packet);
            auto diff = (pkt->createTime / 1000) - newLatencyTimestamp;
            if (diff == 1000000) {
                Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
                pkt->createTime -= 1000000000;
                Minecraft::ClientInstance->packetSender->sendToServer(packet.get());
                *cancel = true;
            }
            newLatencyTimestamp = pkt->createTime / 1000;
        }*/
    }
}