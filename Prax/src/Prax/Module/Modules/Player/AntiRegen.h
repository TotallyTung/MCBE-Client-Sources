#pragma once
//
// Created by vastrakai on 9/19/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>


class AntiRegen : public ModuleBase<AntiRegen> {
public:
#ifndef __RELEASEFREE__
    enum Mode {
        COVER_ON_DESTROY,
        ALWAYS_COVER
    };

    ModeSetting Mode = ModeSetting(xorstr_("Mode"), xorstr_("The mode to use"), { xorstr_("OnDestroy"), xorstr_("Always") }, 0);
    NumberSetting PlacesPerTick = NumberSetting(xorstr_("Places Per Tick"), xorstr_("The amount of blocks to place per tick (Only applies to Always mode!)"), 0, 20, 5.0f, 0.01);
    NumberSetting Distance = NumberSetting(xorstr_("Distance"), xorstr_("The max distance to mine blocks"), 0, 20, 5.0f, 0.01);
    NumberSetting Delay = NumberSetting(xorstr_("Delay"), xorstr_("The delay between start mining and placing blocks"), 0, 1.f, 0.4f, 0.01);
    BoolSetting PingCompensation = BoolSetting(xorstr_("Ping Compensation"), xorstr_("Compensate for ping"), true);
    BoolSetting RedstoneOnly = BoolSetting(xorstr_("Redstone Only"), xorstr_("Only place blocks over redstone"), true);
    BoolSetting IgnoreLocalBreak = BoolSetting(xorstr_("Ignore Local Break"), xorstr_("Ignore blocks being broken by you"), true);
    BoolSetting AbsorptionCheck = BoolSetting(xorstr_("Absorption Check"), xorstr_("Check if the player has absorption hearts"), true);
    BoolSetting IgnoreQueuedOres = BoolSetting(xorstr_("Ignore Queued Ores"), xorstr_("Ignore ores that are queued to be mined"), true);

    AntiRegen() : ModuleBase(xorstr_("AntiRegen"), xorstr_("Prevents people from gaining absorption hearts by placing blocks over redstone and potentially getting them ac banned"), xorstr_("Player"), 0, false) {
        AddSetting(&Mode);
        AddSetting(&PlacesPerTick);
        AddSetting(&Distance);
        AddSetting(&Delay);
        AddSetting(&PingCompensation);
        AddSetting(&RedstoneOnly);
        AddSetting(&IgnoreLocalBreak);
        AddSetting(&AbsorptionCheck);
        AddSetting(&IgnoreQueuedOres);
        Mode.Display = true;
        RenderOnDisabled = true;
        PacketPriority = Priority::VeryLast;
    }

    /* Static variables */
    static std::vector<std::pair<BlockPosPair, uint64_t>> BlockQueue;
    static bool ShouldRotate;
    static BlockPosPair CurrentBlock;
    static std::vector<std::pair<uint64_t, Vector3>> PlayerBlocks;
    static uint64_t LastPlaceMs;

    /* Module functions */
    void CoverBlock(BlockPosPair block);
    bool Tick();

    /* Module overrides */
    void OnTick() override;
    void OnEnable() override;
    void OnDisable() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
    void OnRender() override;
#endif
};
