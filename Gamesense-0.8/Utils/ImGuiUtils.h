#pragma once
#include "../include/imgui/imgui.h"
#include "../SDK/CClientInstance.h"
#include "../SDK/CMinecraftUIRenderContext.h"
#include "../Utils/HMath.h"

class ImGuiUtils {
public:
	static void ctx(C_MinecraftUIRenderContext* ctx, C_GuiData* guiData);
	static void renderText(ImDrawList* d, ImVec2 pos, const char* text, ImColor color, float textSize = 40.f);
	static void renderGradientText(ImDrawList* d, ImVec2 pos, const char* text, float textSize = 40.f, int index = 0, float opacity = 1.f);
	// NOT FOR NOW
	static void glowTextBackground(ImDrawList* d, ImVec2 pos, const char* text, float textSize = 40.f, int index = 0);

	static float getTextWidth(ImDrawList* d, const char* text, float textSize = 40.f);
	static float getTextHeight(ImDrawList* d, const char* text, float textSize = 40.f);

	static void pushClipRect(ImDrawList* d, ImVec4 pos);
	static void popClipRect(ImDrawList* d, ImVec4 pos);

	static void renderRect(ImDrawList* d, ImVec4 pos, ImColor color, float thickness = 1.f, float rounding = 0.f, ImDrawFlags flag = 0);
	static void renderRectFilled(ImDrawList* d, ImVec4 pos, ImColor color, float rounding = 0.f, ImDrawFlags flag = 0);
	static void renderRectShadow(ImDrawList* d, ImVec4 pos, ImColor color, float thickness = 5.f, float rounding = 0.f, bool fill = false, ImVec2 offset = ImVec2(0, 0));

	static void renderCircle(ImDrawList* d, ImVec2 pos, ImColor color, float radius = 1.f, float thickness = 1.f, int num_seg = 0);
	static void renderCircleFilled(ImDrawList* d, ImVec2 pos, ImColor color, float radius = 1.f, int num_seg = 0);
	static void renderCircleShadow(ImDrawList* d, ImVec2 pos, ImColor color, float radius = 1.f, float thickness = 5.f, bool fill = false, int num_seg = 12, ImVec2 offset = ImVec2(0, 0));

	static void renderBox(ImDrawList* d, vec3_t lower, vec3_t upper, float linewidth = 1.f, float opacity = 1.f, bool outline = false);
};