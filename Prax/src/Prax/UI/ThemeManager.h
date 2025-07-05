//
// Created by Flash on 10/24/2023.
//


#pragma once


#include <nlohmann/json.hpp>
#include <Prax/Utils/Render/Font.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/Utils/System/FileSystem.h>
#include <Prax/Utils/Math.h>

struct ClickGuiColor {
    ImColor color;
    bool useRgb;

    // Constructors
    ClickGuiColor() {
        color = ImColor(0, 0, 0, 255);
        useRgb = false;
    }

    ClickGuiColor(ImColor color, bool useRgb = false) {
        this->color = color;
        this->useRgb = useRgb;
    }

    // Get shadow color
    ImColor GetShadowColor() {
        return ImColor(color.Value.x / 3,
            color.Value.y / 3,
            color.Value.z / 3,
            color.Value.w);
    }

    // convert to ImColor
    operator ImColor() {
        if (!useRgb) {
            return color;
        }
        color = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, ColorUtil::GlobalRGBInfo.Separation);
        return color;
    }

    operator ImVec4() {
        if (!useRgb) {
            return color;
        }
        color = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, ColorUtil::GlobalRGBInfo.Separation);
        return color;
    }

    // ImU32
    operator ImU32() {
        if (!useRgb) {
            return color;
        }
        color = ColorUtil::GetDefaultRainbow(0, ColorUtil::GlobalRGBInfo.Time, ColorUtil::GlobalRGBInfo.Saturation, ColorUtil::GlobalRGBInfo.Value, ColorUtil::GlobalRGBInfo.Separation);
        return color;
    }


    // convert from ImColor
    ClickGuiColor& operator=(const ImColor& color) {
        this->color = color;
        return *this;
    }

    // Convert to json object
    void to_json(nlohmann::json& j) const
    {
        j = nlohmann::json{

                        {"r", color.Value.x},
                        {"g", color.Value.y},
                        {"b", color.Value.z},
                        {"a", color.Value.w},
                        {"useRgb", useRgb}
        };
    }

    void from_json(const nlohmann::json& j)
    {
        if (j == nullptr) return;

        if (j.is_null()) return;

        // Make sure the json object is valid
        if (!j.contains("r") || !j.contains("g") || !j.contains("b") || !j.contains("a") || !j.contains("useRgb")) return;


        color = ImColor((float)j["r"], j["g"], j["b"], j["a"]);
        useRgb = j["useRgb"];
    }


};

