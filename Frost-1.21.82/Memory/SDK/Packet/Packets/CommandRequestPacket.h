#pragma once

class CommandOriginData {
public:
    CommandOriginType mType;
    mce::UUID          mUuid;
    std::string       mRequestId;
    int64_t           mPlayerId;
};

class CommandRequestPacket : public Packet {

public:
    std::string       mCommand;        // this+0x30
    CommandOriginData mOrigin;         // this+0x50
    bool              mInternalSource; // this+0x90
};