//
// Created by Flash on 9/13/2023.
//

#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

class HUD : public ModuleBase<HUD>
{
public:
    enum class ColorTheme {
        Trans,
        FREEDOM,
        Flames,
        RedGreen,
        CottonCandy,
        Rainbow,
        Alternate
    };

    static inline std::map<int, std::vector<ImColor>> ColorThemes = {
        {(int)ColorTheme::Trans, {
            ImColor(91, 206, 250, 255),
            ImColor(245, 169, 184, 255),
            ImColor(255, 255, 255, 255),
            ImColor(245, 169, 184, 255),
        },
    },
    {(int)ColorTheme::FREEDOM, {
        ImColor(255, 0, 0, 255),
        ImColor(255, 255, 255, 255),
        ImColor(0, 0, 255, 255),
    }},
    {(int)ColorTheme::Flames, {
        ImColor(255, 0, 0, 255),
        ImColor(255, 165, 0, 255),
        ImColor(255, 255, 0, 255),
    }},
    {(int)ColorTheme::RedGreen, {
        ImColor(255, 0, 0, 255),
        ImColor(0, 255, 0, 255),
    }},
    {(int)ColorTheme::CottonCandy, {
        ImColor(255, 105, 180, 255),
        ImColor(255, 182, 193, 255),
        ImColor(135, 206, 250, 255),
    }},
    {(int)ColorTheme::Rainbow, {}},
        {(int)ColorTheme::Alternate, {}}
};

    static inline std::vector<ImColor> Colors = ColorThemes[(int)ColorTheme::Trans];

    static inline ModeSetting ColorTheme = ModeSetting("Color Theme", "The color theme to use for the HUD", {"Trans", "FREEDOM", "Flames","Red-Green", "Cotton Candy", "Rainbow", "Alternate"}, 0);
    static inline BoolSetting CustomTheme = BoolSetting("Custom Theme", "Use a custom color theme", false);
    NumberSetting Color1Red = NumberSetting("Color 1 Red", "The red value of the first color", 0, 255, 0, 1, &CustomTheme);
    NumberSetting Color1Green = NumberSetting("Color 1 Green", "The green value of the first color", 0, 255, 0, 1, &CustomTheme);
    NumberSetting Color1Blue = NumberSetting("Color 1 Blue", "The blue value of the first color", 0, 255, 0, 1, &CustomTheme);
    NumberSetting Color2Red = NumberSetting("Color 2 Red", "The red value of the second color", 0, 255, 255, 1, &CustomTheme);
    NumberSetting Color2Green = NumberSetting("Color 2 Green", "The green value of the second color", 0, 255, 255, 1, &CustomTheme);
    NumberSetting Color2Blue = NumberSetting("Color 2 Blue", "The blue value of the second color", 0, 255, 255, 1, &CustomTheme);
    BoolSetting ToggleNotifications = BoolSetting("Toggle Notifications", "Show notifications when modules are toggled", true);
    ModeSetting NotificationStyle = ModeSetting("Notification Style", "The style of toggle notifications to show", {"Flash", "Aether", "" }, 0);
    BoolSetting ShowHiddenModules = BoolSetting("Show Hidden Modules", "Show modules that are hidden in the arraylist in the Notifications", false);
    NumberSetting SelectedSlotEasing = NumberSetting("Selected Slot Easing", "How fast the selected slot moves", 0.01f, 40.f, 10.f, 0.01f);

    HUD() : ModuleBase("HUD", "Shows the clients Heads-Up-Display", "Visual", 0, true) {
        AddSetting(&ColorTheme);
        AddSetting(&CustomTheme);
        AddSetting(&Color1Red);
        AddSetting(&Color1Green);
        AddSetting(&Color1Blue);
        AddSetting(&Color2Red);
        AddSetting(&Color2Green);
        AddSetting(&Color2Blue);
        AddSetting(&ToggleNotifications);
        AddSetting(&NotificationStyle);
        AddSetting(&ShowHiddenModules);
        AddSetting(&SelectedSlotEasing);
    }

    static ImColor GetColor(int index, uint64_t ms = 0);
    void OnRender() override;
};
