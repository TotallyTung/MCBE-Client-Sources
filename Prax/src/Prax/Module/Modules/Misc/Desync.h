//
// Created by vastrakai on 11/5/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>

class Desync : public ModuleBase<Desync> {
public:
    NumberSetting Ticks = NumberSetting("Ticks", "Ticks to skip", 0, 100, 20, 1);
    BoolSetting StrafeOnly = BoolSetting("Strafe Only", "Only desync while holding A and D", false);
    BoolSetting VisualizeSetting = BoolSetting("Visualize", "Visualize desync", false);

    Desync() : ModuleBase("Desync", "Test desync", "Misc", 0, false) {
        AddSetting(&Ticks);
        AddSetting(&StrafeOnly);
        AddSetting(&VisualizeSetting);
    }

    static std::vector<std::shared_ptr<PlayerAuthInputPacket>> QueuedPackets;
    static Vector3 LastPos;
    static bool Ticking;
    static bool Visualize;

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;


};