//
// Created by vastrakai on 9/16/2023.
//

#include "src/Prax/SDK/Game/Network/Network.h"

class LoginPacket : public Packet {
public:
    static const PacketID ID = PacketID::Login;

    std::string Username;
    int Protocol;
    std::string ClientUUID;
    __int64 ClientId;

    LoginPacket() = default;
};