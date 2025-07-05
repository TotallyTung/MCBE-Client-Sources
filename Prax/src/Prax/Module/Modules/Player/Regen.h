//
// Created by vastrakai on 12/31/2023.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>


class Regen : public ModuleBase<Regen> {
public:
    enum class ProgressCentering {
        FromLeft,
        FromCenter,
        FromRight,
    };

    enum class DestroySpeedMode {
        Percent,
        Milliseconds,
    };

    enum class UncoverSorting {
        Distance,
        DestroySpeed
    };

    enum class ProgressStyle {
        Default,
        Zephyr
    };

    enum class StartBreakMethod {
        AuthInputActions,
        PlayerActionPacket
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Flareon"}, 0);
    NumberSetting Distance = NumberSetting("Distance", "The max distance to mine blocks", 0, 20, 5.0f, 0.01);
    ModeSetting StartBreakMethod = ModeSetting("Start Break Method", "The method to use for starting to break blocks", {"Auth Input Actions", "Player Action Packet"}, 0);
    ModeSetting DestroySpeedMode = ModeSetting("Destroy Speed Mode", "The mode to use for destroy speed", {"Percent", "Milliseconds"}, 0);
    NumberSetting DestroySpeed = NumberSetting("Destroy Speed", "The speed at which to mine blocks", 0, 1, 0.63f, 0.01);
    NumberSetting DestroySpeedMilliseconds = NumberSetting("Destroy Speed (ms)", "The speed at which to mine blocks in milliseconds", 0, 1000, 500.f, 1);
    NumberSetting OreDestroySpeed = NumberSetting("Ore Destroy Speed", "The speed at which to mine ores", 0, 1, 0.75f, 0.01);
    NumberSetting OtherDestroySpeed = NumberSetting("Other Destroy Speed", "The speed at which to mine non-redstone blocks", 0, 1, 0.63f, 0.01);
    BoolSetting RenderBlock = BoolSetting("Render Block", "Whether or not to render the block being mined", true);
    BoolSetting RenderProgress = BoolSetting("Render Progress", "Whether or not to render the progress of the block being mined", true);
    ModeSetting ProgressStyle = ModeSetting("Progress Style", "The style of the progress bar", {"Default", "Zephyr" }, 0);
    NumberSetting ProgressSize = NumberSetting("Progress Size", "The size of the progress bar", 0, 30, 20.f, 0.01, &RenderProgress);
    NumberSetting ProgressYOffset = NumberSetting("Progress Y Offset", "The Y offset of the progress bar", -300, 300, 0.f, 0.01, &RenderProgress);
    BoolSetting Swing = BoolSetting("Swing", "Whether or not to swing your arm", true);
    ModeSetting UncoverSorting = ModeSetting("Uncover Sorting", "The mode to use for sorting blocks", {"Distance", "Destroy Speed"}, 0);
    BoolSetting AliensDetection = BoolSetting("Aliens Detection", "AAAH NO ALIENS ALLOWED", true);
#ifdef __RELEASEFREE__
    enum class UncoverMode {
        One,
        Two,
    };
    ModeSetting UncoverMode = ModeSetting("Uncover Mode", "The mode to use for uncovering blocks", {"One", "Two"}, 0);
    BoolSetting FreeUncover = BoolSetting("Uncover", "Whether or not to uncover blocks", true);
#endif
#ifndef __RELEASEFREE__
    enum class UncoverMode {
        One,
        Two,
        Three,
        Four,
        Five,
        Six,
        Seven,
        Eight
    };

    enum class FunnyMode {
        TopOnly,
        AllSides,
        RightSide,
    };

