#include "Interface.h"
#include "../pch.h"

Interface::Interface() : IModule(0, Category::VISUAL, "Displays the HUD") {
	registerEnumSetting("Color", &color, 0);
	color.addEntry("Rainbow", 0);
	color.addEntry("Astolfo", 1);
	color.addEntry("Wave", 2);
	color.addEntry("RGBWave", 3);
	registerBoolSetting("ArmorHUD", &armorHUD, armorHUD);
	registerBoolSetting("Info", &info, info);
	registerBoolSetting("Screen outline", &scroutline, scroutline);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("Saturation", &saturation, saturation, 0.f, 1.f);
	registerIntSetting("Seperation", &spacing, spacing, 1, 200);

	registerFloatSetting("Red", &r, r, 0.f, 255.f);
	registerFloatSetting("Green", &g, g, 0.f, 255.f);
	registerFloatSetting("Blue", &b, b, 0.f, 255.f);
	registerFloatSetting("Red2", &r2, r2, 0.f, 255.f);
	registerFloatSetting("Green2", &g2, g2, 0.f, 255.f);
	registerFloatSetting("Blue2", &b2, b2, 0.f, 255.f);
	shouldHide = true;
}

const char* Interface::getModuleName() {
	return ("Theme");
}

//void Interface::onTick(C_GameMode* gm) {
//	shouldHide = true;
//}

void Interface::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	auto clickGUIh = moduleMgr->getModule<ClickGUIMod>();
	auto playerh = g_Data.getLocalPlayer();
	if (playerh == nullptr) return;

	//windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (!clickGUIh->hasOpenedGUI) {
		if (armorHUD) {
			auto interfaceCL = ColorUtil::interfaceColor(1);
			armorX = windowSize.x / 2 + 100;
			armorY = windowSize.y - 23.5;
			vec4_t testRect = vec4_t(armorX, armorY, 50 + armorX, armorY + 20);
			DrawUtils::drawRectangle(vec4_t(testRect.x - 1, testRect.y - 1, testRect.x + 4 * scale * 20.f, windowSize.y + 1), MC_Color(interfaceCL), 1.f);
			DrawUtils::fillRectangleA(vec4_t(testRect.x - 1, testRect.y - 1, testRect.x + 4 * scale * 20.f, windowSize.y + 1), MC_Color(0, 0, 0, Interface::opacity));
		}
	}
}

