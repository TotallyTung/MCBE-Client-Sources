//
// Created by vastrakai on 9/24/2023.
//

#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/SDK/Game/Network/Other/SetTitlePacket.h>
#include <Prax/Utils/String.h>
#include "StreamerMode.h"

std::string StreamerMode::NewName = "Prax User";
std::string StreamerMode::oldNameTag = "";
std::string StreamerMode::nameToReplace = "";
std::string oldLocalName = "";

void StreamerMode::OnInitialize(){
    NewName = Prax::Preferences->Module.StreamerName;
}

void StreamerMode::OnEnable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    oldLocalName = player->localName;
    player->localName = NewName;
    oldNameTag = player->_getNameTag();
    nameToReplace = oldLocalName;
    player->setNameTag(String::ReplaceAll(oldNameTag, nameToReplace.c_str(), NewName.c_str()));

}

void StreamerMode::OnTick(){

    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->localName = NewName;

    if (oldNameTag.empty()) {
        player->setNameTag(String::ReplaceAll(player->_getNameTag(), nameToReplace.c_str(), NewName.c_str()));
    }
}

void StreamerMode::OnDisable() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;
    player->setNameTag(oldNameTag);
    std::string bruh = player->_getNameTag();
    if (bruh != oldNameTag) {
        player->setNameTag(String::ReplaceAll(oldNameTag, NewName.c_str(), nameToReplace.c_str()));
    }
    player->localName = oldLocalName;
}

void StreamerMode::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (packet->getId() == (int)PacketID::Text) {
        auto tp = std::reinterpret_pointer_cast<TextPacket>(packet);

        std::string message = tp->message.getText();
        std::string formattedChatPing = "" + nameToReplace;


        if (String::ContainsIgnoreCase(formattedChatPing, " ")) {
            formattedChatPing = "\"" + formattedChatPing + "\"";
        }
        formattedChatPing = "@" + formattedChatPing;

        if (String::ContainsIgnoreCase(message, formattedChatPing)) {
            std::string lol = "§e@\"" + NewName + "\"§f";
            message = String::ReplaceAll(message, formattedChatPing.c_str(), lol.c_str());
        }



        message = String::ReplaceAll(message, nameToReplace.c_str(), NewName.c_str());

        tp->message.setText(message);
    }

    if (packet->getId() == (int)PacketID::SetTitle) {
        auto title = std::reinterpret_pointer_cast<SetTitlePacket>(packet);
        title->mTitleText = String::ReplaceAll(title->mTitleText, nameToReplace.c_str(), NewName.c_str());
    }
}