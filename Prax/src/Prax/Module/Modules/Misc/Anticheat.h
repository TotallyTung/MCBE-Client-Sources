//
// Created by vastrakai on 4/14/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class Anticheat : public ModuleBase<Anticheat> {
public:
    BoolSetting StrafeCheck = BoolSetting("Strafe Check", "Check if you are strafing", true);
    BoolSetting ExposedCheck = BoolSetting("Exposed Check", "Check if you are exposed", true);
    BoolSetting AntiImmobileCheck = BoolSetting("AntiImmobile Check", "Check if you are immobile", true);
    BoolSetting CombatCheck = BoolSetting("Combat Check", "pig check yaya :3", true);
    BoolSetting InvManagerCheck = BoolSetting("InvManager Check", "Check if you are using InvManager", true);
    BoolSetting HeadYaw = BoolSetting("HeadYaw Check", "Check if you are using HeadYaw", true);
    NumberSetting InvTimeframe = NumberSetting("InvManager Timeframe", "Timeframe to check for InvManager", 1000, 0, 500, 1000);
    NumberSetting MaxSwaps = NumberSetting("Max Swaps", "Max swaps in timeframe", 0, 4, 1, 1);

    Anticheat() : ModuleBase("Anticheat", "Clientside Anticheat, tells you when you flag", "Misc", 0, false) {
        AddSetting(&StrafeCheck);
        AddSetting(&ExposedCheck);
        AddSetting(&AntiImmobileCheck);
        AddSetting(&CombatCheck);
        AddSetting(&InvManagerCheck);
        AddSetting(&HeadYaw);
        AddSetting(&InvTimeframe);
        AddSetting(&MaxSwaps);
    }

    bool IsExposed(Vector3 pos);

    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnEnable() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;

};