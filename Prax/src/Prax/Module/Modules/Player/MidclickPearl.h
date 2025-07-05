//
// Created by vastrakai on 12/4/2023.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class MidclickPearl : public ModuleBase<MidclickPearl> {
public:
    BoolSetting Swing = BoolSetting("Swing", "Swings arm when throwing pearl", true);

    MidclickPearl() : ModuleBase("MidclickPearl", "Throws a pearl when you middle click", "Player", 0, false)
    {
        PacketPriority = Priority::VeryLast;
        AddSetting(&Swing);
    }

    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacket(LoopbackPacketSender* lbps, Packet* packet, bool* canceled) override;

};
