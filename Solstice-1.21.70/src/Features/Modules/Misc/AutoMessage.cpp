//
// Created by vastrakai on 7/19/2024.
//

#include "AutoMessage.hpp"

#include <fstream>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

void AutoMessage::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoMessage::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoMessage::onBaseTickEvent>(this);
}

void AutoMessage::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoMessage::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoMessage::onBaseTickEvent>(this);
}

void AutoMessage::onBaseTickEvent(BaseTickEvent& event)
{
    if (mQueuedMessages.empty())
        return;

    int64_t interval = 500;

    // Get the first message in the queue
    auto it = mQueuedMessages.begin();
    if (it->first < NOW - interval)
    {
        PacketUtils::sendChatMessage(it->second);
        mQueuedMessages.erase(it);
        spdlog::info("Sent message: {}", it->second);
    }
}

void AutoMessage::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::Text)
    {
        auto packet = event.getPacket<TextPacket>();

        if (mOnVote.mValue && packet->mMessage.starts_with("§b§l» §r§7You voted for §e"))
        {
            std::string mapName = packet->mMessage;
            // simply replace the §b§l» §r§7You voted for §e with nothing
            mapName = StringUtils::replaceAll(mapName, "§b§l» §r§7You voted for §e", "");
            mVoteMessageTemplate.setVariableValue("mapName", mapName);
            std::string chatMessage = mVoteMessageTemplate.getEntry();
            mQueuedMessages[NOW] = chatMessage;
        }

        spdlog::info("Received message: {}", packet->mMessage);
    }
    if (event.mPacket->getId() == PacketID::ChangeDimension && mOnDimensionChange.mValue)
    {
        static int64_t lastDimensionChange = 0;
        if (NOW - lastDimensionChange < 1000)
            return;

        lastDimensionChange = NOW;

        std::string chatMessage = mDimensionChangeMessageTemplate.getEntry();
        mQueuedMessages[NOW] = chatMessage;
    }
}
