//
// Created by vastrakai on 9/15/2023.
//

#include "src/Prax/SDK/Game/Network/Network.h"

class CommandOutputMessage{
public:


    /*From pnx
    boolean internal;
    @NotNull
    String messageId;
    @NotNull
    String[] parameters;*/
    bool Internal;
    std::string MessageId;
    std::vector<std::string> Params;
};

class CommandOutputPacket : public Packet
{
public:
    static const PacketID ID = PacketID::CommandOutput;

    std::vector<CommandOutputMessage> messages = {};
    __int64 unknown2;
    __int64 unknown3;
    int successCount;
    std::string data;
};