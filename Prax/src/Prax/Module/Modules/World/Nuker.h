//
// Created by vastrakai on 2/24/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>


class Nuker : public ModuleBase<Nuker> {
public:
    enum class NukerMode {
        INSTANT,
        LEGIT,
#ifndef __RELEASEFREE__
        FATNGGA
#endif
    };

    enum class SortingMode {
        DISTANCE,
        DESTROY_TIME
    };

    ModeSetting Mode = ModeSetting("Mode", "Mode", { "Instant", "Legit",
#ifndef __RELEASEFREE__
        "FatNgga"
#endif
    }, 0);
    ModeSetting Sorting = ModeSetting("Sorting", "Sorting", { "Distance", "Destroy Time" }, 0);
    NumberSetting Range = NumberSetting("Range", "Range", 1, 12, 10, 1);
    NumberSetting DestroySpeed = NumberSetting("Destroy Speed", "Destroy Speed", 0, 1.f, 1.f, 1);
    BoolSetting Underground = BoolSetting("The Funny", "Underground bypass", false);
    BoolSetting Swing = BoolSetting("Swing", "Swing", true);
    BoolSetting Debug = BoolSetting("Debug", "Debug", false);
    BoolSetting RotateSetting = BoolSetting("Rotate", "Rotate", true);
    NumberSetting BlocksToBreak = NumberSetting("Blocks To Break", "Blocks To Break", 1, 5, 1, 1);
    BoolSetting Render = BoolSetting("Render", "Render", true);



    Nuker() : ModuleBase("Nuker", "Breaks blocks around you", "World") {
        Mode.Display = true;
        PacketPriority = Priority::Last;
        AddSetting(&Mode);
        AddSetting(&Sorting);
        AddSetting(&Range);
        AddSetting(&DestroySpeed);
        AddSetting(&Underground);
        AddSetting(&Swing);
        AddSetting(&Debug);
        AddSetting(&RotateSetting);
        AddSetting(&BlocksToBreak);
        AddSetting(&Render);
    }


    /* Static variables */
    static BlockPosPair CurrentBlock;
    static float Progress;
    static bool Rotate;
    static bool Breaking;

    /* Module Functions */
    void Reset();
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled);
    void OnRender();
#ifndef __RELEASEFREE__
    void ModeFatNggaTick();
#endif
    void ModeLegitTick();
    void ModeInstantTick();

    /* Module overrides */
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;

    bool HasToolForBlock(int blockId, std::vector<ItemStack *> items);
    bool IsBlockWithinYaw(BlockPos bPos, float playerYaw, float yawRange);
    bool IsBlockWithinPitch(BlockPos bPos, float playerPitch, float pitchRange);
    void StartDestroy(Vector3 pos, int face);
    void DestroyBlock(Vector3 pos, int face, int slot);

    bool IsExposed(Vector3 pos);
};
