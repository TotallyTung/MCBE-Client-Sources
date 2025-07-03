#pragma once

class EmotePacket : public ::Packet {
public:
    enum class Flags : unsigned char {
        Invalid =    0x0,
        ServerSide = 0x1,
        MuteChat =   0x2,
    };

    ActorRuntimeID mRuntimeId;  // this+0x30
    std::string    mPieceId;    // this+0x38
    std::string    mXuid;       // this+0x58
    std::string    mPlatformId; // this+0x78
    Flags          mFlags;      // this+0x98
};