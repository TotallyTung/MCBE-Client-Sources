#pragma once
//
// Created by vastrakai on 9/16/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Breaker : public ModuleBase<Breaker> {
public:
    BoolSetting TreasuresSetting = BoolSetting("Treasures", "Breaks treasures", true);
    BoolSetting BedsSetting = BoolSetting("Beds", "Breaks beds", true);
    NumberSetting Distance = NumberSetting("Distance", "Max distance to break blocks", 0.0f, 12.0f, 6.0f, 0.1f);
    NumberSetting BreakSpeed = NumberSetting("BreakSpeed", "Speed to break blocks", 0.0f, 1.0f, 1.0f, 0.1f);

    Breaker() : ModuleBase("Breaker", "Automatically breaks blocks", "World", 0, false) {
        Distance.Display = true;
        AddSetting(&TreasuresSetting);
        AddSetting(&BedsSetting);
        AddSetting(&Distance);
        AddSetting(&BreakSpeed);
    };

    static float BreakProgress;
    static bool IsBreaking;
    static bool ShouldRotate;
    static Actor* Target;
    static BlockPosPair CurrentBlock;

    void Reset();

    bool IsValidActor();

    void TickBed();
    void TickTreasure();

    void OnEnable() override;
    void OnDisable() override;
    void AddDebugMessage(const std::string& message);
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;



};