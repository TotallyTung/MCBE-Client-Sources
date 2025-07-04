#include "RenderUtils.h"
#include "../Client/Client.h"

void RenderUtils::drawText(ImDrawList* d, const Vec2<float>& pos, const char* textStr, const Color& color, const float& textSize, const bool& shadow, const float& glowRadius) {
	static CustomFont* customFontMod = client->moduleMgr->getModule<CustomFont>();
	if (customFontMod->textShadow && shadow) d->AddText(NULL, 25.f * textSize, ImVec2(pos.x + 2.f * textSize, pos.y + 2.f * textSize), ImColor((int)(color.r / 5.f), (int)(color.g / 5.f), (int)(color.b / 5.f), color.a), textStr);
	d->AddText(NULL, 25.f * textSize, ImVec2(pos.x, pos.y), ImColor(color.r, color.g, color.b, color.a), textStr);
	if (glowRadius != 0.0f) d->AddShadowRect(ImVec2(pos.x + 5.f * textSize, pos.y + 7.5f * textSize), ImVec2(pos.x + getTextWidth(textStr, textSize) - 5.f * textSize, pos.y + getTextHeight(textStr, textSize) - 5.5f * textSize), ImColor(color.r, color.g, color.b, color.a), glowRadius, ImVec2(0.f, 0.f), 0, 50.f);
	//d->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.x + getTextWidth(textStr, textSize), pos.y + getTextHeight(textStr, textSize)), ImColor(0, 0, 0, 125), 25.f, ImVec2(0.f, 0.f), ImDrawFlags_ShadowCutOutShapeBackground, 10.f);
}

float RenderUtils::getTextWidth(const char* textStr, const float& textSize) {
	return ImGui::CalcTextSize2(textStr, textSize * 25.f).x;
}

float RenderUtils::getTextHeight(const char* textStr, const float& textSize) {
	return ImGui::CalcTextSize2(textStr, textSize * 25.f).y;
}

float RenderUtils::getTextHeightReal(const char* textStr, const float& textSize) {
	return ImGui::CalcTextSize2(textStr, textSize * 25.f).y - 5.f * textSize;
}

void RenderUtils::drawRect(ImDrawList* d, const Vec2<float>& posMin, const Vec2<float>& posMax, const Color& color, const float& thickness, const float& rounding) {
	d->AddRect(ImVec2(posMin.x, posMin.y), ImVec2(posMax.x, posMax.y), ImColor(color.r, color.g, color.b, color.a), rounding, 0, thickness);
}

void RenderUtils::drawRectFilled(ImDrawList* d, const Vec2<float>& posMin, const Vec2<float>& posMax, const Color& color, const float& rounding) {
	d->AddRectFilled(ImVec2(posMin.x, posMin.y), ImVec2(posMax.x, posMax.y), ImColor(color.r, color.g, color.b, color.a), rounding, 0);
}

void RenderUtils::drawRectFilled(ImDrawList* d, const Vec4<float>& pos, const Color& color, const float& rounding) {
	d->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, color.a), rounding, 0);
}

void RenderUtils::drawShadowRect(ImDrawList* d, const Vec4<float>& pos, const Color& color, const float& shadowThickness, const bool& filled, const float& rounding) {
	d->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), ImColor(color.r, color.g, color.b, color.a), shadowThickness, ImVec2(0.f, 0.f), filled ? ImDrawFlags_None : ImDrawFlags_ShadowCutOutShapeBackground, rounding);
}