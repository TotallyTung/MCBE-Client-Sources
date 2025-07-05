#pragma once

#include <string>

enum SettingType {
    Bool,
    Number,
    Mode,
    Color,
};

class Setting {
public:
    virtual ~Setting() = default;
    // Info



    std::string Name;
    std::string Description;
    SettingType Type;
    class BoolSetting* ParentSetting = nullptr; // The setting that this setting is a part of

    // Whether or not to show on the arraylist
    bool Display = false;

    bool* HideOnClickGUI = nullptr;

    // Constructor
    Setting(std::string name, std::string description, SettingType type, BoolSetting* parentSetting = nullptr) {
        this->Name = name;
        this->Description = description;
        this->Type = type;
        this->ParentSetting = parentSetting;
    }

    void SetHideOnClickGUI(bool* shownOnGUI);
    bool IsVisible();
    virtual std::string GetDisplayText() { return "no"; };
    virtual void HandleClick(bool wasLeft) {};
};