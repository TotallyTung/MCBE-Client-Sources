//
// Created by vastrakai on 9/15/2023.
//

#include <string>
#include "src/Prax/SDK/Game/Network/Network.h"

class CommandRequestPacket : public Packet {
public:
    static const PacketID ID = PacketID::CommandRequest;
    // Use fields from PowerNukkitX's CommandRequestPacket

    enum class Origin {
        PLAYER = 0,
        COMMAND_BLOCK = 1,
        MINECART_COMMAND_BLOCK = 2,
        DEV_CONSOLE = 3,
        AUTOMATION_PLAYER = 4,
        CLIENT_AUTOMATION = 5,
        DEDICATED_SERVER = 6,
        ENTITY = 7,
        VIRTUAL = 8,
        GAME_ARGUMENT = 9,
        INTERNAL = 10
    };

    std::string Command;
    Origin CmdOrigin = Origin::PLAYER;
    bool isExternal = 0;
    char shit[0x256];

    CommandRequestPacket();

};