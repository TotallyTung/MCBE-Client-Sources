#pragma once
#include "../ImGui/imgui.h"
#include "Math.h"
#include "ColorUtils.h"

class RenderUtils {
public:
	// Text
	static void drawText(ImDrawList* d, const Vec2<float>& pos, const char* textStr, const Color& color, const float& textSize, const bool& shadow = false, const float& glowRadius = 0.0f);
	static float getTextWidth(const char* textStr, const float& textSize);
	static float getTextHeight(const char* textStr, const float& textSize);
	static float getTextHeightReal(const char* textStr, const float& textSize);

	// Recentangle
	static void drawRect(ImDrawList* d, const Vec2<float>& posMin, const Vec2<float>& posMax, const Color& color, const float& thickness = 1.0f, const float& rounding = 0.0f);
	static void drawRectFilled(ImDrawList* d, const Vec2<float>& posMin, const Vec2<float>& posMax, const Color& color, const float& rounding = 0.0f);
	static void drawRectFilled(ImDrawList* d, const Vec4<float>& pos, const Color& color, const float& rounding = 0.0f);

	// Shadow
	static void drawShadowRect(ImDrawList* d, const Vec4<float>& pos, const Color& color, const float& shadowThickness, const bool& filled = true, const float& rounding = 0.0f);
};
