//
// Created by vastrakai on 9/14/2023.
//

#include "src/Prax/SDK/Game/Network/Network.h"

class LevelEventPacket : public Packet {
public:
    static const PacketID ID = PacketID::LevelEvent;

    static const int EVENT_START_RAIN = 3001;
    static const int EVENT_START_THUNDER = 3002;
    static const int EVENT_STOP_RAIN = 3003;
    static const int EVENT_STOP_THUNDER = 3004;
    int eventId;
    Vector3 pos;
    int data;

    LevelEventPacket() : Packet() {}
};

