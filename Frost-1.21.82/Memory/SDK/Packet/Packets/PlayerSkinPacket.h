#pragma once

class PlayerSkinPacket : public Packet {
public:
    mce::UUID      mUUID;                 // this+0x30
    SerializedSkin mSkin;                 // this+0x40
    std::string    mLocalizedNewSkinName; // this+0x2A8
    std::string    mLocalizedOldSkinName; // this+0x2C8
};