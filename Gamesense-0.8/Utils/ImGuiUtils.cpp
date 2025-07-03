#include "ImGuiUtils.h"
#include "../Utils/Utils.h"
#include "../Gamesense/Module/ModuleManager.h"

C_MinecraftUIRenderContext* imCtx;
C_GuiData* imGuiData;
std::shared_ptr<glmatrixf> imrefdef;

vec2_t imfov;
vec2_t imScreenSize;
vec3_t imOrigin;
void ImGuiUtils::ctx(C_MinecraftUIRenderContext* ctx, C_GuiData* guiData) {
	imCtx = ctx;
	imGuiData = guiData;

	glmatrixf* badrefdef = g_Data.getClientInstance()->getRefDef();

	imrefdef = std::shared_ptr<glmatrixf>(badrefdef->correct());
	imfov = g_Data.getClientInstance()->getFov();
	imScreenSize = imGuiData->windowSizeReal;

	if (g_Data.getClientInstance()->levelRenderer != nullptr)
		imOrigin = g_Data.getClientInstance()->levelRenderer->getOrigin();

}

void ImGuiUtils::renderText(ImDrawList* d, ImVec2 pos, const char* text, ImColor color, float textSize) {
	d->AddText(NULL, textSize, pos, color, text);
}

void ImGuiUtils::renderGradientText(ImDrawList* d, ImVec2 pos, const char* text, float textSize, int index, float opacity) {
	std::string textStr(text);
	static auto ColorsModulee = moduleMgr->getModule<Interface>();
	int index2 = index;
	for (int i = 0; i < textStr.size(); i++) {
		index2++;
		int curIndex = -index2 * ColorsModulee->spacing;
		MC_Color interfaceColor = ColorUtil::interfaceColor(curIndex);

		char charText = text[i];
		std::string breh = "a";
		breh[0] = charText;
		ImColor convertedC = ImColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, opacity);
		float charWidth = getTextWidth(d, breh.c_str(), textSize);

		renderText(d, pos, breh.c_str(), convertedC, textSize);
		pos.x += charWidth;
	}
}

void ImGuiUtils::glowTextBackground(ImDrawList* d, ImVec2 pos, const char* text, float textSize, int index) {
	std::string str(text);
	int index2 = index;
	static auto ColorsModulee = moduleMgr->getModule<Interface>();
	for (int i = 0; i < str.size(); i++) {
		if (index >= 1) {
			index2++;
			int curIndex = -index2 * ColorsModulee->spacing;
		}
		char charText = text[i];
		std::string breh = "a";
		breh[0] = charText;
		MC_Color colorr = ColorUtil::interfaceColor(i);
		ImColor color = ImColor(colorr.r, colorr.g, colorr.b);
		//+ getTextWidth(d, breh.c_str(), textSize) / 2.f
		//+ getTextHeight(d, breh.c_str(), textSize) / 2.f
		if (breh != " ") renderCircleShadow(d, ImVec2(pos.x + getTextWidth(d, breh.c_str(), textSize) / 2.f, pos.y + getTextHeight(d, breh.c_str(), textSize) / 2.f), color, 1.f, 2.5f * textSize, true, 0);
		pos.x += getTextWidth(d, breh.c_str(), textSize);
	}
}

float ImGuiUtils::getTextWidth(ImDrawList* d, const char* text, float textSize) {
	return ImGui::CalcTextSize2(text, textSize).x;
}

float ImGuiUtils::getTextHeight(ImDrawList* d, const char* text, float textSize) {
	return ImGui::CalcTextSize2(text, textSize).y;
}

void ImGuiUtils::pushClipRect(ImDrawList* d, ImVec4 pos) {
	d->PushClipRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w));
}

void ImGuiUtils::popClipRect(ImDrawList* d, ImVec4 pos) {
	d->PopClipRect();
}

void ImGuiUtils::renderRect(ImDrawList* d, ImVec4 pos, ImColor color, float thickness, float rounding, ImDrawFlags flag) {
	d->AddRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, rounding, flag, thickness);
}

