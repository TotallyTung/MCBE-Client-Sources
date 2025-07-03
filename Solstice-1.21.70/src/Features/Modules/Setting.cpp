//
// Created by vastrakai on 6/30/2024.
//

#include "Setting.hpp"

bool Setting::parse(const std::string& value)
{
    if (mType == SettingType::Bool)
    {
        if (value == "true" || value == "1")
        {
            static_cast<BoolSetting*>(this)->mValue = true;
            return true;
        }
        if (value == "false" || value == "0")
        {
            static_cast<BoolSetting*>(this)->mValue = false;
            return true;
        }
        return false;
    }
    if (mType == SettingType::Number)
    {
        try
        {
            reinterpret_cast<NumberSetting*>(this)->mValue = std::stof(value);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
    if (mType == SettingType::Enum)
    {
        auto* enumSetting = reinterpret_cast<EnumSetting*>(this);
        int index = 0;
        for (auto enumValue : enumSetting->mValues)
        {
            enumValue = StringUtils::toLower(enumValue);
            std::erase_if(enumValue, [](char c) { return !std::isalnum(c); });
            std::string filteredValue = StringUtils::toLower(value);
            if (enumValue == filteredValue)
            {
                enumSetting->mValue = index;
                return true;
            }

            index++;
        }
        return false;
    }
    if (mType == SettingType::Color)
    {
        try
        {
            unsigned long val = std::stoul(value, nullptr, 16);
            reinterpret_cast<ColorSetting*>(this)->setFromHex(val);
            return true;
        }
        catch (const std::exception&)
        {
            return false;
        }
    }
    return false;
}
