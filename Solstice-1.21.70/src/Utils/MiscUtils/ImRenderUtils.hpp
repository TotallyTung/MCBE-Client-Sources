#pragma once
#include <string>
#include <Features/Events/MouseEvent.hpp>
#include <Features/Modules/Setting.hpp>
//
// Created by vastrakai on 6/29/2024.
//

struct ImVec4;



class ImRenderUtils
{
public:
	static void addBlur(const ImVec4& pos, float strength, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList(), bool optimized = false);
	static void drawText(ImVec2 pos, const std::string& textStr, const ImColor& color, float textSize, float alpha, bool shadow = false, int index = 0, ImDrawList* d = ImGui::GetBackgroundDrawList());
	static void drawShadowText(ImDrawList* drawList, const std::string& text, ImVec2 pos, ImColor color, float fontSize, bool specializedMultiplier = false);
	static void fillRectangle(ImVec4 pos, const ImColor& color, float alpha, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList(), ImDrawFlags flags = 0);
	static void fillShadowRectangle(ImVec4 pos, const ImColor& color, float alpha, float thickness, ImDrawFlags flags, float radius = 0.f, ImDrawList* list = ImGui::GetBackgroundDrawList());
	static void fillShadowCircle(ImVec2 pos, float radius, const ImColor& color, float alpha, float thickness, ImDrawFlags flags, float segments = 12.f);
	static void drawRoundRect(ImVec4 pos, const ImDrawFlags& flags, float radius, const ImColor& color, float alpha, float lineWidth);
	static void fillGradientOpaqueRectangle(ImVec4 pos, const ImColor& firstColor, const ImColor& secondColor, float firstAlpha, float secondAlpha);
	static void fillRoundedGradientRectangle(ImVec4 pos, const ImColor& firstColor, const ImColor& secondColor, float radius, float firstAlpha, float secondAlpha, ImDrawFlags flags = 0);
	static void drawCheckMark(ImVec2 pos, float size, const ImColor& color, float alpha, float thickness = 2.0f);
	static void fillCircle(ImVec2 center, float radius, const ImColor& color, float alpha, int segments);
	static void drawColorPicker(ImVec4& color, ImVec4 canvas_pos);
	static ImVec2 getScreenSize();
	static bool isFullScreen();
	static float getTextHeightStr(std::string* textStr, float textSize);
	static float getTextWidth(std::string* textStr, float textSize);
	static float getTextHeight(float textSize);
	static float getDeltaTime();
	static ImVec2 getMousePos();
	static bool isMouseOver(ImVec4(pos));
	static bool isPointInRect(ImVec2 point, ImVec2 rectMin, ImVec2 rectMax);
};