void ImGuiUtils::renderRectFilled(ImDrawList* d, ImVec4 pos, ImColor color, float rounding, ImDrawFlags flag) {
	d->AddRectFilled(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, rounding, flag);
}

void ImGuiUtils::renderRectShadow(ImDrawList* d, ImVec4 pos, ImColor color, float thickness, float rounding, bool fill, ImVec2 offset) {
	d->AddShadowRect(ImVec2(pos.x, pos.y), ImVec2(pos.z, pos.w), color, thickness, offset, fill ? 0 : ImDrawFlags_ShadowCutOutShapeBackground, rounding);
}

void ImGuiUtils::renderCircle(ImDrawList* d, ImVec2 pos, ImColor color, float radius, float thickness, int num_seg) {
	d->AddCircle(pos, radius, color, num_seg, thickness);
}

void ImGuiUtils::renderCircleFilled(ImDrawList* d, ImVec2 pos, ImColor color, float radius, int num_seg) {
	d->AddCircleFilled(pos, radius, color, num_seg);
}

void ImGuiUtils::renderCircleShadow(ImDrawList* d, ImVec2 pos, ImColor color, float radius, float thickness, bool fill, int num_seg, ImVec2 offset) {
	d->AddShadowCircle(pos, radius, color, thickness, offset, fill ? 0 : ImDrawFlags_ShadowCutOutShapeBackground, num_seg);
}


void ImGuiUtils::renderBox(ImDrawList* d, vec3_t lower, vec3_t upper, float linewidth, float opacity, bool outline) {
	vec3_t diff;
	diff.x = upper.x - lower.x;
	diff.y = upper.y - lower.y;
	diff.z = upper.z - lower.z;

	vec3_t vertices[8];
	vertices[0] = vec3_t(lower.x, lower.y, lower.z);
	vertices[1] = vec3_t(lower.x + diff.x, lower.y, lower.z);
	vertices[2] = vec3_t(lower.x, lower.y + diff.y, lower.z);
	vertices[3] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z);
	vertices[4] = vec3_t(lower.x, lower.y, lower.z + diff.z);
	vertices[5] = vec3_t(lower.x + diff.x, lower.y, lower.z + diff.z);
	vertices[6] = vec3_t(lower.x, lower.y + diff.y, lower.z + diff.z);
	vertices[7] = vec3_t(lower.x + diff.x, lower.y + diff.y, lower.z + diff.z);

	std::vector<std::tuple<int, vec2_t>> screenCords;
	for (int i = 0; i < 8; i++)
	{
		vec2_t screen;
		if (imrefdef->OWorldToScreen(imOrigin, vertices[i], screen, imfov, imScreenSize))
		{
			screenCords.emplace_back(outline ? (int)screenCords.size() : i, screen);
		}
	}
	if (screenCords.size() < 2)
		return; // No lines possible

	if (!outline)
	{
		for (auto it = screenCords.begin(); it != screenCords.end(); it++)
		{
			auto from = *it;
			auto fromOrig = vertices[std::get<0>(from)];

			for (auto to : screenCords)
			{
				auto toOrig = vertices[std::get<0>(to)];

				bool shouldDraw = false;
				// X direction
				shouldDraw |= fromOrig.y == toOrig.y && fromOrig.z == toOrig.z && fromOrig.x < toOrig.x;
				// Y direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.z == toOrig.z && fromOrig.y < toOrig.y;
				// Z direction
				shouldDraw |= fromOrig.x == toOrig.x && fromOrig.y == toOrig.y && fromOrig.z < toOrig.z;

				if (shouldDraw) {
					ImVec2 imfrom = ImVec2(std::get<1>(from).x, std::get<1>(from).y);
					ImVec2 imto = ImVec2(std::get<1>(to).x, std::get<1>(to).y);

					d->AddLine(imfrom, imto, ImColor(255, 255, 255, 255), linewidth);
				}
			}
		}

		return;
	}
}
