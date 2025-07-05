#pragma once

#include <Prax/SDK/Game/Network/Network.h>
#include <Prax/SDK/Game/Entity/Actor.h>

class TextPacket : Packet {
public:
    static const PacketID ID = PacketID::Text;

    unsigned __int8 type; //0x0028
    char pad_0031[7]; //0x0031
    TextHolder author; //0x0038
    TextHolder message; //0x0058
    char pad_0078[24]; //0x0078
    bool translationNeeded; //0x0090
    char pad_0091[7]; //0x0091
    TextHolder xuid; //0x0098
    TextHolder platformChatId; //0x00B8
    char pad_00D8[1096]; //0x00D8

    TextPacket() = default;

    /*TextPacket(Actor* Entity, std::string message) {
        memset(this, 0x0, sizeof(TextPacket));
        this->setVTable(PacketVtables::textPacket());
        this->translationNeeded = false;
        this->message = message;
        this->type = 1;

        if (Entity == nullptr)
            return;

        this->author.setText(Entity->_getNameTag()->getText());
        this->xuid.setText(this->xuid.getText());
        this->platformChatId.setText(this->xuid.getText());
    }*/

    static std::shared_ptr<TextPacket> Create(Actor* Entity, std::string message) {
        auto textPacket = MinecraftPackets::CreatePacket<TextPacket>();

        textPacket->translationNeeded = false;
        textPacket->message = message;
        textPacket->type = 1;

        if (Entity == nullptr)
            return textPacket;

        textPacket->author.setText(Entity->_getNameTag());
        textPacket->xuid.setText(textPacket->xuid.getText());
        textPacket->platformChatId.setText(textPacket->xuid.getText());

        return textPacket;
    }
};

// TextPacket but with std::string instead of TextHolder
class TextPacket2 : public Packet {
public:
    static const PacketID ID = PacketID::Text;

    unsigned __int8 type; //0x0028
    char pad_0031[7]; //0x0031
    std::string author; //0x0038
    std::string message; //0x0058
    char pad_0078[24]; //0x0078
    bool translationNeeded; //0x0090
    char pad_0091[7]; //0x0091
    std::string xuid; //0x0098
    std::string platformChatId; //0x00B8
    char pad_00D8[1096]; //0x00D8

    TextPacket2() = default;

    static std::shared_ptr<TextPacket2> Create(Actor* Entity, std::string message) {
        auto textPacket = MinecraftPackets::CreatePacket<TextPacket2>();

        textPacket->translationNeeded = false;
        textPacket->message = message;
        textPacket->type = 1;

        if (Entity == nullptr)
            return textPacket;

        textPacket->author = Entity->_getNameTag();
        textPacket->xuid = textPacket->xuid;
        textPacket->platformChatId = textPacket->xuid;

        return textPacket;
    }
};
