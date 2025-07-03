//
// Created by vastrakai on 6/28/2024.
//

#include "Module.hpp"

#include <Features/FeatureManager.hpp>
#include "Setting.hpp"
#include "Visual/Interface.hpp"

static Interface* interfaceMod;


std::string Module::getSettingDisplayText()
{
    if (!interfaceMod) interfaceMod = gFeatureManager->mModuleManager->getModule<Interface>();
    auto style = static_cast<NamingStyle>(interfaceMod->mNamingStyle.mValue);
    if (style == Lowercase || style == LowercaseSpaced)
        return StringUtils::toLower(getSettingDisplay());
    return getSettingDisplay();
}

std::string& Module::getName()
{
    if (!interfaceMod) interfaceMod = gFeatureManager->mModuleManager->getModule<Interface>();
    auto style = static_cast<NamingStyle>(interfaceMod->mNamingStyle.mValue);
    return mNames[style];
}

void Module::setEnabled(bool enabled)
{
    bool prevEnabled = mEnabled;
    bool newEnabled = enabled;

    if (newEnabled && !prevEnabled)
    {
        auto holder = nes::make_holder<ModuleStateChangeEvent>(this, newEnabled, prevEnabled);
        spdlog::trace(mName + " triggered state change [new: " + std::to_string(newEnabled) + ", prev: " + std::to_string(prevEnabled) + "]");
        gFeatureManager->mDispatcher->trigger(holder);
        if (holder->isCancelled()) return;
    }
    if (!newEnabled && prevEnabled)
    {
        auto holder = nes::make_holder<ModuleStateChangeEvent>(this, newEnabled, prevEnabled);
        spdlog::trace(mName + " triggered state change [new: " + std::to_string(newEnabled) + ", prev: " + std::to_string(prevEnabled) + "]");
        gFeatureManager->mDispatcher->trigger(holder);
        if (holder->isCancelled()) return;
    }

    mWantedState = newEnabled;
}

void Module::toggle()
{
    setEnabled(!mEnabled);
}

void Module::enable()
{
    if (!mEnabled) setEnabled(true);
}

void Module::disable()
{
    if (mEnabled) setEnabled(false);
}

void Module::addSetting(Setting* setting)
{
    this->mSettings.emplace_back(setting);
}

std::string& Module::getCategory()
{
    return ModuleCategoryNames[static_cast<int>(mCategory)];
}

nlohmann::json Module::serialize()
{
    nlohmann::json j;
    j["name"] = mName;
    j["enabled"] = mEnabled;
    j["key"] = mKey;
    j["settings"] = nlohmann::json::array();
    for (const auto setting : mSettings)
    {
        if (setting->mType == SettingType::Enum)
        {
            auto* enumSetting = reinterpret_cast<EnumSetting*>(setting);
            // use the serialize() from the enum setting
            j["settings"].push_back(enumSetting->serialize());
        }
        else if (setting->mType == SettingType::Number)
        {
            auto* numberSetting = reinterpret_cast<NumberSetting*>(setting);
            // use the serialize() from the number setting
            j["settings"].push_back(numberSetting->serialize());
        }
        else if (setting->mType == SettingType::Bool)
        {
            auto* boolSetting = reinterpret_cast<BoolSetting*>(setting);
            // use the serialize() from the bool setting
            j["settings"].push_back(boolSetting->serialize());
        }
        else if (setting->mType == SettingType::Color)
        {
            auto* colorSetting = reinterpret_cast<ColorSetting*>(setting);
            // use the serialize() from the color setting
            j["settings"].push_back(colorSetting->serialize());
        }
    }
    return j;
}

Setting* Module::getSetting(const std::string& string)
{
    for (auto setting : mSettings)
    {
        if (setting->mName == string)
        {
            return setting;
        }
    }
    // Go through again
    for (auto setting : mSettings)
    {
        std::string filteredName = StringUtils::toLower(setting->mName);
        std::erase_if(filteredName, [](char c) { return !std::isalnum(c); });

        if (filteredName == StringUtils::toLower(string))
        {
            return setting;
        }
    }
    return nullptr;
}
