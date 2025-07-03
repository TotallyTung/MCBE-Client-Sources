// Created by Tony on 2024-10-10 10:01:54

// Watermark.h
#pragma once
#include "../Module.h"
#include "../../../../Utils/ColorUtils.h"


class Watermark : public Module {
private:
    
    float positionFromCorner = 40.f;
    std::string watermarkText = "Melody Client";
    float watermarkSize = 50.f;

public:
    Watermark();
    virtual void onImGuiRender(ImDrawList* drawList) override;

private:
    void DrawGradientText(ImDrawList* drawList, const std::string& str, const Vec2<float>& pos, float size, float timeInSeconds);
    void DrawGlowText(ImDrawList* drawList, const std::string& str, const Vec2<float>& pos, float size, const std::vector<UIColor>& colors);
};