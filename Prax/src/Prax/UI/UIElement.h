#pragma once

#include <imgui.h>
#include <string>
#include <vector>
#include <map>
#include <Prax/Module/ModuleManager.h>
#include "../Module/Module.h"
#include "../Module/Settings/Setting.h"
#include "ThemeManager.h"


class UIElement {
public:
    ImVec2 Position;
    ImVec2 Size;
    bool IsVisible;

    std::string Name;

    UIElement(std::string name) {
        this->Name = name;
        this->IsVisible = true;
    }
};


class SettingUIElement : public UIElement {
public:
    float SettingTextLength = 0;
    std::string Text = "";

    // Constructor
    SettingUIElement(Setting* setting, ImVec2 initialPos = ImVec2(0, 0)) : UIElement(setting->Name) {
        this->Text = setting->GetDisplayText();
        this->Position = initialPos;
        this->Size = ImVec2(0, 0);
        this->Name = setting->Name;
    }
};


class ModuleUIElement : public UIElement {
public:
    float ModuleTextLength = 0;
    std::string Name = "";

    int settingIndex = 0;

    std::map<Setting*, SettingUIElement*> Settings;

    // Constructor
    ModuleUIElement(Module* module, ImVec2 initialPos = ImVec2(0, 0)) : UIElement(module->Name) {

        for (auto setting : module->Settings) {
            SettingUIElement* settingUIElement = new SettingUIElement(setting);
            Settings.insert(std::pair<Setting*, SettingUIElement*>(setting, settingUIElement));
        }

        this->Position = initialPos;
        this->Size = ImVec2(0, 0);
        this->Name = module->Name;
    }
};


static float Length = 220;
static float Height = 20;
static float HeaderPadding = 5;

class CategoryUIElement : public UIElement {

public:


    CategoryUIElement(std::string name, ImVec2 initialPos,
                      std::vector<std::pair<Module*, ModuleUIElement*>> modules);


    bool firstRender = true;

    std::string Name;
    std::vector<std::pair<Module*, ModuleUIElement*>> Modules;

    int moduleIndex = 0;
    int settingIndex = 0;

    float GuiScale = 1.0f;

    struct {
        ImVec2 Position;
        ImVec2 Size;
        ImColor Color;
    } SelectedModule;

    float CurrentHeight = 0;

    float Padding = 5;


    float GetLength() const {
        return Length * GuiScale;
    }

    float GetHeight() const {
        return CurrentHeight * GuiScale;
    }

    float GetHeaderHeight() const {
        return ThemeManager::CurrentTheme->HeaderHeight * GuiScale;
    }

    float GetModuleHeight() const {
        return ThemeManager::CurrentTheme->ModuleHeight * GuiScale;
    }

    float GetSettingHeight() const {
        return ThemeManager::CurrentTheme->SettingHeight * GuiScale;
    }

    float GetPadding() const {
        return Padding * GuiScale;
    }

    float GetHeaderPadding() const {
        return HeaderPadding * GuiScale;
    }

    float GetModuleFontSize() const {
        return ThemeManager::CurrentTheme->ModuleFontSize * GuiScale;
    }

    float GetSettingFontSize() const {
        return ThemeManager::CurrentTheme->SettingFontSize * GuiScale;
    }

    float GetCategoryFontSize() const {
        return ThemeManager::CurrentTheme->CategoryFontSize * GuiScale;
    }

    float GetIconFontSize() const {
        return ThemeManager::CurrentTheme->IconFontSize * GuiScale;
    }



    float GetFullHeight(bool includePos = true) {
        float y = 0;
        if (includePos)
            y += Position.y;

        y += GetHeaderHeight();

        for (auto module : Modules) {
            y += GetModuleHeight();
            if (module.first->Expanded) {
                for (auto setting : module.second->Settings) {
                    if (!setting.first->IsVisible()) continue;
                    y += GetSettingHeight();
                }
            }
        }

        return y;
    }


    float GetCurrentRenderY(bool includePos = true) {
        float y = 0;
        if (includePos)
            y += Position.y;
        y += GetHeaderHeight();

        int i = 0;
        for (auto module : Modules) {
            if (moduleIndex == i) {
                break;
            }


            int j = 0;
            if (module.first->Expanded) {
                for (auto setting : module.second->Settings) {
                    if (!setting.first->IsVisible()) continue;
                    // This might break
                    if (settingIndex == j) {
                        break;
                    }

                    y += GetSettingHeight();
                    j++;
                }
            }

            y += GetModuleHeight();
            i++;
        }

        return y;
    }
};