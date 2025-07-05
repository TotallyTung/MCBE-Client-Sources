//
// Created by Flash on 7/30/2023.
//
#include "Set.h"

#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/Utils/String.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>

void Set::OnRun(std::vector<std::string> args) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;

    if (args.size() < 3) {
        ShowUsage();
        return;
    }

    auto module = ModuleManager::FindModuleByName(args[0]);

    if (!module) {
        AddCommandMessage("Module not found!");
        return;
    }

    auto setting = module->GetSettingByName(args[1]);

    if (!setting) {
        AddCommandMessage("Setting not found!");
        return;
    }

    std::string value = String::ToLower(args[2]);

    SettingType type = setting->Type;
    int index = 0;
    if (type == SettingType::Bool) {
        // Check if the value is true or false or anything that
        // corresponds to true or false like 1 or 0
        bool finalValue = false;
        if (value == "true" || value == "1" || value == "on") {
            finalValue = true;
        } else if (value == "false" || value == "0" || value == "off") {
            finalValue = false;
        } else {
            AddCommandMessage("Invalid value!");
            return;
        }
        BoolSetting *boolSetting = (BoolSetting *) setting;
        boolSetting->Enabled = finalValue;
        AddCommandMessage("Set " + setting->Name + " to " + value);
    }
    else if (type == SettingType::Number){
        // Check if the value is a number
        if (!String::IsNumber(value)) {
            AddCommandMessage("Invalid value!");
            return;
        }
        NumberSetting *numberSetting = (NumberSetting *) setting;
        numberSetting->Value = std::stof(value);
        AddCommandMessage("Set " + setting->Name + " to " + value + " in module " + module->Name);
    }
    else if (type == SettingType::Mode){
        // Check if the value is a mode in the setting
        ModeSetting *modeSetting = (ModeSetting *) setting;
        int index = modeSetting->GetIndexByName(value);
        if (index == -1) {
            AddCommandMessage("Invalid value!");
            return;
        }

        modeSetting->SelectedIndex = index;
        AddCommandMessage("Set " + setting->Name + " to " + value + " in module " + module->Name);
    }
    else {
        AddCommandMessage("Unsupported setting type for this command!");
        return;
    }
}
