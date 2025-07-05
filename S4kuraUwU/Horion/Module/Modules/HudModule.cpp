#include "HudModule.h"
#include "../../DrawUtils.h"
#include "../../Scripting/ScriptManager.h"
#include "../../../Utils/Logger.h"
#include "Theme.h"
HudModule::HudModule() : IModule(0, Category::HUD, "Displays ArrayList/TabGUI") {
	registerBoolSetting("TabGui", &tabgui, tabgui);
	//registerBoolSetting("ArrayList", &arraylist, arraylist);
	registerBoolSetting("ClickToggle", &clickToggle, clickToggle);
	registerBoolSetting("Watermark", &watermark2, watermark2);
	registerBoolSetting("Coordinates", &coordinates, coordinates);
	registerBoolSetting("Show Keybinds", &keybinds, keybinds);
	registerBoolSetting("Show ArmorHUD", &displayArmor, displayArmor);
	registerBoolSetting("Keystrokes", &keystrokes, keystrokes);
	registerBoolSetting("Show FPS", &fps, fps);
	registerBoolSetting("Show CPS", &cps, cps);
	registerBoolSetting("Show BPS", &bps, bps);
	registerBoolSetting("Always show", &alwaysShow, alwaysShow);
	registerFloatSetting("Scale", &scale, scale, 0.5f, 1.5f);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}