    BoolSetting NaClVisuals = BoolSetting("NaCl Visuals", "NaCl Visuals", true);
    BoolSetting AbsorptionCheck = BoolSetting("Absorption Check", "Whether or not to check for absorption", true);
    NumberSetting QueueDistance = NumberSetting("Queue Distance", "The max distance to queue ores", 0, 20, 5.0f, 0.01);
    BoolSetting UndergroundBypass = BoolSetting("Uncover", "Allows you to uncover redstone that is underground", true);
    ModeSetting UncoverMode = ModeSetting("Uncover Mode", "The mode to use for uncovering blocks", {"One", "Two", "Three", "Four", "Five", "Six", "Seven", "Eight"}, 0);
    BoolSetting QueueOres = BoolSetting("Queue Ores", "Whether or not to queue ores", false);
    BoolSetting StealOres = BoolSetting("Steal ores", "Whether or not to steal ores", false);
    BoolSetting AlwaysSteal = BoolSetting("Always Steal", "Steals ores even if you have full absorption", false);
    BoolSetting AntiSteal = BoolSetting("Anti Steal", "Switches to another ore when the one you are uncovering is being mined", false);
    BoolSetting ResetWhenCovered = BoolSetting("Reset When Covered", "Whether or not to reset the regen state when your block is covered", true);
    BoolSetting ResetWhenCoveredQueued = BoolSetting("Reset When Covered Queued", "Whether or not to reset the regen state when your block is covered", true);
    BoolSetting ExcludePlacedBlocks = BoolSetting("Exclude Placed Blocks", "Whether or not to exclude blocks placed recently", false);
    BoolSetting NoUncoverWhileStealing = BoolSetting("No Uncover While Stealing", "Whether or not to uncover blocks while stealing", false);
    BoolSetting NoUncoverRandomized = BoolSetting("No Uncover Randomized", "hehe :3", false);
    BoolSetting Debug = BoolSetting("Debug", "Whether or not to print debug information", false);
    BoolSetting InstabreakFunny = BoolSetting("Instabreak Funny", "silly instabreak :3", true);
    NumberSetting FunnyMaxMs = NumberSetting("Funny Max Ms", "The max milliseconds for instabreak funny", 0, 1000, 200.f, 1, &InstabreakFunny);
    BoolSetting InfiniteDurability = BoolSetting("Infinite Durability", "Whether or not to have infinite durability", false);
    BoolSetting BreakSpeedAdjustment = BoolSetting("Break Speed Adjustment", "Whether or not to adjust the break speed", false, &InfiniteDurability);
    BoolSetting InfiniteFunnies = BoolSetting("Infinite Funnies", "hehe :3", false);
    BoolSetting TheFunny = BoolSetting("The Funny", "The funny", false);
    ModeSetting FunnyMode = ModeSetting("Funny Mode", "The funny mode", {"Top only", "All sides", "Right Side"}, 0, &TheFunny);

    /*BoolSetting RiskyOpts = BoolSetting("Risky Optimizations", "bipolar regen optimizations\nby kay, all rights reserved", false);
    /*
     * (C)onserve (Q)ueue
     * (T)imer (B)reak (S)ync
     #1#
    NumberSetting MaxCQMs = NumberSetting("Max CQ Ms", "The max milliseconds for CQ optimizations, higher max means higher risk of getting stolen.", 0, 1000, 200.f, 1, &RiskyOpts);
    NumberSetting MaxCQHurtTime = NumberSetting("Max CQ Hurt", "The max hurt time of self for CQ optimizations to take effect.\nOnly needed if a breakspeed bypass is found, otherwise keep on max.", 0, 10, 10, 1, &RiskyOpts);
    BoolSetting TBSSync = BoolSetting("TBS Sync", "Minor timer adjustments to attempt to break earlier.", false, &RiskyOpts);*/

#endif

