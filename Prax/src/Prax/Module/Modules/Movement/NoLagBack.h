#pragma once
//
// Created by vastrakai on 8/31/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class NoLagBack : public ModuleBase<NoLagBack> {
public:
    enum class Mode {
        Cancel,
        SilentAccept,
        RotateOnly
    };

    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Cancel", "Silent Accept", "Rotate Only"}, (int)Mode::Cancel);
    BoolSetting UseMaxDistance = BoolSetting("Use Max Distance", "Only accept packets if the distance is less than 5 blocks", false);
    NumberSetting MaxDistance = NumberSetting("Max Distance", "The max distance to accept packets", 0, 10, 5, 0.1);

    NoLagBack() : ModuleBase("NoLagBack", "Prevents the server from setting your position", "Movement")
    {
        AddSetting(&Mode);
        Mode.Display = true;
        AddSetting(&UseMaxDistance);
        AddSetting(&MaxDistance);
    }

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
};
