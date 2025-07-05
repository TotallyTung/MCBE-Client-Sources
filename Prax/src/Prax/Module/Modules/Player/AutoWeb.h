//
// Created by Flash on 2/4/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoWeb : public ModuleBase<AutoWeb> {
public:
    NumberSetting Distance = NumberSetting("Distance", "The max distance to target", 0, 10, 5.f, 0.1f);
    NumberSetting Prediction = NumberSetting("Prediction", "Predicts the players next pos and compensates for any ping you may have", 1, 10, 0.f, 0.1f);
    BoolSetting HotbarOnly = BoolSetting("Hotbar Only", "Whether or not to only use items in the hotbar", true);

    AutoWeb() : ModuleBase("AutoWeb", "Automatically places webs in front of enemies", "Player") {
        AddSetting(&Distance);
        AddSetting(&Prediction);
        AddSetting(&HotbarOnly);
    }


    void OnTick() override;
    Actor* GetClosestActor() const;
};
