#pragma once

class InventoryContentPacket : public Packet {
public:
    ContainerID                             mInventoryId; // this+0x30
    std::vector<NetworkItemStackDescriptor> mSlots;       // this+0x38
};