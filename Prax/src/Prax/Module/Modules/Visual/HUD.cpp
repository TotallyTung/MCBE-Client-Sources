//
// Created by Flash on 9/13/2023.
//

#include "HUD.h"
#include <Prax/Utils/Render/ColorUtil.h>

#include "ArrayList.h"
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

ImColor HUD::GetColor(int index, uint64_t ms) {
    if (CustomTheme.Enabled) return ColorUtil::LerpColors(ColorUtil::GlobalRGBInfo.Time * 2, index * 10 * ColorUtil::GlobalRGBInfo.Separation, HUD::Colors, ms);

    switch (ColorTheme.SelectedIndex)
    {
    case (int)ColorTheme::Alternate:
        return ColorUtil::GetAlternateRainbow(index * ColorUtil::GlobalRGBInfo.Separation,
            ColorUtil::GlobalRGBInfo.Time,
            ColorUtil::GlobalRGBInfo.Saturation,
            ColorUtil::GlobalRGBInfo.Value);
    case (int)ColorTheme::Rainbow:
        return ColorUtil::GetDefaultRainbow(index * ColorUtil::GlobalRGBInfo.Separation,
            ColorUtil::GlobalRGBInfo.Time,
            ColorUtil::GlobalRGBInfo.Saturation,
            ColorUtil::GlobalRGBInfo.Value);
    default:
        return ColorUtil::LerpColors(ColorUtil::GlobalRGBInfo.Time * 2, index * 10 * ColorUtil::GlobalRGBInfo.Separation, HUD::Colors, ms);
    }
}

void HUD::OnRender() {

    // Set HUD Color Theme
    if (CustomTheme.Enabled) {
        HUD::Colors = {
            ImColor(static_cast<int>(Color1Red.Value), static_cast<int>(Color1Green.Value), static_cast<int>(Color1Blue.Value), 255),
            ImColor(static_cast<int>(Color2Red.Value), static_cast<int>(Color2Green.Value), static_cast<int>(Color2Blue.Value), 255)
        };
    } else
    {
        HUD::Colors = HUD::ColorThemes[ColorTheme.SelectedIndex];
    }
}