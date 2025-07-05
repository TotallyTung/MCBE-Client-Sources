//
// Created by Flash on 11/25/2023.
//


#pragma once

#include "Setting.h"
#include <vector>

#include "BoolSetting.h"

class SettingGroup {
    public:

    std::string Name;
    std::vector<Setting*> Settings;
    BoolSetting* OpenedSetting;

    SettingGroup(const std::string& name, BoolSetting* opened) {
        this->Name = name;
        this->OpenedSetting = opened;
    };

    void AddSetting(Setting* setting) {
        this->Settings.push_back(setting);
    };
};
