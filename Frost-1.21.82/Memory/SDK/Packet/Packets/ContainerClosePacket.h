#pragma once

class ContainerClosePacket : public Packet {
public:
    ContainerID mContainerId;          // this+0x30
    bool        mServerInitiatedClose; // this+0x31
};