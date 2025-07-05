//
// Created by vastrakai on 9/15/2023.
//


#include <string>
#include "src/Prax/SDK/Game/Network/Network.h"

class DisconnectPacket : public Packet {
public:
    static const PacketID ID = PacketID::Disconnect;

    bool hideDisconnectionScreen = false;
    std::string message = "INVALID";

    DisconnectPacket() : Packet() {};
};