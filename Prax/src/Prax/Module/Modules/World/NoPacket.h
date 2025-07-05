//
// Created by Flash on 7/11/2023.
//

#pragma once


#include <Prax/Module/Settings/BoolSetting.h>
#include "Prax/Module/Module.h"

class NoPacket : public ModuleBase<NoPacket> {
public:
    BoolSetting CancelMovingPackets = BoolSetting("Moving", "Whether or not to cancel packets that send your pos to the server", true);
    BoolSetting VisualizeSetting = BoolSetting("Visualize", "Visualize the current and last position", true);
    BoolSetting RenderCurrentAndLastPosSetting = BoolSetting("Render current pos", "Render the current and last position if enabled, \n"
                                                                                         "Otherwise, only render the last pos", false);
    static Vector3 LastPos;

    NoPacket() : ModuleBase("NoPacket", "Prevents packets from being sent to the server", "Player", 0, false) {
        AddSetting(&CancelMovingPackets);
        AddSetting(&VisualizeSetting);
        AddSetting(&RenderCurrentAndLastPosSetting);
    }
    void OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) override;
    void OnDisable() override;
    void OnEnable() override;
};