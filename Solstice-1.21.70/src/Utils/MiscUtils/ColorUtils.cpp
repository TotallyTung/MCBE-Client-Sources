//
// Created by vastrakai on 7/1/2024.
//

#include "ColorUtils.hpp"

#include <regex>
#include <Features/FeatureManager.hpp>
#include <Features/Modules/Visual/Interface.hpp>

ImColor ColorUtils::Rainbow(float seconds, float saturation, float brightness, int index)
{
    float hue = ((NOW + index) % (int)(seconds * 1000)) / (float)(seconds * 1000);
    float r, g, b = 0;
    return ImColor::HSV(hue, saturation, brightness);
}

ImColor ColorUtils::LerpColors(float seconds, float index, std::vector<ImColor> colors, uint64_t ms) {
    if (colors.empty()) return { 255, 255, 255, 255};
    float time = 10000.0f / seconds;
    auto angle = static_cast<float>(((ms == 0 ? NOW : ms) + static_cast<int>(index)) % static_cast<int>(time));
    float segmentTime = time / colors.size();

    int segmentIndex = static_cast<int>(angle / segmentTime);
    float segmentIndexFloat = angle / segmentTime - segmentIndex;

    ImColor startColor = colors[segmentIndex];
    ImColor endColor = colors[(segmentIndex + 1) % colors.size()];
    return startColor.Lerp(endColor, segmentIndexFloat);
}

ImColor ColorUtils::getThemedColor(float index, uint64_t ms)
{
    auto daInterface = gFeatureManager->mModuleManager->getModule<Interface>();
    if (!daInterface) return { 255, 255, 255, 255 };

    auto theme = daInterface->mMode.mValue;
    auto colors = Interface::ColorThemes[theme];
    if (theme == Interface::Rainbow) return Rainbow(daInterface->mColorSpeed.mValue, daInterface->mSaturation.mValue, 1.f, index);
    else if (theme == Interface::Custom)
    {
        colors = daInterface->getCustomColors();
    }

    return LerpColors(daInterface->mColorSpeed.mValue, index, colors, ms);

}

std::string ColorUtils::removeColorCodes(const std::string& text)
{
    static std::regex colorCodeRegex("§[0-9a-z]");
    static std::string emptyString = "";
    return std::regex_replace(text, colorCodeRegex, emptyString);
}

