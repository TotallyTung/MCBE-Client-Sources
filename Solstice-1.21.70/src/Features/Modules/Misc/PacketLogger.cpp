//
// Created by vastrakai on 7/1/2024.
//

#include "PacketLogger.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/ModalFormResponsePacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/ActorDataPacket.hpp>
#include <SDK/Minecraft/Network/Packets/ItemStackRequestPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerActionPacket.hpp>


class PlayerAuthInputPacket;

void PacketLogger::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &PacketLogger::onPacketOutEvent, nes::event_priority::ABSOLUTE_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &PacketLogger::onPacketInEvent, nes::event_priority::ABSOLUTE_LAST>(this);
}

void PacketLogger::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &PacketLogger::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &PacketLogger::onPacketInEvent>(this);
}

std::vector<PacketID> ignored = {
    PacketID::LevelChunk,
    PacketID::Animate,
    PacketID::MoveActorAbsolute,
    PacketID::MoveActorDelta,
    PacketID::UpdateAttributes,
    PacketID::MobEquipment,
    PacketID::PlayerAuthInput,
    PacketID::AddActor,
    PacketID::RemoveActor,
    PacketID::PlaySound,
    PacketID::SetTime,
    PacketID::PlayerList,
    PacketID::PlayerSkin
};

void PacketLogger::onPacketOutEvent(PacketOutEvent& event)
{
    if (!mOutgoing.mValue) return;
    if (std::ranges::find(ignored, event.mPacket->getId()) != ignored.end()) return;

    // Log the packet
    spdlog::info("[out] Packet: {}", event.mPacket->getName());
    if (event.mPacket->getId() == PacketID::Login)
    {
        auto packet = event.getPacket<Packet>();
        spdlog::info("Packet: {}", "Login");
    };

    if (event.mPacket->getId() == PacketID::PlayerAction)
    {
        auto packet = event.getPacket<PlayerActionPacket>();
        spdlog::info("Packet: PlayerAction, action: {}", magic_enum::enum_name(packet->mAction));
    };


    if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
        auto packet = event.getPacket<PlayerAuthInputPacket>();
        spdlog::info("Packet: PlayerAuthInput, info: {}", packet->toString());
    }

    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        auto packet = event.getPacket<InventoryTransactionPacket>();
        auto cit = packet->mTransaction.get();

        spdlog::info("Packet: InventoryTransaction, TransactionType: {}", magic_enum::enum_name(packet->mTransaction->type));
        if (cit->type == ComplexInventoryTransaction::Type::ItemUseOnEntityTransaction)
        {
            const auto iut = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(cit);
            if (iut->mActionType == ItemUseOnActorInventoryTransaction::ActionType::Attack)
            {
                spdlog::info("Packet: InventoryTransaction, ClickPos: {}/{}/{}", iut->mClickPos.x, iut->mClickPos.y, iut->mClickPos.z);
            }
        } else if (cit->type == ComplexInventoryTransaction::Type::NormalTransaction)
        {
            spdlog::info("Packet: InventoryTransaction, NormalTransaction");
        }

    }

    if (event.mPacket->getId() == PacketID::ModalFormResponse)
    {
        auto packet = event.getPacket<ModalFormResponsePacket>();
        std::string json = "";
        spdlog::info("Packet: ModalFormResponse, FormId: {}, FormJson: {}", packet->mFormId, packet->mJSONResponse.has_value() ? packet->mJSONResponse.value().toString() : "null");
    }
}


void PacketLogger::onPacketInEvent(PacketInEvent& event)
{
    if (!mIncoming.mValue) return;
    if (std::ranges::find(ignored, event.mPacket->getId()) != ignored.end()) return;

    if (event.mPacket->getId() == PacketID::SetActorData)
    {
        auto player = ClientInstance::get()->getLocalPlayer();

        auto packet = event.getPacket<ActorDataPacket>();
        if (packet->mId == player->getRuntimeID())
        {
            spdlog::info("[in] Packet: " + packet->toString());
        }

        return;
    }

    spdlog::info("[in] Packet: {}", event.mPacket->getName());

}
