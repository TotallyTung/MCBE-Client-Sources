//
// Created by vastrakai on 1/28/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoDefend : public ModuleBase<AutoDefend> {
public:
    NumberSetting Thickness = NumberSetting("Thickness", "The thickness of the wall", 0, 5, 1, 0.1);
    NumberSetting PlacesPerTick = NumberSetting("PlacesPerTick", "How many blocks to place per tick", 1, 10, 1.f, 1.f);
    AutoDefend() : ModuleBase("AutoDefend", "Automatically defends your treasure on The Hive", "Misc", 0, false) {
        AddSetting(&Thickness);
        AddSetting(&PlacesPerTick);
    }
    /* Static variables */
    static bool Rotate;
    static Vector3 Block;

    /* Module functions */
    Actor* GetTreasure();
    std::vector<BlockPos> GetCollidingBlocks();
    std::vector<BlockPos> GetSurroundingBlocks(bool airOnly);
    Vector3 getClosestPlacePos(Vector3 block, float maxDistance, Vector3 playerPos);
    Vector3 GetNextPlacePos();
    void Tick();

    /* Module overrides */
    void OnEnable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel) override;
    void OnRender() override;
};
