#pragma once
#include "../Utils.h"
#include "../../Include/imgui/imgui.h"
#include "RenderUtils.h"
#include "ColorUtil.h"

class ImGuiUtils {
public:
	template <typename T> // not related to memory LOL
	static std::string combine(T t)
	{
		std::stringstream ss;
		ss << t;
		return ss.str();
	}

	template <typename T, typename... Args> // not related to memory LOL
	static std::string combine(T t, Args... args)
	{
		std::stringstream ss;
		ss << t << combine(args...);
		return ss.str();
	}
	
	static __forceinline void drawText(ImFont* font, Vec2<float> textPos, std::string text, Color color, float fontSize, bool drawShadow) {
		if (!ImGui::GetCurrentContext()) return;
		const auto d = ImGui::GetBackgroundDrawList();
		if (drawShadow)
			d->AddText(font, fontSize * 18.f, RenderUtils::scaledResToReal(Vec2<float>(textPos.x + 1.f, textPos.y + 1.f)), ImColor(50, 50, 50, (int)(color.a * 255)), text.c_str());
		d->AddText(font, fontSize * 18.f, RenderUtils::scaledResToReal(textPos), ImColor(color.r, color.g, color.b, color.a), text.c_str());
	}

	static __forceinline void RenderGlowText(Vec2<float> textPos, std::string text, float fontSize) {
		Vec2<float> logoPos = Vec2<float>(textPos.x, textPos.y);
		auto font = ImGui::GetFont();

		Vec2<float> pos2 = logoPos;
		int ind = 0;
		int ind2 = 0;
		std::string name = text;

		name = text;

		for (char c : (std::string)name)
		{
			std::string string = combine(c, "");
			int colorIndex = ind * 25;

			float charWidth = getTextLen(&string, fontSize);
			float charHeight = getTextHeight(fontSize);
			Vec4<float> rect(pos2.x, pos2.y, pos2.x + 20, pos2.y + 20);
			drawShadowSquare(Vec2<float>(pos2.x + charWidth / 2, pos2.y + charHeight / 1.2), 15.f, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, -colorIndex * 90, 255), 1.f, 70.f, 0);

			drawText(font,Vec2<float>(pos2.x + 11.5, pos2.y + 10), string, Color(0, 0, 0), fontSize, false);
			drawText(font, Vec2<float>(pos2.x + 10, pos2.y + 10), string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, -colorIndex * 90, 255), fontSize, false);
			drawText(font, Vec2<float>(pos2.x + 10.5, pos2.y + 10), string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, -colorIndex * 90, 255), fontSize, false);
			drawText(font, Vec2<float>(pos2.x + 11, pos2.y + 10), string, ColorUtil::waveColor(2, 69, 95, 255, 255, 255, -colorIndex * 90, 255), fontSize, false);

			pos2.x += charWidth;
			++ind;
		}
	}

	static void drawShadowSquare(Vec2<float> center, float size, const Color& color, float alpha, float thickness, ImDrawFlags flags)
	{
		if (!ImGui::GetCurrentContext())
			return;

		ImDrawList* list = ImGui::GetBackgroundDrawList();
		ImVec2 offset = ImVec2(0, 0);

		// Define the four corners of the square
		ImVec2 points[4];
		points[0] = ImVec2(center.x - size / 2.f, center.y - size / 2.f);
		points[1] = ImVec2(center.x + size / 2.f, center.y - size / 2.f);
		points[2] = ImVec2(center.x + size / 2.f, center.y + size / 2.f);
		points[3] = ImVec2(center.x - size / 2.f, center.y + size / 2.f);

		list->AddShadowConvexPoly(points, 4, ImColor(color.r, color.g, color.b, alpha), thickness, offset, flags);
	}
	
	static __forceinline void renderOutlinedText(std::string text, Vec2<float> textPos, Color fillColor, Color outlineColor, float textSize, ImFont* font) {
		// Calculate how much to go forward/backwards based on text size
		float backwards = -0.25f * textSize;
		float forwards = 0.5f * textSize;
		// Get rid of formatting
		std::string rt = text;
		int formatPos = rt.find("§");
		while (formatPos != std::string::npos) {
			if (rt.size() > formatPos + 2) {
				if (rt.at(formatPos + 2) != 'l') { // Don't get rid of bold
					rt.erase(formatPos - 1, 3);
				}
			}
			formatPos = rt.find("§", formatPos + 1);
		}
		// Outline the text
		ImGuiUtils::drawText(font, textPos + Vec2<float>(backwards, backwards), rt, outlineColor, textSize, false);
		ImGuiUtils::drawText(font, textPos + Vec2<float>(forwards, forwards), rt, outlineColor, textSize, false);
		ImGuiUtils::drawText(font, textPos + Vec2<float>(backwards, forwards), rt, outlineColor, textSize, false);
		ImGuiUtils::drawText(font, textPos + Vec2<float>(forwards, backwards), rt, outlineColor, textSize, false);

		// Draw text
		ImGuiUtils::drawText(font, textPos, text, fillColor, textSize, false);
	}

	static __forceinline float getTextLen(std::string* textStr, float textSize) {
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, textStr->c_str()).x;
	}

	static __forceinline float getTextHeight(float textSize) {
		return ImGui::GetFont()->CalcTextSizeA(textSize * 18, FLT_MAX, -1, "").y;
	}

	static __forceinline void drawRect(Rect position, Color color, float alpha, float lineWidth) {
		if (!ImGui::GetCurrentContext()) return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRect(RenderUtils::scaledResToReal(Vec2<float>(position.x, position.y)), (RenderUtils::scaledResToReal(Vec2<float>(position.z, position.w))), ImColor(color.r, color.g, color.b, alpha), 0.f, 0, lineWidth);
	}

	static __forceinline void fillRectangle(Rect position, Color color) {
		if (!ImGui::GetCurrentContext()) return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddRectFilled(RenderUtils::scaledResToReal(Vec2<float>(position.x, position.y)), (RenderUtils::scaledResToReal(Vec2<float>(position.z, position.w))), ImColor(color.r, color.g, color.b, color.a));
	}

	static __forceinline void drawLine(Vec2<float> start, Vec2<float> end, Color color, float lineWidth) {
		if (!ImGui::GetCurrentContext()) return;
		const auto d = ImGui::GetBackgroundDrawList();
		d->AddLine(RenderUtils::scaledResToReal(start), RenderUtils::scaledResToReal(end), ImColor(color.r, color.g, color.b, color.a), lineWidth);
	}
};
extern ImGuiUtils* imGuiUtils;