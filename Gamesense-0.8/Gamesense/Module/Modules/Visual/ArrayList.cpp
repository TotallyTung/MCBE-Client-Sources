#include "ArrayList.h"

#include "../../Utils/ImGuiUtils.h"
SettingEnum toolTips;
bool shouldGlow = true;
using namespace std;
ArrayList::ArrayList() : IModule(0, Category::OTHER, "Displays enabled modules on the top right")
{
	registerEnumSetting("Style", &mode, 0);
	mode.addEntry("None", 0);
	mode.addEntry("Bar", 1);
	mode.addEntry("Semi Bar", 2);
	mode.addEntry("Outline", 3); // ugly but ill rewrite dis tmrow
	registerEnumSetting("Modes", &toolTips, 0);
	toolTips.addEntry("Normal", 0);
	toolTips.addEntry("Grayed Out", 1);
	registerBoolSetting("Glow", &shouldGlow, shouldGlow);
	registerIntSetting("Size", &size, size, 15, 40, "Text Size");
	registerFloatSetting("Opacity", &opacity, opacity, 0.f, 1.f, 0.01f, "Change background opacitys");
	registerBoolSetting("Bottom", &bottom, bottom, "BOTOM");
	shouldHide = true;
}

struct sortByWidth {
	ImDrawList* d = ImGui::GetBackgroundDrawList();
	bool operator()(std::shared_ptr<IModule> lhs, std::shared_ptr<IModule> rhs) {
		float textSize = 40.f;
		return ImGuiUtils::getTextWidth(d, lhs.get()->getRawModuleName(), textSize) + ImGuiUtils::getTextWidth(d, lhs.get()->getModName(), textSize) > ImGuiUtils::getTextWidth(d, rhs.get()->getRawModuleName(), textSize) + ImGuiUtils::getTextWidth(d, rhs.get()->getModName(), textSize);
	}
};

const char *ArrayList::getRawModuleName()
{
	return ("ArrayList");
}

void ArrayList::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
}

void ArrayList::onImGuiRender(ImDrawList* d) {
	float offset = 0.f;

	float textSize = (float)size;
	float textPadding = 1.f * (textSize / 25.f);
	ImVec2 windowSize = ImGui::GetIO().DisplaySize;
	windowSize.x -= offset;
	ImVec4 lastRect = ImVec4(0, 0, 0, 0);

	std::vector<std::shared_ptr<IModule>> moduleList = *moduleMgr->getModuleList();
	//std::sort(moduleList.begin(), moduleList.size());
	std::sort(moduleList.begin(), moduleList.end(), sortByWidth());
	int index = 0;

	float yOffset = 0.f + offset;
	if (bottom) yOffset = windowSize.y - ImGuiUtils::getTextHeight(d, "A", textSize);

	auto themeMod = moduleMgr->getModule<Interface>();
	for (std::shared_ptr<IModule> mod : moduleList) {

		if (mod->shouldHide) continue;
		int curIndex = -index * themeMod->spacing;

		MC_Color Mcolor = ColorUtil::interfaceColor(curIndex);
		ImColor color = ImColor(Mcolor.r, Mcolor.g, Mcolor.b, mod->onArrlistDuration);
		float textWidth = ImGuiUtils::getTextWidth(d, mod->getRawModuleName(), textSize);
		float textHeight = ImGuiUtils::getTextHeight(d, mod->getRawModuleName(), textSize);
		float toolTipWidth = ImGuiUtils::getTextWidth(d, mod->getModName(), textSize);
		float toolTipHeight = ImGuiUtils::getTextHeight(d, mod->getModName(), textSize);

		if (mod->isEnabled()) mod->onArrlistDuration += (1.f - mod->onArrlistDuration) * 0.2f;
		else mod->onArrlistDuration += (0.f - mod->onArrlistDuration) * 0.2f;

		ImVec2 textPos = ImVec2(windowSize.x - (textWidth + toolTipWidth + textPadding * 2.f) * mod->onArrlistDuration, yOffset);

		if (mode.getSelectedValue() == 1 || mode.getSelectedValue() == 2) textPos.x -= 2.f;

		ImVec4 rect = ImVec4(textPos.x - textPadding * 2.f, textPos.y, textPos.x + textWidth + toolTipWidth + textPadding * 2.f, textPos.y + textHeight);
		if (mod->onArrlistDuration > 0.1f) {
			ImGuiUtils::renderRectFilled(d, rect, ImColor(0, 0, 0, (int)((opacity * mod->onArrlistDuration) * 255.f)));
			ImGuiUtils::renderText(d, textPos, mod->getRawModuleName(), color, textSize);
			if (shouldGlow) ImGuiUtils::glowTextBackground(d, textPos, mod->getRawModuleName(), textSize, 1);
			ImVec2 toolTipPos = ImVec2(textPos.x + textWidth, textPos.y);
			std::string modName = std::string(mod->getModName());
			if (toolTips.getSelectedValue() == 0)
			ImGuiUtils::renderText(d, toolTipPos, modName.c_str(), ImColor(255, 255, 255, int(255 * mod->onArrlistDuration)), textSize);
			if (toolTips.getSelectedValue() == 1)
				ImGuiUtils::renderText(d, toolTipPos, modName.c_str(), ImColor(180, 180, 180, int(255 * mod->onArrlistDuration)), textSize);

			if (mode.getSelectedValue() == 1) {
				ImVec4 bar = ImVec4(rect.z - 2.f, rect.y, rect.z, rect.w);
				ImGuiUtils::renderRectFilled(d, bar, color);
			}

			if (mode.getSelectedValue() == 2) {
				ImVec4 bar = ImVec4(rect.z - 2.f, rect.y + 3.f, rect.z, rect.w - 3.f);
				ImGuiUtils::renderRectFilled(d, bar, color, 10.f);
			}
			if (mode.getSelectedValue() == 3) {
				ImVec4 bar = ImVec4(rect);
				ImGuiUtils::renderRect(d, bar, color, 1.f);
			}

			if (!bottom) yOffset += textHeight * mod->onArrlistDuration;
			else yOffset -= textHeight * mod->onArrlistDuration;
			lastRect = rect;
			++index;
		}
	}
}