/*
 *{
    "BorderSize": 0,
    "ButtonTextAlign": [
      0.5,
      0.5
    ],
    "CategoryFontSize": 30.48900032043457,
    "EyeIconOffColor": {
      "a": 1,
      "b": 0.5882353186607361,
      "g": 0.5882353186607361,
      "r": 0.5882353186607361,
      "useRgb": false
    },
    "EyeIconOnColor": {
      "a": 1,
      "b": 1,
      "g": 1,
      "r": 1,
      "useRgb": false
    },
    "GlobalRGBInfo": {
      "Saturation": 0.75,
      "Separation": 10,
      "Time": 2,
      "Value": 1
    },
    "HeaderHeight": 39.75199890136719,
    "IconFontSize": 15,
    "InnerItemSpacing": [
      10,
      3
    ],
    "ItemPadding": [
      0,
      0
    ],
    "ItemSpacing": [
      4,
      0
    ],
    "MaxWindowHeightPercent": 0.75,
    "ModuleFontSize": 23.04400062561035,
    "ModuleHeight": 32.369998931884766,
    "Rounding": 0,
    "SettingFontSize": 15,
    "SettingHeight": 37,
    "ToolTipFontSize": 20,
    "WindowTitleTextAlign": [
      0.5,
      0.5
    ],
    "backgroundColor1": {
      "a": 0,
      "b": 0,
      "g": 0,
      "r": 1,
      "useRgb": false
    },
    "backgroundColor2": {
      "a": 1,
      "b": 0.0470588281750679,
      "g": 0.0470588281750679,
      "r": 0.0470588281750679,
      "useRgb": false
    },
    "backgroundColor3": {
      "a": 1,
      "b": 0.21568629145622253,
      "g": 0.21568629145622253,
      "r": 0.21568629145622253,
      "useRgb": false
    },
    "blur": 10,
    "categoryTextColor": {
      "a": 1,
      "b": 1,
      "g": 1,
      "r": 1,
      "useRgb": false
    },
    "gobalOpacity": 0.800000011920929,
    "moduleDisabledTextColor": {
      "a": 1,
      "b": 0.5882353186607361,
      "g": 0.5882353186607361,
      "r": 0.5882353186607361,
      "useRgb": false
    },
    "moduleTextColor": {
      "a": 1,
      "b": 1,
      "g": 1,
      "r": 1,
      "useRgb": false
    },
    "primaryColor": {
      "a": 1,
      "b": 0.9803922176361084,
      "g": 0.3294117748737335,
      "r": 0.5176470875740051,
      "useRgb": false
    },
    "secondaryColor": {
      "a": 1,
      "b": 0.9803922176361084,
      "g": 0.3294117748737335,
      "r": 0.5176470875740051,
      "useRgb": false
    },
    "settingTextColor": {
      "a": 1,
      "b": 1,
      "g": 1,
      "r": 1,
      "useRgb": false
    },
    "shadowColor": {
      "a": 1,
      "b": 0,
      "g": 0,
      "r": 0,
      "useRgb": false
    },
    "toggleColor": {
      "a": 1,
      "b": 0.44999998807907104,
      "g": 0.6600000262260437,
      "r": 0.1599999964237213,
      "useRgb": false
    },
    "toggleDisabledColor": {
      "a": 1,
      "b": 0.75,
      "g": 0.75,
      "r": 0.75,
      "useRgb": false
    },
    "toggleHoverColor": {
      "a": 1,
      "b": 0.5699999928474426,
      "g": 1,
      "r": 0,
      "useRgb": false
    }
  }*/

struct Theme {
    RGBRenderInfo GlobalRGBInfo;

    // Colors
    /*ClickGuiColor backgroundColor1 = ImColor(0, 0, 0, 100);
    ClickGuiColor backgroundColor2 = ImColor(0, 0, 0, 50);

    ClickGuiColor backgroundColor3 = ImColor(55, 55, 55);

    ClickGuiColor primaryColor = ImColor(132, 84, 250);
    ClickGuiColor secondaryColor = ImColor(132, 84, 250);

    ClickGuiColor moduleTextColor = ImColor(255, 255, 255);
    ClickGuiColor moduleDisabledTextColor = ImColor(150, 150, 150);

    ClickGuiColor settingTextColor = ImColor(255, 255, 255);

    ClickGuiColor toggleColor = ImColor(0.16f, 0.66f, 0.45f, 1.0f);
    ClickGuiColor toggleDisabledColor = ImColor(0.75f, 0.75f, 0.75f, 1.0f);
    ClickGuiColor toggleHoverColor = ImColor(0.0f, 1.0f, 0.57f, 1.0f);

    ClickGuiColor shadowColor = ImColor(0.0f, 0.0f, 0.0f, 1.f);
    ClickGuiColor categoryTextColor = ImColor(255, 255, 255);

    ClickGuiColor EyeIconOnColor = ImColor(255, 255, 255);
    ClickGuiColor EyeIconOffColor = ImColor(150, 150, 150);

    ClickGuiColor BorderColor = ImColor(0, 0, 0, 0);

    ImVec2 WindowTitleTextAlign = ImVec2(0.5f, 0.5f);
    ImVec2 ButtonTextAlign = ImVec2(0.5f, 0.5f);

    ImVec2 ItemSpacing = ImVec2(4, 0);
    ImVec2 InnerItemSpacing = ImVec2(10, 3);
    ImVec2 ItemPadding = ImVec2(0, 0);

    // Misc
    float blur = 10;
    float gobalOpacity = 0.8f;
    float Rounding = 0.f;
    float BorderSize = 0.f;
    float MaxWindowHeightPercent = 0.75f;

    float HeaderHeight = 30;
    float ModuleHeight = 25;
    float SettingHeight = 37;

    float CategoryFontSize = 30;;
    float ModuleFontSize = 25;
    float SettingFontSize = 15;
    float ToolTipFontSize = 20.f;
    float IconFontSize = 15;*/