    Regen() : ModuleBase("Regen", "Automatically breaks redstone on The Hive", "Player", 0, false) {
        PacketPriority = Priority::Last;
        AddSetting(&Mode);
        Mode.Display = true;
        AddSetting(&Distance);
        AddSetting(&DestroySpeedMode);
        AddSetting(&StartBreakMethod);
        AddSetting(&DestroySpeed);
        AddSetting(&DestroySpeedMilliseconds);
        AddSetting(&OreDestroySpeed);
        AddSetting(&OtherDestroySpeed);
        AddSetting(&RenderBlock);
        AddSetting(&RenderProgress);
        AddSetting(&ProgressStyle);
        AddSetting(&ProgressYOffset);
        AddSetting(&ProgressSize);
        AddSetting(&Swing);
        AddSetting(&UncoverSorting);
        AddSetting(&AliensDetection);
#ifdef __RELEASEFREE__
        AddSetting(&UncoverMode);
        AddSetting(&FreeUncover);
#endif
#ifndef __RELEASEFREE__
        AddSetting(&NaClVisuals);
        AddSetting(&AbsorptionCheck);
        AddSetting(&QueueDistance);
        AddSetting(&UndergroundBypass);
        AddSetting(&UncoverMode);
        AddSetting(&QueueOres);
        AddSetting(&StealOres);
        AddSetting(&AlwaysSteal);
        //AddSetting(&AntiSteal);
        AddSetting(&ResetWhenCovered);
        AddSetting(&ResetWhenCoveredQueued);
        AddSetting(&ExcludePlacedBlocks);
        AddSetting(&NoUncoverWhileStealing);
        AddSetting(&NoUncoverRandomized);
        AddSetting(&Debug);
        AddSetting(&InstabreakFunny);
        AddSetting(&FunnyMaxMs);
        AddSetting(&InfiniteDurability);
        AddSetting(&BreakSpeedAdjustment);
        AddSetting(&InfiniteFunnies);
        AddSetting(&TheFunny);
        AddSetting(&FunnyMode);

        /*AddSetting(&RiskyOpts);
        AddSetting(&MaxCQMs);
        AddSetting(&MaxCQHurtTime);
        AddSetting(&TBSSync);*/
#endif
        RenderOnDisabled = true;
    }

    struct QueuedPacket {
        Vector3 pos;
        std::shared_ptr<InventoryTransactionPacket> packet;
        int slot;
    };

    static bool IsBreaking;
    static float Progress;
    static BlockPosPair CurrentBlock;
    static bool ShouldRotate;
    static bool StartStealing;
    static BlockPosPair StealBlock;
    static int StealSide;
    static bool IsStealing;
    static uint64_t StealStartTime;
    static float AnimationPerc;
    static std::map<uint64_t, QueuedPacket> QueuedPackets;
    static std::vector<std::pair<uint64_t, Vector3>> RecentlyCoveredBlocks;
    static uint64_t LastBlockPlace;

    void Reset();
    bool ShouldQueueOres();
    float GetBbDestroySpeed(Block* block);
    bool IsExposed(Vector3 pos);
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnRenderBlock();
    void OnRenderProgress();
    void OnRenderProgressZephyr();

    void OnRender() override;
    void StartBreaking(Vector3 blockPos, int blockFace, bool &outBool);
    void DestroyBlock(Vector3 blockPos, int blockFace);
    bool FindFunnyBlock(std::vector<BlockPosPair> blocks, BlockPosPair &pair, float v1, float v2);

    // Define macro for adddebugmessage
    /*
    *#ifndef __RELEASEFREE__
    if (Debug.Enabled) {
    Logger::Write("Regen", str, Logger::Debug);
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player) player->addChatMessage("&7[Regen Debug] " + str + " &r", true);
    }
    #endif*/
#ifndef __RELEASEFREE__
#define REGEN_AddDebugMessage(str) \
    if (Debug.Enabled) { \
        Logger::Write("Regen", (str), Logger::Debug); \
        auto player = Minecraft::ClientInstance->ThePlayer(); \
        std::string dastr = (str); \
        if (player) player->addChatMessage("&7[Regen Debug] " + dastr + " &r", true); \
    }
#else
#define REGEN_AddDebugMessage(str)
#endif


};
