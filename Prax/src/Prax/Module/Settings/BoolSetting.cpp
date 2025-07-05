#pragma once

#include "BoolSetting.h"

void BoolSetting::HandleClick(bool wasLeft)
{
    Enabled = !Enabled;
}

std::string BoolSetting::GetDisplayText() {
    return Enabled ? "true" : "false";
}