    // Use the same stuff as the theme json above
    ClickGuiColor backgroundColor1 = ImColor(1.f, 0.f, 0.f, 0.f);
    ClickGuiColor backgroundColor2 = ImColor(0.0470588281750679f, 0.0470588281750679f, 0.0470588281750679f, 1.f);
    ClickGuiColor backgroundColor3 = ImColor(0.21568629145622253f, 0.21568629145622253f, 0.21568629145622253f, 1.f);
    ClickGuiColor primaryColor = ImColor(0.5176470875740051f, 0.3294117748737335f, 0.9803922176361084f, 1.f);
    ClickGuiColor secondaryColor = ImColor(0.5176470875740051f, 0.3294117748737335f, 0.9803922176361084f, 1.f);
    ClickGuiColor moduleTextColor = ImColor(1.f, 1.f, 1.f, 1.f);
    ClickGuiColor moduleDisabledTextColor = ImColor(0.5882353186607361f, 0.5882353186607361f, 0.5882353186607361f, 1.f);
    ClickGuiColor settingTextColor = ImColor(1.f, 1.f, 1.f, 1.f);
    ClickGuiColor toggleColor = ImColor(0.1599999964237213f, 0.6600000262260437f, 0.44999998807907104f, 1.f);
    ClickGuiColor toggleDisabledColor = ImColor(0.75f, 0.75f, 0.75f, 1.f);
    ClickGuiColor toggleHoverColor = ImColor(0.f, 1.f, 0.5699999928474426f, 1.f);
    ClickGuiColor shadowColor = ImColor(0.f, 0.f, 0.f, 1.f);
    ClickGuiColor categoryTextColor = ImColor(1.f, 1.f, 1.f, 1.f);
    ClickGuiColor EyeIconOnColor = ImColor(1.f, 1.f, 1.f, 1.f);
    ClickGuiColor EyeIconOffColor = ImColor(0.5882353186607361f, 0.5882353186607361f, 0.5882353186607361f, 1.f);
    ClickGuiColor BorderColor = ImColor(0.f, 0.f, 0.f, 0.f);
    ImVec2 WindowTitleTextAlign = ImVec2(0.5f, 0.5f);
    ImVec2 ButtonTextAlign = ImVec2(0.5f, 0.5f);
    ImVec2 ItemSpacing = ImVec2(4.f, 0.f);
    ImVec2 InnerItemSpacing = ImVec2(10.f, 3.f);
    ImVec2 ItemPadding = ImVec2(0.f, 0.f);
    float blur = 10;
    float gobalOpacity = 0.800000011920929;
    float Rounding = 0;
    float BorderSize = 0;
    float MaxWindowHeightPercent = 0.75;
    float HeaderHeight = 39.75199890136719;
    float ModuleHeight = 32.369998931884766;
    float SettingHeight = 37;
    float CategoryFontSize = 30.48900032043457;
    float ModuleFontSize = 23.04400062561035;
    float SettingFontSize = 15;
    float ToolTipFontSize = 20;
    float IconFontSize = 15;



};

class ThemeManager {
public:

    static Theme* CurrentTheme;

    static void LoadPreferences(std::string name, Theme& preferences);
    static void SavePreferences(std::string name, Theme& preferences);
    static void OnRender(bool render);
    static void Init();
};