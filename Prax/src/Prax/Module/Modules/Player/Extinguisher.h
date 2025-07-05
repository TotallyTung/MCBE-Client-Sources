//
// Created by vastrakai on 12/21/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class Extinguisher : public ModuleBase<Extinguisher> {
public:
    NumberSetting Range = NumberSetting("Range", "Range of the extinguisher", 0.f, 10.f, 10.f, 0.1f);

    Extinguisher() : ModuleBase("Extinguisher", "Destroys fire", "Player", 0, false) {
        AddSetting(&Range);
    }

    static bool Rotate;
    static Vector3 CurrentPosition;

    void OnTick() override;
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
};