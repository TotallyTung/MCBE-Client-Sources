#pragma once

#include "Setting.h"
#include "BoolSetting.h"

void Setting::SetHideOnClickGUI(bool *shownOnGUI)  {
    this->HideOnClickGUI = shownOnGUI;
}

bool Setting::IsVisible() {
    if (!ParentSetting) return true;
    return ParentSetting->Enabled;
}


