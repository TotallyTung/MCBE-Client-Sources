#pragma once
#include <string>
#include <vector>

#include "Setting.h"

class BoolSetting : public Setting {
public:
    bool Enabled;
    //Constructor
    BoolSetting(std::string name, std::string description, bool enabled, BoolSetting* parentSetting = nullptr) : Setting::Setting(name, description, Bool, parentSetting) {
        this->Enabled = enabled;
    };


    void HandleClick(bool wasLeft) override;
    std::string GetDisplayText() override;
};
