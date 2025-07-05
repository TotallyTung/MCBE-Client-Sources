#pragma once
//
// Created by vastrakai on 8/24/2023.
//

#include <imgui.h>
#include <Prax/Utils/Misc.h>
#include <Prax/Utils/Render/Font.h>
#include <algorithm>

struct RGBRenderInfo {
    float Time = 2.0f; // Time in seconds to go through the rainbow
    float Saturation = 0.75f;
    float Value = 1;
    float Separation = 10;

    // Add callback to update colors on render
    std::vector<std::function<void()>> ColorUpdateCallbacks;
};

class ColorUtil {
public:

    static RGBRenderInfo GlobalRGBInfo;

    static ImColor GetDefaultRainbow(int index, float speed, float saturation, float value, int separation = 10);
    static ImColor GetAlternateRainbow(int index, float speed, float saturation, float value, int separation = 10);
    static ImColor HSVtoImColor(float h, float s, float v);
    static ImColor LerpColors(float seconds, float index, ImColor start, ImColor end);
    static ImColor LerpColors(float seconds, float index, std::vector<ImColor> colors, uint64_t ms = 0);
    //static void AddColorUpdateCallback(std::function<void()> callback);
    static void OnRender();
};