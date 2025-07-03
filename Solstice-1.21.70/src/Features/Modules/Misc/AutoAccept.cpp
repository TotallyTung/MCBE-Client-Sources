//
// Created by vastrakai on 8/23/2024.
//

#include "AutoAccept.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

void AutoAccept::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoAccept::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &AutoAccept::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoAccept::onPacketOutEvent>(this);
}

void AutoAccept::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoAccept::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &AutoAccept::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoAccept::onPacketOutEvent>(this);
}

void AutoAccept::onBaseTickEvent(BaseTickEvent& event)
{
    for (auto& [time, cmd] : mQueuedCommands) {
        if (NOW - mLastCommandTime < 1000) {
            continue;
        }

        mLastCommandTime = NOW;
        CommandUtils::executeCommand(cmd);
        mQueuedCommands.erase(mQueuedCommands.begin());
        break;
    }


}

void AutoAccept::onPacketInEvent(PacketInEvent& event)
{
    if (event.mPacket->getId() == PacketID::Text)
    {
        auto packet = event.getPacket<TextPacket>();
        auto message = packet->mMessage;
        message = ColorUtils::removeColorCodes(message);

        //You received a friend invite from PlayerName.
        if (message.starts_with("You received a friend invite from ") && mAcceptFriendRequests)
        {
            std::string playerName = message.substr(34);
            playerName.pop_back();

            mQueuedCommands.push_back({NOW, "/friend accept " + playerName});
            spdlog::info("Accepting friend request from {}", playerName);
        }
        // PencilShip6193 wants you to join their party!
        else if (message.ends_with(" wants you to join their party!") && mAcceptPartyInvites)
        {
            std::string playerName = message.substr(0, message.find(" wants you to join their party!"));
            mQueuedCommands.push_back({NOW, "/party accept " + playerName});
            spdlog::info("Accepting party invite from {}", playerName);
        }
        //spdlog::info("Received message: {}", message);
    }
}

void AutoAccept::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::CommandRequest)
    {
        mLastCommandTime = NOW;
    }
}
