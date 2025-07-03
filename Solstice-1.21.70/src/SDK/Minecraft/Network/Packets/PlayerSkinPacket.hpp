//
// Created by vastrakai on 9/24/2024.
//

#pragma once

class PlayerSkinPacket : public Packet {
public:
    static inline PacketID ID = PacketID::PlayerSkin;

    mce::UUID      mUUID;                 // this+0x30
    SerializedSkin mSkin;                 // this+0x40
    std::string    mLocalizedNewSkinName; // this+0x2A8
    std::string    mLocalizedOldSkinName; // this+0x2C8

    std::string toString() {
        std::stringstream ss;
        ss << "UUID: " << mUUID.toString();
        ss << ", Skin: " << mSkin.toString();
        return ss.str();
    }
};