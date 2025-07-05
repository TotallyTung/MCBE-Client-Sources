//
// Created by Flash on 12/22/2023.
//


#pragma once

#include <string>
#include "Setting.h"
#include <imgui.h>

class ColorSetting : public Setting {
public:
    ColorSetting(const std::string& name, const std::string& description, const ImColor& value) : Setting::Setting(name, description, Color) {
        this->Value = value;
    };

    ImColor Value;
};