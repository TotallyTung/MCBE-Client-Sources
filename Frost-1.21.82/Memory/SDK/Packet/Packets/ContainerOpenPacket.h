#pragma once

class ContainerOpenPacket : public Packet {
public:
    ContainerID          mContainerId;    // this+0x30
    ContainerType        mType;           // this+0x31
    Vector3<int>         mPos;            // this+0x34
    int64_t              mEntityUniqueID; // this+0x40
};