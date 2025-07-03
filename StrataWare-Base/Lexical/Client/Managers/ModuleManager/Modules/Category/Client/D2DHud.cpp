#include "D2DHUD.h"
#include "../../../ModuleManager.h"
int sortingmode = 0;
UIColor mainTTTXXXTTTCOLOR = (0, 0, 0, 255);
HUD::HUD() : Module("D2DHud", "Displays HUD Elements in D2D", Category::CLIENT) {
	registerSetting(new ColorSetting("Color", "NULL", &mainTTTXXXTTTCOLOR, UIColor(0, 101, 90), true));
	registerSetting(new ToggleSetting("Position", "NULL", &showPosition, true));
	registerSetting(new ToggleSetting("Direction", "NULL", &showDirection, false));
	registerSetting(new ToggleSetting("Effects", "NULL", &showEffects, true));
	registerSetting(new ToggleSetting("ArrayList", "", &arraylist, true));
	registerSetting(new NumberSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
	registerSetting(new NumberSetting<int>("Spacing", "NULL", &spacing, 0, -3, 3));
	registerSetting(new NumberSetting<int>("Offset", "NULL", &offset, 10, 0, 30));
	registerSetting(new NumberSetting<int>("Size", "NULL", &size, 10, 0, 30));
	registerSetting(new ToggleSetting("ArrayListBottom", "", &bottom, true));
	registerSetting(new ToggleSetting("ArrayListShow Modes", "", &showModes, true));
	registerSetting(new EnumSetting("ArrayList Mode", "Sorting mode", { "Alphabet", "Length" }, &sortingmode, 0));
}

HUD::~HUD() {
}


void HUD::onD2DRender() {
	LocalPlayer* localPlayer = Game::ourPlayer();
	if (localPlayer == nullptr)
		return;
	Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
	Vec2<float> windowSizeScaled = Game::clientInstance->guiData->windowSizeScaled;
	float textSize = 1.f;
	float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
	float textPaddingY = (float)spacing * textSize;
	float textHeight = D2D::getTextHeight("", textSize);
	float posX = 0.f + (float)offset;
	float posY = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;
	if (showPosition) {
		Vec3<float> lpPos = localPlayer->getPos();
		int dimensionId = localPlayer->getDimensionTypeComponent()->type;

		if (dimensionId == 0) {
			char coords[40];
			sprintf_s(coords, 40, "%.1f, %.1f, %.1f", (lpPos.x / 8.f), (lpPos.y - 1.6f), (lpPos.z / 8.f));
			std::string label = "Nether: ";
			float labelWidth = D2D::getTextWidth(label, textSize, false);
			float coordsWidth = D2D::getTextWidth(std::string(coords), textSize, false);

			D2D::fillRectangle(Vec4<float>(posX, posY, posX + labelWidth + coordsWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
			D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainTTTXXXTTTCOLOR, textSize, false);
			D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coords, UIColor(160, 160, 160, mainTTTXXXTTTCOLOR.a), textSize, false);
			posY -= textHeight + textPaddingY * 2.f;
		}
		else if (dimensionId == 1) {
			char coords[40];
			sprintf_s(coords, 40, "%.1f, %.1f, %.1f", (lpPos.x * 8.f), (lpPos.y - 1.6f), (lpPos.z * 8.f));
			std::string label = "Overworld: ";
			float labelWidth = D2D::getTextWidth(label, textSize, false);
			float coordsWidth = D2D::getTextWidth(std::string(coords), textSize, false);

			D2D::fillRectangle(Vec4<float>(posX, posY, posX + labelWidth + coordsWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
			D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainTTTXXXTTTCOLOR, textSize, false);
			D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coords, UIColor(160, 160, 160, mainTTTXXXTTTCOLOR.a), textSize, false);
			posY -= textHeight + textPaddingY * 2.f;
		}

		char coords[40];
		sprintf_s(coords, 40, "%.1f, %.1f, %.1f", lpPos.x, (lpPos.y - 1.6f), lpPos.z);
		std::string label = "Position: ";
		float labelWidth = D2D::getTextWidth(label, textSize, false);
		float coordsWidth = D2D::getTextWidth(std::string(coords), textSize, false);

		D2D::fillRectangle(Vec4<float>(posX, posY, posX + labelWidth + coordsWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainTTTXXXTTTCOLOR, textSize, false);
		D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coords, UIColor(160, 160, 160, mainTTTXXXTTTCOLOR.a), textSize, false);
		posY -= textHeight + textPaddingY * 2.f;
	}

	if (showDirection) {
		float rotation = localPlayer->rotation->presentRot.y;
		if (rotation < 0) {
			rotation += 360.0;
		}
		std::string facing;
		if ((0 <= rotation && rotation < 45) || (315 <= rotation && rotation < 360)) {
			facing = "South (+Z)";
		}
		else if (45 <= rotation && rotation < 135) {
			facing = "West (-X)";
		}
		else if (135 <= rotation && rotation < 225) {
			facing = "North (-Z)";
		}
		else if (225 <= rotation && rotation < 315) {
			facing = "East (+X)";
		}
		else {
			facing = "NULL";
		}

		std::string label = "Direction: ";
		float labelWidth = D2D::getTextWidth(label, textSize);
		float facingWidth = D2D::getTextWidth(facing, textSize);

		D2D::fillRectangle(Vec4<float>(posX, posY, posX + labelWidth + facingWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainTTTXXXTTTCOLOR, textSize);
		D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), facing, UIColor(160, 160, 160, mainTTTXXXTTTCOLOR.a), textSize);

		posY -= textHeight + textPaddingY * 2.f;
	}


	if (showEffects) {
		Vec2<float> EffectPos = Vec2<float>(windowSize.x - (float)offset, (float)offset);
		if (!bottom)
			EffectPos.y = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;
		for (uint32_t effectId = 1; effectId < 37; effectId++) {
			MobEffect* mEffect = MobEffect::getById(effectId);
			if (mEffect == nullptr)
				continue;
			if (localPlayer->hasEffect(mEffect)) {
				MobEffectInstance* mEffectInstance = localPlayer->getEffect(mEffect);
				if (mEffectInstance == nullptr)
					continue;
				std::string text1 = mEffectInstance->getDisplayName() + " ";
				std::string text2 = getEffectTimeLeftStr(mEffectInstance);
				std::string fullText = text1 + text2;

				Vec4<float> eRectPos = Vec4<float>(EffectPos.x - D2D::getTextWidth(fullText, textSize, false) - (textPaddingX * 2.f),
					EffectPos.y,
					EffectPos.x,
					EffectPos.y + textHeight + (textPaddingY * 2.f));

				Vec2<float> eTextPos = Vec2<float>(eRectPos.x + textPaddingX, eRectPos.y + textPaddingY);
				Vec2<float> eDurationTextPos = Vec2<float>(eTextPos.x + D2D::getTextWidth(text1, textSize, true), eTextPos.y);

				D2D::fillRectangle(eRectPos, UIColor(0, 0, 0, opacity));
				D2D::drawText(eTextPos, text1, mEffect->color, textSize, true);
				D2D::drawText(eDurationTextPos, text2, mainTTTXXXTTTCOLOR, textSize, false);

				EffectPos.y -= (textHeight + (textPaddingY * 2.f)) * (bottom ? -1.f : 1.f);
			}
		}
	}
	if (arraylist) {
		LocalPlayer* localPlayer = Game::ourPlayer();
		if (!localPlayer) return;

		Vec2<float> winSize = Game::clientInstance->guiData->windowSizeReal;
		float textSize = size;
		float padding = 2.0f * textSize;
		float textHeight = D2D::getTextHeight("", textSize);
		Vec2<float> pos(winSize.x - offset, offset);

		if (bottom)
			pos.y = winSize.y - offset;

		auto modules = ModuleManager::moduleList;
		if (sortingmode == 0) {
			std::sort(modules.begin(), modules.end(), [](Module* a, Module* b) {
				return a->getModuleName() < b->getModuleName();
				});
		}
		else {
			std::sort(modules.begin(), modules.end(), [this](Module* a, Module* b) {
				float widthA = D2D::getTextWidth(a->getModuleName(), size);
				float widthB = D2D::getTextWidth(b->getModuleName(), size);
				if (showModes) {
					if (a->getModeText() != "NULL")
						widthA += D2D::getTextWidth(" " + a->getModeText(), size);
					if (b->getModeText() != "NULL")
						widthB += D2D::getTextWidth(" " + b->getModeText(), size);
				}
				return widthA > widthB;
				});
		}

		for (auto& mod : modules) {
			if (!mod->isEnabled() || !mod->isVisible()) continue;

			std::string name = mod->getModuleName();
			std::string mode = showModes ? mod->getModeText() : "NULL";
			float fullWidth = D2D::getTextWidth(name, textSize);

			if (mode != "NULL")
				fullWidth += D2D::getTextWidth(" " + mode, textSize);

			float height = textHeight + padding * 2;
			pos.y += bottom ? -height : 0;

			Vec4<float> rect(pos.x - fullWidth - padding * 2, pos.y, pos.x, pos.y + height);
			D2D::fillRectangle(rect, UIColor(10, 10, 10, opacity));

			Vec2<float> textPos(rect.x + padding, rect.y + padding);
			D2D::drawText(textPos, name, mainTTTXXXTTTCOLOR, textSize, true);

			if (mode != "NULL") {
				textPos.x += D2D::getTextWidth(name + " ", textSize);
				D2D::drawText(textPos, mode, UIColor(160, 160, 160), textSize, true);
			}

			if (!bottom)
				pos.y += height;
		}
	}
}
