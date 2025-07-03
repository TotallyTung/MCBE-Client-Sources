#pragma once

class AddPaintingPacket : public ::AddActorBasePacket {
public:
    uint64_t       mEntityId;  // this+0x30
    ActorRuntimeID mRuntimeId; // this+0x38
    Vector3<float> mPos;       // this+0x40
    int            mDir;       // this+0x4c
    std::string    mMotive;    // this+0x50
};