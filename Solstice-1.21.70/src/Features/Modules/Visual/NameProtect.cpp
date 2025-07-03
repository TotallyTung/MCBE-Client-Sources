//
// Created by alteik on 12/10/2024.
//

#include "NameProtect.hpp"
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/Packet.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <regex>

void NameProtect::onEnable() {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mOldLocalName = player->getLocalName();
    mOldNameTag = player->getNameTag();

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &NameProtect::onBaseTickEvent, nes::event_priority::ABSOLUTE_FIRST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &NameProtect::onPacketInEvent, nes::event_priority::ABSOLUTE_FIRST>(this);
}

void NameProtect::onDisable() {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->setLocalName(mOldLocalName);
    player->setNametag(mOldNameTag);

    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &NameProtect::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &NameProtect::onPacketInEvent>(this);
}

void NameProtect::onBaseTickEvent(BaseTickEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->setLocalName(mNewName);
    player->setNametag(mNewName);
}

void NameProtect::onPacketInEvent(PacketInEvent& event) {
    if (event.mPacket->getId() != PacketID::Text) return;

    auto packet = event.getPacket<TextPacket>();
    std::string message = packet->mMessage;

    if (message.find(mOldLocalName) != std::string::npos) {
        std::regex oldNameRegex(mOldLocalName);
        message = std::regex_replace(message, oldNameRegex, mNewName);
    }

    packet->mMessage = message;
}