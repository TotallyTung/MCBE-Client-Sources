//
// Created by vastrakai on 1/27/2024.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

struct VoidStateStruct
{
    uint64_t LastCheck;
    bool IsOverVoid;
};

class Surround : public ModuleBase<Surround> {
public:
    enum SortingMode {
        DISTANCE,
        Y,
        NONE,
    };

    NumberSetting Distance = NumberSetting("Distance", "The max distance to target", 0, 10, 5.f, 0.1f);
    NumberSetting PlacesPerTick = NumberSetting("PlacesPerTick", "How many blocks to place per tick", 1, 10, 1.f, 1.f);
    NumberSetting AabbPadding = NumberSetting("AABB Padding", "The padding for the AABB", 0, 5, 0.5f, 0.1f);
    BoolSetting IgnoreVoid = BoolSetting("Ignore Void", "Whether or not to ignore void blocks", true);
    BoolSetting DoubleThickness = BoolSetting("Double Thickness", "Whether or not to double the thickness", false);
    BoolSetting PadY = BoolSetting("Pad Y", "Whether or not to pad the Y axis as well as X and Z", false);
    BoolSetting RotateSetting = BoolSetting("Rotate", "Whether or not to rotate to the block", true);
    ModeSetting SortingMode = ModeSetting("Sorting Mode", "The mode for sorting the blocks", {"Distance", "Y", "None"}, 1);

    Surround() : ModuleBase("Surround", "Surrounds other people with blocks and prevents them from moving", "Misc", 0, false)
    {
        AddSetting(&Distance);
        AddSetting(&PlacesPerTick);
        AddSetting(&AabbPadding);
        AddSetting(&IgnoreVoid);
        AddSetting(&DoubleThickness);
        AddSetting(&PadY);
        AddSetting(&RotateSetting);
        AddSetting(&SortingMode);
    }


    /* Static variables */
    static bool Rotate;
    static Vector3 Block;
    static std::map<Vector3, VoidStateStruct> VoidState;

    /* Module overrides */
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnRender() override;
    void OnPacket(LoopbackPacketSender* sender, Packet* packet, bool* cancel) override;

    /* Module functions */
    bool IsPosOverVoid(Vector3& pos);
    Actor* GetClosestActor();
    Vector3 GetNextPlacePos();
    std::vector<BlockPos> GetCollidingBlocks();
    std::vector<BlockPos> GetSurroundingBlocks(bool airOnly = true);
    void Tick();
};