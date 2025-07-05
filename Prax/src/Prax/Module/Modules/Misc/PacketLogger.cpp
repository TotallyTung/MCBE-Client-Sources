//
// Created by vastrakai on 9/14/2023.
//

#include <Prax/SDK/Game/Network/Other/SetEntityMotionPacket.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/UpdateAttributesPacket.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>
#include <Prax/SDK/Game/Network/Other/UpdateBlockPacket.h>
#include "PacketLogger.h"

std::vector<int> PacketLogger::ExcludedPacketIds = {
        (int)PacketID::MoveEntityAbsolute,
        (int)PacketID::MoveEntityDelta,
        (int)PacketID::SetEntityMotion,
        (int)PacketID::SetEntityLink,
        (int)PacketID::PlaySound,
        (int)PacketID::LevelSoundEvent,
        (int)PacketID::LevelSoundEvent2,
        (int)PacketID::LevelSoundEvent3,
        (int)PacketID::Animate,
        (int)PacketID::PlayerList,
        (int)PacketID::SetTime,
        (int)PacketID::RemoveEntity,
        (int)PacketID::NetworkChunkPublisherUpdate,
        (int)PacketID::AddEntity,
        (int)PacketID::MovePlayer,
        (int)PacketID::LevelEvent,
        (int)PacketID::SetEntityData,
        (int)PacketID::UpdateAttributes,
        (int)PacketID::LevelChunk,
        (int)PacketID::SetTitle,
        (int)PacketID::NetworkStackLatency,
        (int)PacketID::UpdateBlock,
};

bool LogFieldsForPacket(int packetId){
    std::vector loggedPackets = {
            (int)PacketID::Text,
            (int)PacketID::SetEntityData,
            (int)PacketID::UpdateAttributes,
            (int)PacketID::MobEquipment,
            (int)PacketID::PlayerAuthInput,
            (int)PacketID::PlayerAction,
            (int)PacketID::InventoryTransaction,
            //(int)PacketID::PlayerAuthInput
    };

    return std::find(loggedPackets.begin(), loggedPackets.end(), packetId) != loggedPackets.end();
}

void PacketLogger::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (!LogIncoming.Enabled) return;

    // Check if the packet is excluded
    if (std::find(ExcludedPacketIds.begin(), ExcludedPacketIds.end(), (int)packet->getId()) != ExcludedPacketIds.end()) return;
    if (!LogFieldsForPacket((int)packet->getId())) Logger::Write("PacketLogger", "[in] " + packet->getName(), Logger::LogType::Info);

    if (packet->getId() == (int)PacketID::Text) {
        auto textPacket = std::reinterpret_pointer_cast<TextPacket>(packet);
        std::string text = textPacket->message.getText();
        text = String::ReplaceAll(text, "§", "&");
        Logger::Write("PacketLogger", "[in] " + packet->getName() + " - " + text, Logger::LogType::Info);
    }
    else if (packet->getId() == (int)PacketID::UpdateAttributes) {
        auto uaPacket = std::reinterpret_pointer_cast<UpdateAttributesPacket>(packet);
        std::string outputStr = "[in] " + packet->getName() + " - ";
        // copy the packet address to the clipboard

        Misc::SetClipboard(String::ToHex((uintptr_t)&uaPacket));
        Logger::Write("PacketLogger", outputStr, Logger::LogType::Info);
    } else if (packet->getId() == (int)PacketID::UpdateBlock) {
        auto ub = std::reinterpret_pointer_cast<UpdateBlockPacket>(packet);
        std::string outputStr = "[in] " + packet->getName() + " - " + ub->toString();
        Logger::Write("PacketLogger", outputStr, Logger::LogType::Info);
    }


}

void PacketLogger::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (!LogOutgoing.Enabled) return;

    // Check if the packet is excluded
    if (std::find(ExcludedPacketIds.begin(), ExcludedPacketIds.end(), (int) packet->getId()) !=
    ExcludedPacketIds.end())
        return;
    if (!LogFieldsForPacket((int) packet->getId()))
        Logger::Write("PacketLogger", "[out] " + packet->getName(), Logger::LogType::Info);


    if (packet->getId() == (int)PacketID::Text) {
        auto textPacket = reinterpret_cast<TextPacket*>(packet);
        std::string text = textPacket->message.getText();

        Logger::Write("PacketLogger", "[out] " + packet->getName() + " - " + text, Logger::LogType::Info);
    }

    if (packet->getId() == (int)PacketID::MobEquipment) {
        auto mobEquipmentPacket = reinterpret_cast<MobEquipmentPacket*>(packet);
        std::string outputStr = "[out] " + packet->getName() + " - ";
        outputStr += "Entity ID: " + std::to_string(mobEquipmentPacket->RuntimeId);
        outputStr += " HotbarSlot: " + std::to_string(mobEquipmentPacket->HotbarSlot);
    }

    if (packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto playerAuthInputPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        std::string outputStr = "[out] " + packet->getName() + " - ";
        // Check if the input flags contains START_JUMPING
        if ((playerAuthInputPacket->inputFlags & AuthInputAction::START_JUMPING) == AuthInputAction::START_JUMPING) {
            outputStr += "START_JUMPING ";
        }


        Logger::Write("PacketLogger", outputStr, Logger::LogType::Info);
    }

    if (packet->getId() == (int)PacketID::PlayerAction) {
        auto uaPacket = reinterpret_cast<PlayerActionPacket*>(packet);
        std::string outputStr = "[out] " + packet->getName() + " - " + uaPacket->GetActionType() + " pos: " + uaPacket->mPos.ToString() + " resultPos: " + uaPacket->mResultPos.ToString();
        Logger::Write("PacketLogger", outputStr, Logger::LogType::Info);

    }

    if (packet->getId() == (int)PacketID::InventoryTransaction) {
        auto itp = reinterpret_cast<InventoryTransactionPacket*>(packet);
        std::string outputStr = "[out] " + packet->getName() + " - " + "TransactionType: " + itp->mTransaction->GetTransactionType();


        Logger::Write("PacketLogger", outputStr, Logger::LogType::Info);
    }
}
