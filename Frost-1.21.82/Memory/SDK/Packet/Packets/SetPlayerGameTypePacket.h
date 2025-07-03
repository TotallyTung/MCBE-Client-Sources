#pragma once

class SetPlayerGameTypePacket : public Packet {
public:
    GameType mPlayerGameType; // this+0x30
};