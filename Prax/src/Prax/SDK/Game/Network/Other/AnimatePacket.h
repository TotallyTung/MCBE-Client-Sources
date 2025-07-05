//
// Created by vastrakai on 11/10/2023.
//

#pragma once
#include <Prax/SDK/Game/Network/Network.h>


class AnimatePacket : public Packet
{
public:
    static const PacketID ID = PacketID::Animate;

    uint64_t ActorRuntimeID;
    unsigned int ActionID;
    float Data;


};
