//
// Created by vastrakai on 4/17/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>

class OreMiner : public ModuleBase<OreMiner> {
public:
    /*enum class OreType : int {
        Diamond,
        Emerald,
        Gold,
        Iron,
        Lapis,
        Redstone,
        Coal,
        NetherQuartz,
        AncientDebris,
        Copper,
        Amethyst,
        None
    };*/

    enum class SelectionMode {
        Spoof
    };

    NumberSetting Range = NumberSetting("Range", "The range to mine ores", 1, 10, 6, 1);
    //ModeSetting OreType = ModeSetting("Ore Type", "The type of ore to mine", {"Diamond", "Emerald", "Gold", "Iron", "Lapis", "Redstone", "Coal", "Nether Quartz", "Ancient Debris", "Copper", "Amethyst"}, 0);
    // instead of using a modesetting, lets use a boolsetting for each ore type
    BoolSetting Diamond = BoolSetting("Diamond", "Whether or not to mine diamond ore", true);
    BoolSetting Emerald = BoolSetting("Emerald", "Whether or not to mine emerald ore", true);
    BoolSetting Gold = BoolSetting("Gold", "Whether or not to mine gold ore", true);
    BoolSetting Iron = BoolSetting("Iron", "Whether or not to mine iron ore", true);
    BoolSetting Lapis = BoolSetting("Lapis", "Whether or not to mine lapis ore", true);
    BoolSetting Coal = BoolSetting("Coal", "Whether or not to mine coal ore", true);
    BoolSetting NetherQuartz = BoolSetting("Nether Quartz", "Whether or not to mine nether quartz ore", true);
    BoolSetting AncientDebris = BoolSetting("Ancient Debris", "Whether or not to mine ancient debris ore", true);
    BoolSetting Copper = BoolSetting("Copper", "Whether or not to mine copper ore", true);
    BoolSetting Amethyst = BoolSetting("Amethyst", "Whether or not to mine amethyst ore", true);
    ModeSetting SelectionMode = ModeSetting("Selection Mode", "The mode for selecting blocks", {"Spoof"}, 0);
    NumberSetting DestroySpeed = NumberSetting("Destroy Speed", "The speed at which the ore is destroyed", 0, 1, 1.f, 0.01);
    BoolSetting Swing = BoolSetting("Swing", "Whether or not to swing your arm", true);
    BoolSetting RenderCurrentBlocks = BoolSetting("Render Current Blocks", "Whether or not to render the current blocks being mined", true);
    BoolSetting RenderPastBlocks = BoolSetting("Render Past Blocks", "Whether or not to render the past blocks being mined (for a specified amount of time)", true);
    NumberSetting RenderPastBlocksTime = NumberSetting("Render Past Blocks Time", "The time to render past blocks", 0, 10, 3, 0.01);

    OreMiner() : ModuleBase("OreMiner", "Mines ores for you", "Misc") {
        AddSetting(&Range);
        AddSetting(&Diamond);
        AddSetting(&Emerald);
        AddSetting(&Gold);
        AddSetting(&Iron);
        AddSetting(&Lapis);
        AddSetting(&Coal);
        AddSetting(&NetherQuartz);
        AddSetting(&AncientDebris);
        AddSetting(&Copper);
        AddSetting(&Amethyst);
        AddSetting(&SelectionMode);
        AddSetting(&DestroySpeed);
        AddSetting(&Swing);
        AddSetting(&RenderCurrentBlocks);
        AddSetting(&RenderPastBlocks);
        AddSetting(&RenderPastBlocksTime);
        PacketPriority = Priority::VeryLast;
    }

    static bool IsBreaking;
    static float Progress;
    static BlockPosPair CurrentBlock;
    static bool ShouldRotate;
    static std::map<Vector3, uint64_t> PastBlocks;

    struct QueuedPacket {
        Vector3 pos;
        std::shared_ptr<InventoryTransactionPacket> packet;
        int slot;
    };

    static std::map<uint64_t, OreMiner::QueuedPacket> QueuedPackets;
    void Reset();
    void OnTick() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
    void OnDisable() override;
    void OnRender() override;

    std::vector<int> GetOreType();

    bool IsExposed(Vector3 pos);

};
