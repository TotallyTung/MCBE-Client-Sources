#include "Packet.h"
#include "../../../Utils/Maths.h"

class DisconnectPacket : public Packet {
public:
    static const PacketID ID = PacketID::Disconnect;

    bool hideDisconnectionScreen = false;
    std::string message = "INVALID";

    DisconnectPacket() : Packet() {};
};