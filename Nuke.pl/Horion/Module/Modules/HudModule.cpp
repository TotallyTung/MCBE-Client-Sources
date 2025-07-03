#include "HudModule.h"
static UIColor graycolor = UIColor(155, 155, 155);
HudModule::HudModule() : IModule(0, Category::CLIENT, "Displays Hud") {
	registerBoolSetting("ArmorHUD", &armorHud, armorHud);
	registerBoolSetting("Watermark", &watermark, watermark);
	registerBoolSetting("InvView", &inv, inv);
	registerBoolSetting("Welcomer", &wlc, wlc);
	registerBoolSetting("Compass", &compass, compass);
	registerBoolSetting("FPS", &fps, fps);
	registerBoolSetting("BPS", &bps, bps);
	registerBoolSetting("Pos", &showPos, showPos);

	registerFloatSetting("Inv-X", &this->invX, this->invX, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
	registerFloatSetting("Inv-Y", &this->invY, this->invY, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);

	registerFloatSetting("FPS-X", &this->fpsX, this->fpsX, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
	registerFloatSetting("FPS-Y", &this->fpsY, this->fpsY, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);

	registerFloatSetting("BPS-X", &this->bpsX, this->bpsX, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
	registerFloatSetting("BPS-Y", &this->bpsY, this->bpsY, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);

	registerFloatSetting("POS-X", &this->posX, this->posX, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.x);
	registerFloatSetting("POS-Y", &this->posY, this->posY, 0.f, Game.getClientInstance()->getGuiData()->windowSizeReal.y);

	registerIntSetting  ("Compass-Range", &this->range, this->range, 45, 180);

	registerFloatSetting("Opacity", &this->opacity, this->opacity, 0.f, 1.f);
	registerBoolSetting("Inv-Outline", &this->outline, this->outline);
	registerIntSetting("CompassRange", &range, range, 45, 180);
	registerEnumSetting("Welcomer", &WelcomeID, 0);
	WelcomeID.addEntry(w1.c_str(), 0);
	WelcomeID.addEntry(w2.c_str(), 1);
	WelcomeID.addEntry(w3.c_str(), 2);
	WelcomeID.addEntry(w4.c_str(), 3);
	WelcomeID.addEntry(w5.c_str(), 4);
	WelcomeID.addEntry(w6.c_str(), 5);
	WelcomeID.addEntry(w7.c_str(), 6);
}

HudModule::~HudModule() {
}

const char* HudModule::getModuleName() {
	return ("HUD");
}
void HudModule::onPostRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	MC_Color mainColor2 = moduleMgr->getModule<Colors>()->getColorV2();
	if (inv) {
		Vec2 windowSize = Game.getClientInstance()->getGuiData()->windowSize;
		Vec2 windowSizeReal = Game.getClientInstance()->getGuiData()->windowSizeReal;
		auto clickGUI = moduleMgr->getModule<ClickGuiMod>();
		if (!clickGUI->isEnabled())
		{
			if (Game.canUseMoveKeys())
				if (!(Game.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys()))
				{
					LocalPlayer* player = Game.getLocalPlayer();
					PlayerInventoryProxy* supplies2 = Game.getLocalPlayer()->getSupplies();
					Inventory* inv2 = supplies2->inventory;
					Game.getLocalPlayer()->getSupplies()->inventory;
					Vec2 invPos = Vec2(100.f, 100.f);
					Vec2 invPos2 = Vec2(100.f, 120.f);
					ItemStack* test = inv2->getItemStack(1);
					ItemStack* test2 = inv2->getItemStack(2);
					float testF = 100.f;
					float sideBars = 2.5f;
					float yVal = invY + sideBars;

					xVal = invX + sideBars;

					Vec4 breh = Vec4(invX, invY, invX + 153.f + sideBars + 2, invY + 58.f + sideBars);
					if (outline)
					{
						DrawUtils::drawRectangle(Vec4(breh.x, breh.y, breh.z, breh.w), MC_Color(mainColor2.r, mainColor2.g, mainColor2.b), 1.f, sideBars / 2);
					}
					DrawUtils::fillRectangle(breh, MC_Color(0.f, 0.f, 0.f), opacity);
					for (int j = 9; j < 18; j++)
					{
						if ((inv2->getItemStack(j)->item != nullptr))
						{
							DrawUtils::drawItem(inv2->getItemStack(j), Vec2(xVal, yVal + 0.f), 1.f, 1.f, false);
							char stackAmount = inv2->getItemStack(j)->count;
							std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
							if (inv2->getItemStack(j)->count > 1)
								DrawUtils::drawText(Vec2(xVal + 7.f, (yVal + 0.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
						}
						xVal += 17.f;
					}
					xVal -= 180.f;
					for (int j = 18; j < 27; j++)
					{
						if ((inv2->getItemStack(j)->item != nullptr))
						{
							DrawUtils::drawItem(inv2->getItemStack(j), Vec2(xVal + 27.f, yVal + 20.f), 1.f, 1.f, false);
							std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
							if (inv2->getItemStack(j)->count > 1)
								DrawUtils::drawText(Vec2(xVal + 34.f, (yVal + 20.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
						}
						xVal += 17.f;
					}
					xVal -= 180.f;
					for (int j = 27; j < 36; j++)
					{
						if ((inv2->getItemStack(j)->item != nullptr))
						{
							DrawUtils::drawItem(inv2->getItemStack(j), Vec2(xVal + 54.f, yVal + 40.f), 1.f, 1.f, false);
							std::string stackStr = std::to_string(inv2->getItemStack(j)->count);
							if (inv2->getItemStack(j)->count > 1)
								DrawUtils::drawText(Vec2(xVal + 61.f, (yVal + 40.f) + 10.f), &stackStr, MC_Color(255, 255, 255), 1.f - .20f, 1.f, true);
						}
						xVal += 17.f;
					}
					xVal -= 180.f;
				}
		}
	}

	if (armorHud) {
		static float constexpr scale = 1.f;
		static float constexpr opacity = 0.25f;
		static float constexpr spacing = scale + 20.f;
		float x = windowSize.x / 2.f + 9.5f;
		float y = windowSize.y - 57.5f;
		if (localPlayer->isInWater() || localPlayer->isInWall()) y -= 10.f;
		for (int i = 0; i < 4; i++) {
			ItemStack* stack = localPlayer->getArmor(i);
			if (stack->isValid()) {
				DrawUtils::drawItem(stack, Vec2(x, y), 1.f);
				DrawUtils::drawBarItem(stack, Vec2(x, y), 1.f);
				x += scale * spacing;
			}
		}
	}
}
void HudModule::onImGuiRender(ImDrawList* drawlist) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (player == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	UIColor mainColor = moduleMgr->getModule<Colors>()->getColor();
	UIColor extraColor = UIColor(255, 255, 255);	
	Vec2 windowSize = Game.getGuiData()->windowSizeReal;
	static const float textSize = 1.f;
	static const float textPadding = 1.f * textSize;
	static const float textHeight = ImGuiUtils::getTextHeight(textSize);
	static const float x = 2.f;
	float y = windowSize.y - textHeight - textPadding;
	std::string playerNameReal = player->getNameTag()->getText();
	Vec2 pos = Vec2(windowSize.x / 2.25, 0.f);

	switch (WelcomeID.getSelectedValue()) {
	default:
		name = "Welcome " + playerNameReal;
		pos = Vec2(windowSize.x / 2.25, 0.f);
		break;

	case 1:
		name = "Welcome to " + Utils::getClientName() + " " + playerNameReal;
		pos = Vec2(windowSize.x / 2.35, 0.f);
		break;

	case 2:
		name = Utils::getClientName() + " " + playerNameReal;
		pos = Vec2(windowSize.x / 2.25, 0.f);
		break;

	case 3:
		name = "Xin Chao " + playerNameReal;
		pos = Vec2(windowSize.x / 2.25, 0.f);
		break;
	case 4:
		name = "Star's blessing to " + playerNameReal;
		pos = Vec2(windowSize.x / 2.35, 0.f);
		break;
	case 5:
		name = playerNameReal + " on top";
		pos = Vec2(windowSize.x / 2.25, 0.f);
		break;
	case 6:
		name = "Meow " + playerNameReal;
		pos = Vec2(windowSize.x / 2.15, 0.f);
		break;

	}
	if (fps) {
		float f = 10.f * 1.f;
		float yVal = fpsY;
		float xVal = fpsX;
		std::string Text = "FPS";
		std::string fps = std::to_string(Game.getFPS());
		Vec2 textPos = Vec2(xVal, yVal);
		Vec2 textPos2 = Vec2(xVal + 40, yVal);
		ImGuiUtils::drawText(textPos, Text.c_str(), graycolor, 1.f, 1.f);
		ImGuiUtils::drawText(textPos2, fps.c_str(), extraColor, 1.f, 1.f);
		yVal += f;
	}

	if (bps) {
		float b = 10.f * 1.f;

		float yVal = bpsY;
		float xVal = bpsX;
		std::string text1 = "Speed";
		std::string text2 = std::to_string((int)player->getBlocksPerSecond()) + "." + std::to_string((int)(player->getBlocksPerSecond() * 10) - ((int)player->getBlocksPerSecond() * 10))+"m/s";
		Vec2 textPos1 = Vec2(xVal, yVal);
		Vec2 textPos2 = Vec2(xVal+70, yVal);
		Vec2 textPos3 = Vec2(xVal+110+ImGuiUtils::getTextWidth(text2.c_str(), 1.f) + 10, yVal);
		ImGuiUtils::drawText(textPos1, text1.c_str(), graycolor, 1.f, 1.f);
		ImGuiUtils::drawText(textPos2, text2.c_str(), extraColor, 1.f, 1.f);
		yVal += b;
	}
	if (showPos) {
		int curDim = 0;
		player->getDimensionId(&curDim);
		const Vec3 pos = player->eyePos0;
		char extraText2[50];
		char extraText3[50];
		std::string owText = "Overworld XYZ ";
		if (curDim == 1) {
			sprintf_s(extraText2, 50, "%.1f %.1f, %.1f", pos.x * 8, pos.y, pos.z * 8);
		}
		else if (curDim == 0) {
			sprintf_s(extraText2, 50, "%.1f %.1f %.1f", pos.x, pos.y, pos.z);
		}

		std::string nthText = "Nether XYZ ";
		if (curDim == 0) {
			sprintf_s(extraText3, 50, "%.1f %.1f %.1f", pos.x / 8, pos.y, pos.z / 8);
		}
		else if (curDim == 1) {
			sprintf_s(extraText3, 50, "%.1f %.1f %.1f", pos.x, pos.y, pos.z);
		}


		ImGuiUtils::drawText(Vec2(posX, posY), owText.c_str(), graycolor, textSize);
		ImGuiUtils::drawText(Vec2(posX, posY - 20), nthText.c_str(), graycolor, textSize);
		ImGuiUtils::drawText(Vec2(posX + ImGuiUtils::getTextWidth(owText.c_str(), textSize), posY), extraText2, extraColor, textSize);
		ImGuiUtils::drawText(Vec2(posX + ImGuiUtils::getTextWidth(nthText.c_str(), textSize), posY - 20), extraText3, extraColor, textSize);
	};
	if (watermark) {
		static const std::string version = " - " + modversion + type;
		ImGuiUtils::drawText(Vec2(windowSize.x - windowSize.x, 0.f), Utils::getClientName().c_str(), mainColor, textSize);
		ImGuiUtils::drawText(Vec2(windowSize.x - windowSize.x + ImGuiUtils::getTextWidth(Utils::getClientName().c_str(), textSize), 0.f), version.c_str(), extraColor, textSize);
	};
	if (wlc) {
		playerName = player->getNameTag()->getText();
		ImGuiUtils::drawText(pos, name.c_str(), mainColor, textSize);
	};
}
void HudModule::onPreRender(MinecraftUIRenderContext* renderCtx) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (!compass) return;
	if (player == nullptr || !GameData::canUseMoveKeys()) return; 
	auto extraPoints = std::multimap<int, std::string>{};

	std::vector<Vec2> stacking{};

	const int deg = (int)(player->yaw + 180);
	const float degSubOffset = 0;  // -fmodf(player->yaw, 1)
	const float sCenter = Game.getGuiData()->widthGame / 2;

	for (int off = -range; off <= range; off++) {
		int oDeg = (deg + off) % 360;
		if (oDeg < 0) oDeg += 360;

		const float xOff = sCenter + off + degSubOffset;
		constexpr float fadeOutPos = 0.25f;
		constexpr float centerCutoff = 5, centerFadeout = 10;
		float majorOpacity = opacity;  // big compass text
		float minorOpacity = majorOpacity;   // minor features, degree numbers

		// Fading logic
		{
			if ((range - abs(off)) < range * fadeOutPos)  // Far from center
				minorOpacity = majorOpacity = lerp(0, opacity, (range - abs(off)) / (range * fadeOutPos));
			else if (abs(off) < centerFadeout)  // Close to center
				minorOpacity = lerp(0, opacity, (abs(off) - centerCutoff) / (centerFadeout - centerCutoff));
		}
		switch (oDeg) {
		case 0:
			drawCenteredText(Vec2(xOff, 30.f), "N", 1, majorOpacity);
			break;
		case 45:
			drawCenteredText(Vec2(xOff, 30.f), "NE", 1, majorOpacity);
			break;
		case 90:
			drawCenteredText(Vec2(xOff, 30.f), "E", 1, majorOpacity);
			break;
		case 135:
			drawCenteredText(Vec2(xOff, 30.f), "SE", 1, majorOpacity);
			break;
		case 180:
			drawCenteredText(Vec2(xOff, 30.f), "S", 1, majorOpacity);
			break;
		case 225:
			drawCenteredText(Vec2(xOff, 30.f), "SW", 1, majorOpacity);
			break;
		case 270:
			drawCenteredText(Vec2(xOff, 30.f), "W", 1, majorOpacity);
			break;
		case 315:
			drawCenteredText(Vec2(xOff, 30.f), "NW", 1, majorOpacity);
			break;
		}
		auto result = extraPoints.equal_range(oDeg);
		for (auto it = result.first; it != result.second; it++) {
			std::string pName = it->second;
			std::transform(pName.begin(), pName.end(), pName.begin(), ::toupper);

			Vec2 pos = Vec2(xOff, 31.f);

			int overlapping = 0;
			const float tSize = 0.75f;
			const float tWidth = DrawUtils::getTextWidth(&pName, tSize);
			pos.x -= tWidth / 2;
			const Vec2 myTextRange = Vec2(pos.x, pos.x + tWidth);
			for (const Vec2 otherTextRange : stacking) {
				// Check if other text overlaps us
				if (myTextRange.x < otherTextRange.y && otherTextRange.x < myTextRange.y) {
					overlapping++;
				}
			}
			stacking.push_back(myTextRange);
			pos.y += 5 * (overlapping + 1);
			DrawUtils::drawText(pos, &pName, MC_Color(255, 255, 255), tSize, majorOpacity);
		}

		if (off != 0 && minorOpacity > 0) {
			if ((oDeg % 15) != 0) {
				if ((oDeg % 5) == 0) {
					DrawUtils::fillRectangle(Vec4(xOff - 0.25f, 16, xOff + 0.25f, 19), MC_Color(255, 255, 255), minorOpacity);
				}
				continue;
			}

			// Bigger line with degree displayed
			DrawUtils::fillRectangle(Vec4(xOff - 0.5f, 15, xOff + 0.5f, 20), MC_Color(255, 255, 255), minorOpacity);
			drawCenteredText(Vec2(xOff, 20.f), std::to_string(oDeg), 0.75f, minorOpacity);
		}
	}

	// Center line
	DrawUtils::fillRectangle(Vec4(sCenter - 0.5f, 15, sCenter + 0.5f, 25), MC_Color(255, 255, 255), opacity);
	drawCenteredText(Vec2(sCenter, 25.f), std::to_string(deg), 0.75f, opacity);
	DrawUtils::flush();
}

void HudModule::drawCenteredText(Vec2 pos, std::string text, float size, float textOpacity) {
	pos.x -= DrawUtils::getTextWidth(&text, size) / 2;
	DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, textOpacity);
}
void HudModule::drawcText(Vec2 pos, std::string text, float size, float textOpacity) {
	//DrawUtils::drawText(pos, &text, MC_Color(255, 255, 255), size, textOpacity);
	return;
}