void HudModule::drawKeystroke(char key, const vec2_t& pos) {
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

void HudModule::drawLeftMouseKeystroke(vec2_t pos) {
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

void HudModule::drawRightMouseKeystroke(vec2_t pos) {
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

void HudModule::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	float f = 10.f * scale;
	std::string tempStr("Movement");
	float len = DrawUtils::getTextWidth(&tempStr, scale) + 7.f;
	float startY = tabgui ? 6 * f : 0.f;
	static auto ThemeMod = moduleMgr->getModule<Theme>();
	if (tabgui && scriptMgr.getNumEnabledScripts() > 0)
		startY += f;
	{
		if (!(g_Data.getLocalPlayer() == nullptr || !watermark2)) {
			using namespace std;

			auto player = g_Data.getLocalPlayer();
			if (player == nullptr) return;

			vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;

			if (g_Data.isInGame() && GameData::canUseMoveKeys()) {
				float nameTextSize = scale2 + 0.5f;;
				static const float textHeight = (nameTextSize * 0.7f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
				constexpr float borderPadding = 1;
				constexpr float margin = 5;
				std::string playerName = std::string(g_Data.getLocalPlayer()->getNameTag()->getText());

				switch (TextMode.selected) {
				default:
					name = "S4kuraUwU ";
					name2 = "v1.7.0 Dev";
					break;
				}
				switch (rainBow.selected) {
				default:
					pastel = true;
					rainbow = false;
					break;
				case 1:
					pastel = false;
					rainbow = true;
					break;
				}
				float nameLength = DrawUtils::getTextWidth(&name, nameTextSize);
				float fullTextLength = nameLength;
				float x = windowSize.x - windowSize.x +28 - nameLength / 2;
				float y = windowSize.y - windowSize.y + 2;
				vec4_t rect = vec4_t(
					windowSize.x - margin - fullTextLength - borderPadding * 415,
					windowSize.y - margin - textHeight,
					windowSize.x - margin + borderPadding,
					windowSize.y - margin);
				static float rcolors[4];	// Rainbow color array RGBA
				static float rcolors2[4];          
				static float disabledRcolors[4];	// Rainbow Colors, but for disabled modules
				static float disabledRcolors2[4];  
				static float currColor[4];  
				static float currColor2[4];  // ArrayList colors
				{
					Utils::ApplyRainbow(rcolors, 0.0008f);  // Increase Hue of rainbow color array
					disabledRcolors[0] = std::min(1.f, rcolors[0] * 0.4f + 0.2f);
					disabledRcolors[1] = std::min(1.f, rcolors[1] * 0.4f + 0.2f);
					disabledRcolors[2] = std::min(1.f, rcolors[2] * 0.4f + 0.2f);
					disabledRcolors[3] = 1;
				}

				int a = 0;
				int b = 0;
				int c = 0;
				{
					Utils::ApplyRainbow(rcolors2);  // Increase Hue of rainbow color array
					disabledRcolors2[0] = std::min(1.f, rcolors2[0] * 0.4f + 0.2f);
					disabledRcolors2[1] = std::min(1.f, rcolors2[1] * 0.4f + 0.2f);
					disabledRcolors2[2] = std::min(1.f, rcolors2[2] * 0.4f + 0.2f);
					disabledRcolors2[3] = 1;
				}
				c++;
				b++;
				if (b < 20)
					a = moduleMgr->getEnabledModuleCount() * 2;
				else
					b = 0;
				currColor2[3] = rcolors2[3];
				Utils::ColorConvertRGBtoHSV(rcolors2[0], rcolors2[1], rcolors2[2], currColor2[0], currColor2[1], currColor2[2]);
				currColor2[0] += 0.5f / a * c;
				Utils::ColorConvertHSVtoRGB(currColor2[0], currColor2[1], currColor2[2], currColor2[0], currColor2[1], currColor2[2]);

				currColor[3] = rcolors[5];
				currColor[3] = rcolors[5];
				Utils::ColorConvertRGBtoHSV(rcolors[0 & 1], rcolors[2], rcolors[01], currColor[0], currColor[1], currColor[2]);
				currColor[0] += 0.5f;
				Utils::ColorConvertHSVtoRGB(currColor[0 & 1], currColor[2], currColor[2], currColor[0], currColor[1], currColor[2]);
				int ind = 1;
				for (char c : name) {
					ind++;
					int charWidth = DrawUtils::getTextWidth(&string(&c), nameTextSize);
					if (ThemeMod->rainbow) {
						DrawUtils::drawText(vec2_t(x, y), &string(&c), MC_Color(currColor2), nameTextSize, 1);
					}
					else {
						DrawUtils::drawText(vec2_t(x, y), &string(&c), MC_Color(Theme::rFloat, Theme::gFloat, Theme::bFloat), nameTextSize, 1);
					}
					x += charWidth;
				}
				for (char c2 : name2) {
					ind++;
					int charWidth = DrawUtils::getTextWidth(&string(&c2), nameTextSize);
					{
						//MC_Color arrayColor = DrawUtils::getWaveRGB(r1, g1, b1, r2, g2, b2, -ind * (sped * -1));
						DrawUtils::drawText(vec2_t(x, y), &string(&c2), MC_Color(155,155,155), nameTextSize , 1);
					}
					x += charWidth;
				}
			}
		}
	}
	{  // FPS
		if (!(g_Data.getLocalPlayer() == nullptr || !fps)) {
			std::string fpsText = "FPS: ";
			std::string fpsText2 = std::to_string(g_Data.getFPS() / 2);
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 299.6f);
			vec2_t textPos2 = vec2_t(rectPos.x + 20.f, rectPos.y + 299.6f);
			DrawUtils::drawText(textPos, &fpsText, MC_Color(155, 155, 155, 1), scale);
			DrawUtils::drawText(textPos2, &fpsText2, MC_Color(255,255,255,1), scale);
			startY += f;
		}
	}
	{  // CPS
		if (!(g_Data.getLocalPlayer() == nullptr || !cps)) {
			std::string cpsText = "CPS: ";
			std::string cpsText2 = std::to_string(g_Data.getLeftCPS()) + " - " + std::to_string(g_Data.getRightCPS());
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 299.8f);
			vec2_t textPos2 = vec2_t(rectPos.x + 20.f, rectPos.y + 299.6f);
			DrawUtils::drawText(textPos, &cpsText, MC_Color(155, 155, 155, 1), scale);
			DrawUtils::drawText(textPos2, &cpsText2, MC_Color(255, 255, 255, 1), scale);
			startY += f;
		}
	}
	{  // Coordinates
		if (!(g_Data.getLocalPlayer() == nullptr || !coordinates)) {
			vec3_t* pos = &g_Data.getLocalPlayer()->getHumanPos();
			std::string coordsX = std::to_string((int)floorf(pos->x));
			std::string coordsY = std::to_string((int)floorf(pos->y - 1.f));
			std::string coordsZ = std::to_string((int)floorf(pos->z));
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 35.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 300.f);
			vec2_t textPos2 = vec2_t(rectPos.x + 20.f, rectPos.y + 299.6f);
			std::string coordsText = "XYZ: ";
			std::string coordsText2 = coordsX + " " + coordsY + " " + coordsZ;
			DrawUtils::drawText(textPos, &coordsText, MC_Color(155, 155, 155, 1), scale);
			DrawUtils::drawText(textPos2, &coordsText2, MC_Color(255, 255, 255, 1), scale);
		}
	}
	{  // Bps
		if (!(g_Data.getLocalPlayer() == nullptr || !bps)) {
			auto player = g_Data.getLocalPlayer();
			vec3_t vel = player->velocity;
			if (vel.x < 0.f)
				vel.x *= -1.f;
			if (vel.y < 0.f)
				vel.y *= -1.f;
			if (vel.z < 0.f)
				vel.z *= -1.f;

			speed = sqrtf((vel.x * vel.x) + (vel.z * vel.z));
			speed *= 36.6f;

			if (!player->onGround)
				speed /= 1.5f;
			vec4_t rectPos = vec4_t(2.5f, startY + 5.f * scale, len, startY + 15.f * scale);
			vec2_t textPos = vec2_t(rectPos.x + 1.5f, rectPos.y + 275.f);
			vec2_t textPos2 = vec2_t(rectPos.x + 30.f, rectPos.y + 275.f);
			std::string BpsText = "Speed: ";
			std::string BpsText2 = std::string(std::to_string((int)speed)) + "km/h";
			DrawUtils::drawText(textPos, &BpsText, MC_Color(155, 155, 155, 1), scale);
			DrawUtils::drawText(textPos2, &BpsText2, MC_Color(255, 255, 255, 1), scale);
			startY += f;
		}
	}
	{  // ArmorHUD
		if (displayArmor && GameData::canUseMoveKeys()) {
			static float constexpr scale = 1.f;
			static float constexpr opacity = 0.25f;
			static float constexpr spacing = scale + 17.f;
			float x = windowSize.x / 2.f + 7.0f;
			float y = windowSize.y - 72.5f;
			float y1 = windowSize.y - 78.5f;
			C_LocalPlayer* player = g_Data.getLocalPlayer();

			for (int i = 0; i < 4; i++) {
				C_ItemStack* stack = player->getArmor(i);
				if (stack->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
					C_Item* item = stack->getItem();
					if (item->getMaxDamage() > 0) {
						int armorDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
						std::string armorDamageText = std::to_string(armorDamage) + "%";
						int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
						DrawUtils::drawText(vec2_t(x + scale, y1), &armorDamageText, MC_Color(255 - i, i, 100), 0.7f, 1.f);
					}  // 盔甲耐久度
				}
				x += scale * spacing;
			}
			C_ItemStack* stack = player->getSelectedItem();
			// x += scale * spacing;
			if (stack->isValid()) {
				DrawUtils::drawItem(stack, vec2_t(x, y), opacity, scale, stack->isEnchanted());
				C_Item* item = stack->getItem();
				if (item->getMaxDamage() > 0) {
					int itemDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
					std::string itemDamageText = std::to_string(itemDamage) + "%";
					int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
					DrawUtils::drawText(vec2_t(x + scale, y1), &itemDamageText, MC_Color(255 - i, i, 100), 0.7f, 1.f);
				}  // 手持物品耐久度
			}
		}
	}
	{  // Keystrokes
		if (!(g_Data.getLocalPlayer() == nullptr || !keystrokes || !GameData::canUseMoveKeys())) {
			C_GameSettingsInput* input = g_Data.getClientInstance()->getGameSettingsInput();
			HudModule::drawKeystroke(*input->forwardKey, vec2_t(32.f, windowSize.y - 84));
			HudModule::drawKeystroke(*input->leftKey, vec2_t(10.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->backKey, vec2_t(32.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->rightKey, vec2_t(54.f, windowSize.y - 62));
			HudModule::drawKeystroke(*input->spaceBarKey, vec2_t(10.f, windowSize.y - 40));
			HudModule::drawLeftMouseKeystroke(vec2_t(10.f, windowSize.y - 25));
			HudModule::drawRightMouseKeystroke(vec2_t(43.f, windowSize.y - 25));
		}
	}
}