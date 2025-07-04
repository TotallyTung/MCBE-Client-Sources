#include "Watermark.h"
#include "../pch.h"
#include <chrono>

using namespace std;
Watermark::Watermark() : IModule(0, Category::VISUAL, "Displays the watermark") {
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerBoolSetting("Outline", &outlinec, outlinec);
	shouldHide = true;
}

const char* Watermark::getModuleName() {
	return ("Watermark");
}

void Watermark::onEnable() {
}

void Watermark::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr || moduleMgr->getModule<DebugMenu>()->isEnabled()) return;

	auto interfaceMod = moduleMgr->getModule<Interface>();
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();

	//DrawUtils::drawImage("textures/ui/heart_new.png", vec2_t(positionX + 5, positionY + 50), vec2_t(30, 30), vec2_t(0, 0), vec2_t(1, 1));

	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI) {

		//DrawUtils::drawRounderRectangle(vec4_t(positionX, positionY, positionX + 15, positionY + 2.F), interfaceColor);
		//DrawUtils::drawCircle(vec4_t(positionX + 10, positionY, positionX + 14, positionY + 2.F), MC_Color(10, 10, 10, 255));

		float cc = 0;
		float hai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * scale;
		float wit = DrawUtils::getTextWidth(&std::string("Aura - V4.8 Beta"), 1.f);
		auto interfaceColor = ColorUtil::interfaceColor(1);
		//if (gradient) DrawUtils::drawGradientText(vec2_t(simpleRect.x, simpleRect.y), &clientName, scale, 1, true);
		//else DrawUtils::drawText(vec2_t(simpleRect.x, simpleRect.y), &clientName, interfaceColor, scale, 1, true);
		DrawUtils::drawText(vec2_t(2, 2), &std::string("Aura - V4.8 Beta"), MC_Color(interfaceColor), 1.f, 1.f, true);
		if (outlinec) {
			DrawUtils::drawRectangle(vec4_t(cc - 3, cc - 3, cc + wit + 3, cc + hai + 2), MC_Color(interfaceColor), 1.f);
		}
		if (opacity != 0) {
			DrawUtils::fillRectangleA(vec4_t(cc - 3, cc - 3, cc + wit + 3, cc + hai + 2), MC_Color(0, 0, 0, opacity));
		}
	}
}
