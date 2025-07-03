//
// Created by vastrakai on 9/2/2024.
//

#include "ServerSneak.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/ActorDataPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void ServerSneak::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &ServerSneak::onPacketOutEvent, nes::event_priority::ABSOLUTE_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &ServerSneak::onPacketInEvent>(this);
}

void ServerSneak::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &ServerSneak::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &ServerSneak::onPacketInEvent>(this);
}

void ServerSneak::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        auto at = event.getPacket<InventoryTransactionPacket>();
        auto trs = at->mTransaction.get();
        if (trs->type == ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            auto iut = reinterpret_cast<ItemUseInventoryTransaction*>(trs);
            if (iut->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                static std::unordered_map<int, glm::vec3> faceToClickPos = {
                    {0, glm::vec3(0.5, 0, 0.5)},
                    {1, glm::vec3(0.5, 1, 0.5)},
                    {2, glm::vec3(0.5, 0.5, 0)},
                    {3, glm::vec3(0.5, 0.5, 1)},
                    {4, glm::vec3(0, 0.5, 0.5)},
                    {5, glm::vec3(1, 0.5, 0.5)}
                };

                int face = iut->mFace;
                glm::vec3 clickPos = faceToClickPos[face];

                if (iut->mClickPos == clickPos) return;

                mLastInteract = NOW;
            }
        }
    }

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        if (NOW - mLastInteract < 100)
        {
            // remove start sneaking
            paip->mInputData &= ~AuthInputAction::START_SNEAKING;
            // add stop sneaking
            paip->mInputData |= AuthInputAction::STOP_SNEAKING;
            return;
        }

        paip->mInputData |= AuthInputAction::SNEAK_DOWN | AuthInputAction::SNEAKING | AuthInputAction::START_SNEAKING;
        // remove stop sneaking
        paip->mInputData &= ~AuthInputAction::STOP_SNEAKING;
    }
}

void ServerSneak::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();

    static uint64_t lastDimensionChange = 0;

    if (event.mPacket->getId() == PacketID::ChangeDimension)
    {
        lastDimensionChange = NOW;
    }

    if (event.mPacket->getId() == PacketID::SetActorData && mCancelActorData.mValue)
    {
        if (NOW - lastDimensionChange < 1000 || player->getStatusFlag(ActorFlags::Noai)) return;

        auto packet = event.getPacket<ActorDataPacket>();
        if (packet->mId == player->getRuntimeID())
        {
            event.cancel();
        }
    }
}
