#pragma once

#include "Module.h"
#include "ModuleManager.h"
#include <Prax/Utils/String.h>

#include <string>
#include <Prax/Config/ConfigManager.h>
#include <src/Prax/Module/Modules/Misc/ToggleSounds.h>
#include <src/Prax/Module/Modules/Visual/HUD.h>
#include <src/Prax/UI/Notifications.h>

Module::Module(std::string const& name,
        std::string_view const& description,
std::string_view const& category = "misc",
int keyId,
bool enabled) {
    this->Name = name.data();
    this->Enabled = enabled;
    this->Description = description;
    this->Category = category;
    this->KeyId = keyId;

    Logger::Write("Module", "Initialized Module: " + this->Name, Logger::LogType::Debug);
}

void Module::AddSetting(Setting* setting)
{

    Settings.push_back(setting);
}

std::string Module::GetSettingDisplayText() {
    std::string text = "";
    for (auto setting : Settings) {
        if (setting->Display && setting->Type == SettingType::Mode) {
            // If its a mode get the current mode
            auto modeSetting = reinterpret_cast<ModeSetting*>(setting);
            text += modeSetting->GetDisplayTextNoName();
        }
        break;
    }
    return text;
}

void Module::Toggle() {
    if (Enabled) {
        OnDisable();

    } else {
        OnEnable();

    }

    Enabled = !Enabled;

    if (ConfigManager::Loading) return;

    static auto* hud = ModuleManager::GetModule<HUD>();
    bool hide = hud->ShowHiddenModules.Enabled ? false : HideInArraylist;
    if (Name != "ClickGui" && Name != "Zoom" && !hide) {
        if (hud->ToggleNotifications.Enabled) {
            std::string message = Name + " was " + (Enabled ? "enabled" : "disabled");
            Notifications::Notify(Name, message, Enabled ? NotificationType::Enable : NotificationType::Disable, 2.5f);
        }
    }




    // if there is a better way of doing this please tell me
    static auto* toggleSounds = ModuleManager::GetModule<ToggleSounds>();
    if (toggleSounds->Enabled && Name != "Zoom")
        toggleSounds->OnToggle(Enabled);
}

void Module::SetToggleState(bool enable) {
    if (enable && !Enabled || !enable && Enabled)
        Toggle();
}

void Module::OnInitialize()
{

}
void Module::OnEject()
{

}
void Module::OnTick()
{

}
void Module::OnClientTick()
{

}
void Module::OnEnable()
{

}
void Module::OnDisable()
{

}
void Module::OnRender()
{

}
void Module::OnTessellate()
{

}
void Module::OnSettingRender()
{

}

Setting *Module::GetSettingByName(std::string name) {
    for (auto setting : Settings) {
        std::string settingName = setting->Name;
        // Remove any spaces from the setting name
        settingName.erase(std::remove(settingName.begin(), settingName.end(), ' '), settingName.end());
        // Same with the name we're searching for
        name.erase(std::remove(name.begin(), name.end(), ' '), name.end());

        if (String::ToLower(settingName) == String::ToLower(name)) {
            return setting;
        }
    }
    return nullptr;
}
