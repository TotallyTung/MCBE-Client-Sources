//
// Created by vastrakai on 10/28/2023.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>

class ShowGracePeriod : public ModuleBase<ShowGracePeriod> {
public:
    enum Style {
        Line = 0,
        Circle
    };

    ModeSetting Style = ModeSetting("Style", "The style of this visual.", { "Line", "Circle" }, Style::Line);
    ShowGracePeriod() : ModuleBase("ShowGracePeriod", "Shows the grace period some minigames on The Hive.", "Visual", 0, false) {
        this->AddSetting(&Style);

        RenderOnDisabled = true;
    };

    void OnTick() override;
    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;

};