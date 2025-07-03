#pragma once
//
// Created by vastrakai on 7/1/2024.
//

#include <chrono>

#define NOW std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count()

class ColorUtils {
public:
    static ImColor Rainbow(float seconds, float saturation, float brightness, int index);
    static ImColor LerpColors(float seconds, float index, std::vector<ImColor> colors, uint64_t ms = 0);
    static ImColor getThemedColor(float index, uint64_t ms = 0);
    static std::string removeColorCodes(const std::string& text);
};