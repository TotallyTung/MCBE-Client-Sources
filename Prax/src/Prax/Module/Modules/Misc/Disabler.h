#pragma once
//
// Created by vastrakai on 8/27/2023.
//


#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include "src/Prax/Module/Module.h"
#include <Prax/SDK/Game/Network/Other/NetworkStackLatencyPacket.h>


class Disabler : public ModuleBase<Disabler> {
public:

    enum DisablerMode {
        NukkitLagback,
        NetherGames,
        Sentinel,
        BdsPredictions,
        Custom
    };

    ModeSetting DisablerMode = ModeSetting("Mode", "The mode to use", {"NukkitLagback","NetherGames", "Sentinel", "BdsPredictions", "Custom"}, 0);
    NumberSetting VelX = NumberSetting("VelX", "The velocity to set on the x axis", -2, 2, 0, 1);
    NumberSetting VelY = NumberSetting("VelY", "The velocity to set on the y axis", -2, 2, -0.0784000015258789, 1);
    NumberSetting VelZ = NumberSetting("VelZ", "The velocity to set on the z axis", -2, 2, 0, 1);

    
    Disabler() : ModuleBase("Disabler", "Attempts to disable anti-cheat checks", "Misc", 0, false) {
        PacketPriority = Priority::AbsoluteLast;
        DisablerMode.Display = true;
        AddSetting(&DisablerMode);
        AddSetting(&VelX);
        AddSetting(&VelY);
        AddSetting(&VelZ);
    };

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel);

};