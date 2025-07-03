#include "Watermark.h"
#include <chrono>
#include "../../../../Utils/ImGuiUtils.h"
using namespace std;
Watermark::Watermark() : IModule(0, Category::OTHER, "Displays the watermark")
{
	registerEnumSetting("Style", &mode, 0);
	mode.addEntry("New", 0);
	mode.addEntry("Glow", 1);
	mode.addEntry("Basic", 2);
	mode.addEntry("Old", 3);
	registerBoolSetting("Show Version", &showVersion, showVersion);
	registerBoolSetting("Gradient", &gradient, gradient);
	registerBoolSetting("isFirstTime", &isFirstTime, isFirstTime, "Developer module");
	registerFloatSetting("Opacity", &alpha, alpha, 0.f, 255.f);
	registerIntSetting("Size", &textSize, textSize, 10, 40);
	//registerIntSetting("Gradient Spacing", &spacing, spacing, 5, 500);
	shouldHide = true;
}

const char *Watermark::getRawModuleName()
{
	return ("Watermark");
}

void Watermark::onEnable()
{
}

void Watermark::onImGuiRender(ImDrawList* d) {
	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	auto interfaceColor = ColorUtil::interfaceColor(1);
	ImColor convertedC = ImColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, alpha / 255);
	if (!clickGUI->hasOpenedGUI) {
		if (mode.getSelectedValue() == 0 || mode.getSelectedValue() == 1) {
			if (gradient) {
				if (mode.getSelectedValue() == 1)
				ImGuiUtils::glowTextBackground(d, ImVec2(15, 15), "Gamesense", textSize, 1);
				ImGuiUtils::renderGradientText(d, ImVec2(15, 15), "Gamesense", textSize, alpha / 255);
			}
			else {
				if (mode.getSelectedValue() == 1)
					ImGuiUtils::glowTextBackground(d, ImVec2(15, 15), "Gamesense", textSize, 1);
				ImGuiUtils::renderText(d, ImVec2(15, 15), "Gamesense", convertedC, textSize);
			}
			std::string version = std::string("v") + interfaceMod->versionStr;
			if (showVersion) {
				if (gradient) {
					if (mode.getSelectedValue() == 1)
						ImGuiUtils::glowTextBackground(d, ImVec2(15, 15 + ImGuiUtils::getTextHeight(d, "Gamesense", textSize)), version.c_str(), textSize, 1);
					ImGuiUtils::renderGradientText(d, ImVec2(15, 15 + ImGuiUtils::getTextHeight(d, "Gamesense", textSize)), version.c_str(), textSize - 10, alpha / 255);
				}
				else {
					if (mode.getSelectedValue() == 1)
						ImGuiUtils::glowTextBackground(d, ImVec2(15, 15 + ImGuiUtils::getTextHeight(d, "Gamesense", textSize)), version.c_str(), textSize - 10, 0);
					ImGuiUtils::renderText(d, ImVec2(15, 15 + ImGuiUtils::getTextHeight(d, "Gamesense", textSize)), version.c_str(), convertedC, textSize - 10);
				}
			}
		}
		if (mode.getSelectedValue() == 2) {
		if (gradient)
			ImGuiUtils::renderGradientText(d, ImVec2(5, 5), "Gamesense", textSize, alpha / 255);
		else
			ImGuiUtils::renderText(d, ImVec2(5, 5), "Gamesense", convertedC, textSize);
		std::string version = std::string("v") + interfaceMod->versionStr;
		if (showVersion)
			ImGuiUtils::renderText(d, ImVec2(5 + ImGuiUtils::getTextWidth(d, "Gamesense ", textSize), 5), version.c_str(), ImColor(255, 255, 255), textSize - 10);
		}
	}
	// Do cool welcome thingy
	if (isFirstTime && !clickGUI->hasOpenedGUI) {
		fTDel++;
		if (fTDel >= blackDelay) {
			magicOpacity--;
			magicOpacity2 = magicOpacity2 - 0.004f;
		}
		ImColor intColor = ImColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, magicOpacity2);
		ImVec2 windowSize = ImVec2(g_Data.getClientInstance()->getGuiData()->windowSize.x, g_Data.getClientInstance()->getGuiData()->windowSize.y);
		auto theX = 0.77f;
		auto theY = 0.78f;
		std::string version = std::string("v") + interfaceMod->versionStr;
		auto cs = moduleMgr->getModule<ClientSetting>();
		if (cs->welcome) {
			ImGuiUtils::renderRectFilled(d, ImVec4(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame * 10, g_Data.getClientInstance()->getGuiData()->heightGame * 10), ImColor(0, 0, 0, magicOpacity));
			ImGuiUtils::renderText(d, ImVec2(windowSize.x * theX, windowSize.y * theY), "Welcome to", ImColor(255, 255, 255, magicOpacity), textSize);
			ImGuiUtils::renderText(d, ImVec2(windowSize.x * theX + ImGuiUtils::getTextWidth(d, "Welcome to", textSize), windowSize.y * theY), " Gamsense", ImColor(intColor), textSize);
			ImGuiUtils::glowTextBackground(d, ImVec2(windowSize.x * theX + ImGuiUtils::getTextWidth(d, "Welcome to", textSize), windowSize.y * theY), " Gamsense", textSize);
			ImGuiUtils::renderText(d, ImVec2(windowSize.x * theX + ImGuiUtils::getTextWidth(d, "Welc.", textSize), windowSize.y * theY + ImGuiUtils::getTextHeight(d, "Welcome to Gamesense", textSize)), "Loading Client...", ImColor(255, 255, 255, magicOpacity), textSize - 10);
		}
		if (magicOpacity == 0) {
			isFirstTime = false;
			magicOpacity = 255;
			magicOpacity2 = 1.f;
			fTDel = 0;
		}
	}
}

void Watermark::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
	
	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	static int index = 0;
	int curIndex = -index * spacing;
	auto interfaceColor = ColorUtil::interfaceColor(1);
	auto player = g_Data.getLocalPlayer();
	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI)
	{
		if (mode.getSelectedValue() == 3)
		{
			std::string version = interfaceMod->versionStr;
			if (gradient)
			{
				std::string title2 = " Gamesense";
				auto gradientColor = ColorUtil::interfaceColor(curIndex);
				DrawUtils::drawGradientText(vec2_t(2, 5), &title2, 1.5f, alpha / 255, true);
				if (showVersion)
				DrawUtils::drawGradientText(vec2_t(2, 19), &std::string("  v" + version),  1.f, alpha / 255, true);
			}
			if (!gradient)
			{
				if (showVersion)
				    DrawUtils::drawText(vec2_t(2, 5), &std::string(" Gamesense"), MC_Color(interfaceColor), 1.5f, alpha / 255, true);
				    DrawUtils::drawText(vec2_t(2, 19), &std::string("  v" + version), MC_Color(interfaceColor), 1.f, alpha / 255, true);
				if (!showVersion)
					DrawUtils::drawText(vec2_t(2, 5), &std::string(" Gamesense"), MC_Color(interfaceColor), 1.5f, alpha / 255, true);
			}
		}
	}
}
