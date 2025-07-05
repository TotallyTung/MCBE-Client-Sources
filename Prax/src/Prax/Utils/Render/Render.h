//
// Created by Flash on 9/13/2023.
//

#pragma once

#include <string>
#include <imgui.h>
#include <Prax/Utils/Render/ColorUtil.h>

struct TextRenderInfo {
    std::string Text;
    ImVec2 Pos = { 0, 0 };
    float Size = 0.f;
    ImColor Color = { 0, 0, 0, 0 };
    ImColor Color2 = { 0, 0, 0, 0 };
    bool RenderShadow = false;
    bool UsesRGB = false;
    bool UsesGradient = false;
    RGBRenderInfo RGBInfo = ColorUtil::GlobalRGBInfo;
};

struct TextRenderInfo2 {
    std::string Text;
    ImVec2 Pos = { 0, 0 };
    float Size = 0.f;
    std::vector<ImColor> Colors;
    bool RenderShadow = false;
    bool UsesRGB = false;
    bool UsesGradient = false;
    RGBRenderInfo RGBInfo = ColorUtil::GlobalRGBInfo;
};



class Render {
public:
    static ImDrawList* DrawList;
    static inline FrameTransform Transform;

    static void PushDrawList(ImDrawList* drawList);
    static void PopDrawList();
    static void RenderSmoothScrolledText(std::string str, ImVec2 pos, float size, std::vector<ImColor>& color, bool shadow, std::map<int, float>& indexOffsetMap, float animationPerc);
    static void RenderSmoothScrolledText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow, std::map<int, float>& indexOffsetMap, float animationPerc);
    static TextRenderInfo DrawRainbowText(std::string str, ImVec2 pos, float size, float timeInSeconds = 4, float saturation = 0.75f, float value = 1, float separation = 10, bool shadow = true);
    static TextRenderInfo DrawRainbowText(const TextRenderInfo& info);
    static TextRenderInfo DrawGradientText(std::string str, ImVec2 pos, float size, float timeInSeconds, ImColor color1, ImColor color2, bool shadow = true);
    static TextRenderInfo DrawGradientText(const TextRenderInfo& info);
    static TextRenderInfo DrawShadowText(const std::string& str, ImVec2 pos, float size, ImColor color, bool shadowOnly = false);
    static TextRenderInfo DrawShadowText(const TextRenderInfo& info, bool shadow = true);
    static TextRenderInfo RenderText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow);
    static TextRenderInfo RenderText(const TextRenderInfo &info);
    static TextRenderInfo RenderTextColored(std::string str, ImVec2 pos, float size, ImColor color, bool shadow);
    static TextRenderInfo DrawGlowText(std::string str, ImVec2 pos, float size, ImColor color, bool shadow = true);
    static TextRenderInfo DrawGlowText(std::string str, ImVec2 pos, float size, ImColor color, int thickness, bool shadow = true);
    static TextRenderInfo DrawGlowText(const TextRenderInfo& info);
    static void FillScreenWithColor(ImColor color);
    static void FillScreenWithBlur(float strength);
    static void FillAreaWithBlur(float strength, ImVec4 area);
    static void DrawLine(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness);
    static void DrawGlowLine(ImVec2 pos1, ImVec2 pos2, ImColor color, float thickness);
    static void DrawRect(ImVec2 pos1, ImVec2 pos2, ImColor color);
    static void DrawImageFromURL(const std::string& url, ImVec2 pos, ImVec2 size);
    static void DrawRoundedImageFromURL(const std::string& url, ImVec2 pos, ImVec2 size, float rounding, ImColor color);
    static void RenderTextWithBG(const std::string &str, ImVec2 pos, float size, ImColor color, ImColor bgColor, bool shadow, float textPadding);
    static void DrawBoxAroundBlockOutline(struct Vector3 blockPos, ImColor color);
    static void DrawBoxAroundBlock(struct Vector3 blockPos, ImColor borderColor, ImColor filledColor);
    static void Spinner(const char* label, ImVec2 pos, float radius, int thickness, const ImU32& color);
    static void LoadingSpinner(float radius, ImVec2 position, float duration);
    static void DrawCircleLoader(ImVec2 pos, ImColor color, float radius, int segments, float timeLeft, float total, float thickness = 2.0f);
    static void DrawGradientText(std::string str, ImVec2 pos, float size, float timeInSeconds, std::vector<ImColor>* colors, bool shadow);
    static void DrawGlowText(std::string str, ImVec2 pos, float size, std::vector<ImColor> color, bool shadow);
};
