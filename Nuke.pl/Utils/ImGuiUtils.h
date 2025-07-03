#pragma once
#include "HMath.h"
#include "../ImGui/imgui.h"
#include "../ImFX/imfx.h"
#include "ColorUtil.h"

class ImGuiUtils {
public:
	static float fontSize;
	static void setDrawList(ImDrawList* d);
	static void drawText(Vec2 pos, const char* text, UIColor color, float textSize = 1.f, bool shadow = true);
	static float getTextWidth(const char* textStr, float textSize = 1.f);
	static float getTextHeight(float textSize = 1.f);
	static void drawRect(const Vec4& pos, UIColor color, float thickness = 1.f, float rounding = 0.f);
	static void drawRectFilled(const Vec4& pos, UIColor color, float rounding = 0.f);
	

	static void drawShadowRect(const Vec4& pos, UIColor color, float shadowThickness = 50.f, bool filled = true, float rounding = 0.f);
};