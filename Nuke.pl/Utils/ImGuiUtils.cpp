#include "ImGuiUtils.h"
#include "../Horion/Module/ModuleManager.h"

ImDrawList* drawList;
float ImGuiUtils::fontSize = 22.f;

void ImGuiUtils::setDrawList(ImDrawList* d) {
	drawList = d;
}

void ImGuiUtils::drawText(Vec2 pos, const char* text, UIColor color, float textSize, bool shadow) {
	/*
	if (shadow) {
		drawList->AddText(nullptr, 22.f * textSize, pos.add(2.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
	}
	*/
	static CustomFont* customFontMod = moduleMgr->getModule<CustomFont>();
	if (shadow) {
		if (customFontMod->shadowMode.getSelectedValue() == 1) {
			drawList->AddText(nullptr, fontSize * textSize, pos.add(2.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
		} else if (customFontMod->shadowMode.getSelectedValue() == 2) {
			drawList->AddText(nullptr, fontSize * textSize, pos.add(1.f * textSize).toImVec2(), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), text);
		}
	}
	drawList->AddText(nullptr, fontSize * textSize, pos.toImVec2(), color.toImColor(), text);
}

float ImGuiUtils::getTextWidth(const char* textStr, float textSize) {
	return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, textStr).x;
}

float ImGuiUtils::getTextHeight(float textSize) {
	return ImGui::GetFont()->CalcTextSizeA(fontSize * textSize, FLT_MAX, -1, "").y;
}

void ImGuiUtils::drawRect(const Vec4& pos, UIColor color, float thickness, float rounding) {
	drawList->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), rounding, 0, thickness);
}

void ImGuiUtils::drawRectFilled(const Vec4& pos, UIColor color, float rounding) {
	drawList->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), rounding, 0);
}

void ImGuiUtils::drawShadowRect(const Vec4& pos, UIColor color, float shadowThickness, bool filled, float rounding) {
	drawList->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color.toImColor(), shadowThickness, ImVec2(0.f, 0.f), filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding);
}