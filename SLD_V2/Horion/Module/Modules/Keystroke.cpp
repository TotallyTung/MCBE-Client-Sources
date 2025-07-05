#include "Keystroke.h"
#include "../../DrawUtils.h"

Keystroke::Keystroke() : IModule(0, Category::HUD, "Shows what keys you press") {
	registerBoolSetting("Keystrokes", &keystrokes, keystrokes);
}

Keystroke::~Keystroke(){};

const char* Keystroke::getModuleName() {
	return ("Keystroke");
}

void Keystroke::drawKeystroke(char key, const vec2_t& pos) {
	std::string keyString = Utils::getKeybindName(key);
	C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
	if (key == *input->forwardKey) {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 20.f,
			pos.y + 20.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f) + 0.2f,
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
	if (key == *input->spaceBarKey) {
		if (key == *input->spaceBarKey) keyString = "-";
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + 64.f,
			pos.y + 15.f);
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 7.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	} else {
		vec4_t rectPos(
			pos.x,
			pos.y,
			pos.x + ((key == *input->spaceBarKey) ? 64.f : 20.f),
			pos.y + ((key == *input->spaceBarKey) ? 15.f : 20.f));
		vec2_t textPos(
			(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
			rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);

		DrawUtils::fillRectangle(rectPos, GameData::isKeyDown(key) ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
		DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
	}
}

void Keystroke::drawLeftMouseKeystroke(vec2_t pos) {
	std::string keyString;
	keyString = "LMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRectangle(rectPos, GameData::GameData::isLeftClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void Keystroke::drawRightMouseKeystroke(vec2_t pos) {
	std::string keyString;
	keyString = "RMB";
	vec4_t rectPos(
		pos.x,
		pos.y + 2,
		pos.x + 31.f,
		pos.y + 22.f);
	DrawUtils::fillRectangle(rectPos, GameData::GameData::isRightClickDown() ? MC_Color(85, 85, 85) : MC_Color(12, 12, 12), 1.f);
	vec2_t textPos(
		(rectPos.x + (rectPos.z - rectPos.x) / 2) - (DrawUtils::getTextWidth(&keyString) / 2.f),
		rectPos.y + 10.f - DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() / 2.f);
	DrawUtils::drawText(textPos, &keyString, MC_Color(255, 255, 255), 1.f, 1.f);
}

void Keystroke::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	//float f = 10.f * scale;

	if (!(g_Data.getLocalPlayer() == nullptr || !keystrokes || !GameData::canUseMoveKeys())) {
		C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
		Keystroke::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 84));
		Keystroke::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 62));
		Keystroke::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 62));
		Keystroke::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 62));
		Keystroke::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 40));
		Keystroke::drawLeftMouseKeystroke(vec2_t(10.f, windowSize.y - 25));
		Keystroke::drawRightMouseKeystroke(vec2_t(43.f, windowSize.y - 25));
	}
}
