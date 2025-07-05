#pragma once

#include <iomanip>
#include "NumberSetting.h"


void NumberSetting::HandleClick(bool left) {
    //Logger::Write(__FUNCTION__, Name + " HandleClick called with param " + (left ? "true" : "false"));
}

std::string NumberSetting::GetDisplayText() {
    std::ostringstream oss;
    oss << std::fixed << std::setprecision(2) << Value;
    std::string result = oss.str();

    // format the number to 2 decimal places ex: Value: 1.00
    std::string text = Name + ": " + result;

    return text;
}