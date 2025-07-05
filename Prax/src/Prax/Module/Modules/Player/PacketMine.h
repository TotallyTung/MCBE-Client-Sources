//
// Created by vastrakai on 12/21/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class InventoryTransactionPacket;

struct QueuedBlockData {
    Vector3 Position;
	int Side;
    Block* Block;
};

class PacketMine : public ModuleBase<PacketMine> {
public:
    NumberSetting DestroySpeed = NumberSetting("Destroy Speed", "How fast you should mine", 0.f, 1.f, 1.f, 0.01);
    BoolSetting RotateSetting = BoolSetting("Rotate", "Rotates towards the block you're mining", true);
    BoolSetting RenderQueued = BoolSetting("Render Queued", "Renders the blocks you're mining", true);
    BoolSetting SpoofToBestTool = BoolSetting("Spoof To Best Tool", "Spoofs your item to the best tool for the block you're mining", true);
    BoolSetting ExposedOnly = BoolSetting("Exposed Only", "Only mines blocks that are exposed", true);
    BoolSetting OresOnly = BoolSetting("Ores Only", "Only mines ores", false);

    PacketMine() : ModuleBase("PacketMine", "Mines blocks serversidedly instead of clientsidedly", "Player", 0, false)
    {
        PacketPriority = Priority::Last;
        AddSetting(&DestroySpeed);
        AddSetting(&RotateSetting);
        AddSetting(&RenderQueued);
        AddSetting(&SpoofToBestTool);
        AddSetting(&ExposedOnly);
        AddSetting(&OresOnly);
    }

    struct QueuedPacket {
        Vector3 pos;
        std::shared_ptr<InventoryTransactionPacket> packet;
        int slot;
    };

    static std::vector<QueuedBlockData> BlocksToMine;
    static std::map<uint64_t, QueuedPacket> QueuedPackets;
    static float BreakProgress;
    static bool IsDestroying;
    static bool Rotate;
    static Vector3 LastPos;

    void OnDisable() override;
    void RotateIf(Vector3 block, int side);
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnRender() override;
};