void Interface::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	auto interfaceCL = ColorUtil::interfaceColor(1);
	auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	if (scroutline)
		DrawUtils::drawRectangle(vec4_t(0, 0, g_Data.getClientInstance()->getGuiData()->widthGame, g_Data.getClientInstance()->getGuiData()->heightGame), MC_Color(interfaceCL), 1.f, 2.f);

	static int index = 0;
	int curIndex = -index * spacing;
	auto hudColor = ColorUtil::interfaceColor(curIndex);
	windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	if (!clickGUI->hasOpenedGUI) {
		if (armorHUD) {
			vec4_t testRect = vec4_t(armorX, armorY, 50 + armorX, armorY + 20);
			vec2_t armorPos = vec2_t(testRect.x, testRect.y);
			static float constexpr opacity = 10;
			float scale = 1.f;
			float spacing = 20.f;
			for (int t = 0; t < 4; t++) {
				C_ItemStack* stack = player->getArmor(t);
				if (stack->isValid()) {
					float dura1 = stack->getItem()->getMaxDamage();
					float dura2 = stack->getItem()->getDamageValue(stack->tag);
					float dura3 = dura1 - dura2;
					int dura4 = dura3 / dura1 * 100;
					std::string durastr = std::to_string((int)dura4) + std::string("%");
					MC_Color green(0, 255, 0);
					MC_Color red(255, 0, 0);
					float dura5 = dura3 / dura1 * 100;
					float fraction = dura5 / 100;
					vec3_t greenVec(0, 0, 0);
					vec3_t redVec(0, 0, 0);
					Utils::ColorConvertRGBtoHSV(green.r, green.g, green.b, greenVec.x, greenVec.y, greenVec.z);
					Utils::ColorConvertRGBtoHSV(red.r, red.g, red.b, redVec.x, redVec.y, redVec.z);
					vec3_t colorVec = redVec.lerp(greenVec, fraction, fraction, fraction);

					MC_Color gamerColor(0, 0, 0);

					Utils::ColorConvertHSVtoRGB(colorVec.x, colorVec.y, colorVec.z, gamerColor.r, gamerColor.g, gamerColor.b);
	
					//DrawUtils::drawText(vec2_t(x, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 0.55f, 1.f, true);
					DrawUtils::drawCenteredString(vec2_t(armorPos.x + 9.f, armorPos.y + 20.f), &durastr, 0.75f, MC_Color(gamerColor), true);
					
					DrawUtils::drawItem(stack, armorPos, 1, 1, false);
					armorPos.x += scale * spacing;
				}
				
			}
			//C_PlayerInventoryProxy* supplies = player->getSupplies();
			//C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
			//if (item->isValid()) DrawUtils::drawItem(item, armorPos, opacity, 1, item->isEnchanted());
		//	DrawUtils::drawRectangle(vec4_t(testRect.x - 1, testRect.y - 1, testRect.x + 4 * scale * spacing, windowSize.y), MC_Color(interfaceCL), 1.f);
		//	DrawUtils::fillRectangleA(vec4_t(testRect.x - 1, testRect.y - 1, testRect.x + 4 * scale * spacing, windowSize.y), MC_Color(0, 0, 0, Interface::opacity));
		}


		if (info) {
			auto hudColor = ColorUtil::interfaceColor(curIndex);

			std::string fpsText = "FPS: " + std::to_string(g_Data.getFPS());
			float lFPS = DrawUtils::getTextWidth(&fpsText, 1) + 6.5;
			//vec4_t fpsRectPos = vec4_t(fpsX, fpsY + 7, lFPS + fpsX, fpsY + 20);
			//vec2_t fpsTextPos = vec2_t(fpsRectPos.x + 3, fpsRectPos.y + 3.5);
			//DrawUtils::drawGradientText(fpsTextPos, &fpsText, 1, 1, true);
			float fpswit = DrawUtils::getTextWidth(&fpsText, 1.f);
			float techai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * 1.f;
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x + 3, windowSize.y - 36), &fpsText, MC_Color(interfaceCL), 1.f, 1.f, true);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x - 3, windowSize.y - 39, windowSize.x - windowSize.x + 5 + fpswit, windowSize.y - 38), MC_Color(interfaceCL), 1.f);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x + 4 + fpswit, windowSize.y - 39, windowSize.x - windowSize.x + 5 + fpswit, windowSize.y - 38 + techai), MC_Color(interfaceCL), 1.f);
			if (opacity != 0)
				DrawUtils::fillRectangleA(vec4_t(windowSize.x - windowSize.x - 3, windowSize.y - 38, windowSize.x - windowSize.x + 4 + fpswit, windowSize.y - 38 + techai), MC_Color(0, 0, 0, opacity));
			//DrawUtils::fillRoundRectangle(fpsRectPos, MC_Color(0, 0, 0, opacity), false);

			// Speed (BPS)
			auto player = g_Data.getLocalPlayer();
			std::string speedText = "Speed: " + std::to_string((int)player->getBlocksPerSecond()) + std::string(".") + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10));
			float lSpeed = DrawUtils::getTextWidth(&speedText, 1) + 6.5;
			//vec4_t speedRectPos = vec4_t(bpsX, bpsY + 6, lSpeed + bpsX, bpsY + 20);
			//vec2_t speedPos = vec2_t(speedRectPos.x + 3, speedRectPos.y + 3.5);
			//DrawUtils::drawGradientText(speedPos, &speedText, 1, 1, true);
			float speedwit = DrawUtils::getTextWidth(&speedText, 1.f);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x + 3, windowSize.y - 24), &speedText, MC_Color(interfaceCL), 1.f, 1.f, true);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x + 4 + fpswit, windowSize.y - 39 + techai, windowSize.x - windowSize.x + 5 + speedwit, windowSize.y - 38 + techai), MC_Color(interfaceCL), 1.f);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x + 4 + speedwit, windowSize.y - 38 + techai, windowSize.x - windowSize.x + 5 + speedwit, windowSize.y - 37 + 2 * techai), MC_Color(interfaceCL), 1.f);
			if (opacity != 0)
				DrawUtils::fillRectangleA(vec4_t(windowSize.x - windowSize.x - 3, windowSize.y - 38 + techai, windowSize.x - windowSize.x + 4 + speedwit, windowSize.y - 36 + 2 * techai), MC_Color(0, 0, 0, opacity));
			//DrawUtils::fillRoundRectangle(speedRectPos, MC_Color(0, 0, 0, opacity), false);

			// Position
			vec3_t* currPos = g_Data.getLocalPlayer()->getPos();
			std::string position = "Position: " + std::to_string((int)floorf(currPos->x)) + " " + std::to_string((int)floorf(currPos->y)) + " " + std::to_string((int)floorf(currPos->z));
			float lPos = DrawUtils::getTextWidth(&position, 1) + 6.5;
			float poswit = DrawUtils::getTextWidth(&position, 1.f);
			//vec4_t rectPos = vec4_t(posX, posY + 6, lPos + posX, posY + 20);
			//vec2_t textPos = vec2_t(rectPos.x + 3, rectPos.y + 3);
			//DrawUtils::drawGradientText(textPos, &position, 1, 1, true);
			DrawUtils::drawText(vec2_t(windowSize.x - windowSize.x + 3, windowSize.y - 12), &position, MC_Color(interfaceCL), 1.f, 1.f, true);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x + 4 + speedwit, windowSize.y - 37 + 2 * techai, windowSize.x - windowSize.x + 5 + poswit, windowSize.y - 36 + 2 * techai), MC_Color(interfaceCL), 1.f);
			DrawUtils::fillRectangle(vec4_t(windowSize.x - windowSize.x + 4 + poswit, windowSize.y - 36 + 2 * techai, windowSize.x - windowSize.x + 5 + poswit, windowSize.y), MC_Color(interfaceCL), 1.f);
			if (opacity != 0)
				DrawUtils::fillRectangleA(vec4_t(windowSize.x - windowSize.x, windowSize.y - techai - 6, windowSize.x - windowSize.x + 4 + poswit, windowSize.y), MC_Color(0, 0, 0, opacity));
			//DrawUtils::fillRoundRectangle(vec4_t(posX, posY + 6, lPos + posX, posY + 20), MC_Color(0, 0, 0, opacity), false);
		}
	}
}

void Interface::onDisable() {
	setEnabled(true);
}