// Created by Tony on 2024-10-10 10:01:54

// Watermark.cpp
#include "Watermark.h"
#include "../../../Client.h"

Watermark::Watermark() : Module("Watermark", "Shows the client watermark", Category::CLIENT) {
    addSlider<float>("Position", "Distance from the corner", ValueType::FLOAT_T, &positionFromCorner, 0.f, 100.f);
}

void Watermark::onImGuiRender(ImDrawList* drawList) {
  
    if (!mc.getClientInstance()->minecraftGame->canUseKeys) return;

    ImGuiIO& io = ImGui::GetIO();
    Vec2<float> renderPosition(positionFromCorner, positionFromCorner);

    static Colors* colorsMod = (Colors*)client->moduleMgr->getModule("Colors");

    float time = (float)ImGui::GetTime() * 2.0f;
    std::vector<UIColor> colors;

    DrawGradientText(drawList, watermarkText, renderPosition, watermarkSize, time);
    DrawGlowText(drawList, watermarkText, renderPosition, watermarkSize, colors);
}

void Watermark::DrawGradientText(ImDrawList* drawList, const std::string& str, const Vec2<float>& pos, float size, float timeInSeconds) {
    Vec2<float> currentPos = pos;
    std::vector<UIColor> colors;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        UIColor color = ColorUtils::getRainbowColor(timeInSeconds, 1.0f, 1.0f, i * 100);
        colors.push_back(color);

        float charWidth = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str()).x;

        // Draw shadow
        ImGuiUtils::drawText(currentPos.add(Vec2<float>(1, 1)), std::string(1, c), UIColor(0, 0, 0, 128), size);

        // Draw character
        ImGuiUtils::drawText(currentPos.add(Vec2<float>(0, 0)), std::string(1, c), color, size);

        currentPos.x += charWidth;
    }
}

void Watermark::DrawGlowText(ImDrawList* drawList, const std::string& str, const Vec2<float>& pos, float size, const std::vector<UIColor>& colors) {
    Vec2<float> currentPos = pos;

    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        UIColor color = colors[i];

        float charWidth = ImGui::GetFont()->CalcTextSizeA(size, FLT_MAX, 0.0f, std::string(1, c).c_str()).x;
        float charHeight = ImGui::GetFont()->FontSize * (size / ImGui::GetFontSize());

        // Add glow
        drawList->AddCircle(ImVec2(currentPos.x + (charWidth / 2), currentPos.y + (charHeight / 2)),
            size / 3, color.toImColor(), 12, 2.0f);

        currentPos.x += charWidth;
    }
}