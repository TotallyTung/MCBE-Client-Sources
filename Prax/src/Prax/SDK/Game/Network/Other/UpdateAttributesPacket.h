//
// Created by vastrakai on 10/10/2023.
//

#pragma once

#include "src/Prax/SDK/Game/Network/Network.h"

struct AttributeData {
    float Min;
    float Max;
    float Value;
    float Default;
};

class UpdateAttributesPacket : public Packet {
public:
    static const PacketID ID = PacketID::UpdateAttributes;

    uint64_t EntityId;
    std::map<Attribute*, AttributeInstance*> Attributes;
    uint64_t Tick;

    UpdateAttributesPacket() = default;
};