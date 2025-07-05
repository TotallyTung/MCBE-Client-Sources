// ReSharper disable CppClangTidyCppcoreguidelinesPreferMemberInitializer
#pragma once
#include <sstream>
#include <string>

#include "Setting.h"

class NumberSetting : public Setting {
public:
    float Min, Max, Value, Increment;
    //Constructor
    NumberSetting(std::string name, std::string description, float min, float max, float value, float increment, class BoolSetting* parentSetting = nullptr) : Setting::Setting(name, description, Number, parentSetting) {
        this->Min = min;
        this->Max = max;
        this->Value = value;
        this->Increment = increment;
    }

    void HandleClick(bool left) override;
    std::string GetDisplayText() override;

    operator float() const { return Value; }
    operator int() const { return static_cast<int>(Value); }
    operator double() const { return (double)Value; }
};
