#pragma once
//
// Created by vastrakai on 9/22/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <memory>

class Trail : public ModuleBase<Trail> {
public:

    NumberSetting Delay = NumberSetting("Delay", "How long to wait before drawing the next trail segment", 1, 100, 20, 0.1);
    NumberSetting Size = NumberSetting("Size", "How big the trail segment should be", 1, 100, 10, 0.1);
    NumberSetting ScaleInTime = NumberSetting("ScaleInTime", "How long it takes for the trail to scale in", 0, 1, 0.80, 0.1);
    NumberSetting FadeTime = NumberSetting("FadeTime", "How long it takes for the trail to fade away", 1, 20, 2, 0.1);
    BoolSetting RenderOnOthers = BoolSetting("RenderOnOthers", "Render the trail on other players", false);
    BoolSetting RenderOnSelf = BoolSetting("RenderOnSelf", "Render the trail on yourself", true);
    BoolSetting RGB = BoolSetting("RGB", "Enable RGB", false);

    Trail() : ModuleBase("Trail", "Draws a trail behind you", "Visual", 0, false) {
        AddSetting(&Delay);
        AddSetting(&Size);
        AddSetting(&ScaleInTime);
        AddSetting(&FadeTime);
        AddSetting(&RenderOnOthers);
        AddSetting(&RenderOnSelf);
        AddSetting(&RGB);
    };


    void OnRender() override;
    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
    void OnEnable() override;
    void OnDisable() override;
};