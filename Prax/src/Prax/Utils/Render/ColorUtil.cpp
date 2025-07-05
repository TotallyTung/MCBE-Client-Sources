//
// Created by vastrakai on 8/24/2023.
//

#include "ColorUtil.h"
//d
RGBRenderInfo ColorUtil::GlobalRGBInfo;

ImColor ColorUtil::HSVtoImColor(float h, float s, float v)
{
    float r, g, b;
    ImGui::ColorConvertHSVtoRGB(h, s, v, r, g, b);
    return {r, g, b};
}


ImColor ColorUtil::GetDefaultRainbow(int index, float speed, float saturation, float value, int separation)
{
    // Clamp the speed to 1 - 100
    speed = std::clamp(speed, 1.0f, 100.0f);
    float hue = (float)((Misc::GetCurrentMs() + index * separation) % (int)(((int)speed) * 1000)) / (float)(((int)speed) * 1000);
    return HSVtoImColor(hue, saturation, value);
}

ImColor ColorUtil::GetAlternateRainbow(int index, float speed, float saturation, float value, int separation)
{
    // Clamp the speed to 1 - 100
    speed = std::clamp(speed, 1.0f, 100.0f);
    float hue = (float)((Misc::GetCurrentMs() + index * separation) % (int)(((int)speed) * 1000)) / (float)(((int)speed) * 1000);
    if (hue > 0.5) hue = 0.5F - (hue - 0.5f);
    hue += 0.5f;
    return HSVtoImColor(hue, saturation, value);
}

// From tha brit
ImColor ColorUtil::LerpColors(float seconds, float index, ImColor start, ImColor end) {
    float time = 10000.0f / seconds;
    auto angle = static_cast<float>((Misc::GetCurrentMs() + static_cast<int>(index)) % static_cast<int>(time));
    float halfTime = time / 2;

    return start.Lerp(end, angle <= halfTime ? angle / halfTime : 1.0f - ((angle - halfTime) / halfTime));
}

ImColor ColorUtil::LerpColors(float seconds, float index, std::vector<ImColor> colors, uint64_t ms) {
    if (colors.empty()) return ImColor(255, 255, 255, 255);
    float time = 10000.0f / seconds;
    auto angle = static_cast<float>(((ms == 0 ? Misc::GetCurrentMs() : ms) + static_cast<int>(index)) % static_cast<int>(time));
    float segmentTime = time / colors.size();

    int segmentIndex = static_cast<int>(angle / segmentTime);
    float segmentIndexFloat = angle / segmentTime - segmentIndex;

    ImColor startColor = colors[segmentIndex];
    ImColor endColor = colors[(segmentIndex + 1) % colors.size()];
    return startColor.Lerp(endColor, segmentIndexFloat);
}


// Add callback to update colors on render
/*void ColorUtil::AddColorUpdateCallback(std::function<void()> callback) {
    ColorUtil::GlobalRGBInfo.ColorUpdateCallbacks.push_back(callback);
}*/

void ColorUtil::OnRender() {
    for (auto callback : ColorUtil::GlobalRGBInfo.ColorUpdateCallbacks) {
        callback();
    }
}