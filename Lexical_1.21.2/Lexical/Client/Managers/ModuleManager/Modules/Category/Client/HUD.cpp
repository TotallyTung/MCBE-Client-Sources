#include "HUD.h"
#include "../../../ModuleManager.h"

HUD::HUD() : Module("HUD", "Displays HUD Elements", Category::CLIENT) {
	registerSetting(new BoolSetting("Position", "NULL", &showPosition, true));
	registerSetting(new BoolSetting("Direction", "NULL", &showDirection, false));
	registerSetting(new BoolSetting("Effects", "NULL", &showEffects, true));
	registerSetting(new BoolSetting("Watermark", "NULL", &waterMark, true));
	//
	registerSetting(new SliderSetting<int>("Opacity", "NULL", &opacity, 130, 0, 255));
	registerSetting(new SliderSetting<int>("Spacing", "NULL", &spacing, -2, -3, 3));
	registerSetting(new SliderSetting<int>("Offset", "NULL", &offset, 10, 0, 30));
}

HUD::~HUD() {
}

void HUD::onD2DRender() {
	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr)
		return;

	Vec2<float> windowSize = Game::clientInstance->guiData->windowSizeReal;
	Vec2<float> windowSizeScaled = Game::clientInstance->guiData->windowSizeScaled;
	static Colors* colorMod = ModuleManager::getModule<Colors>();

	int index = 0;
	float textSize = 1.f;
	float textPaddingX = std::max(1.f, ((float)spacing + 3.f)) * textSize;
	float textPaddingY = (float)spacing * textSize;
	float textHeight = D2D::getTextHeight("", textSize);
	float posX = 0.f + (float)offset;
	float posY = windowSize.y - (textHeight + textPaddingY * 2.f) - (float)offset;

	if (waterMark) {
		UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());
		D2D::fillRectangle(Vec4<float>(offset, offset, offset + D2D::getTextWidth(std::string("Lexical"), textSize, false) + textPaddingX * 2.f, offset + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(offset, offset), std::string("Lexical"), mainColor, textSize, false);
		index++;
	}

	if (showPosition) {
		Vec3<float> lpPos = localPlayer->getPos();
		int dimensionId = localPlayer->getDimensionTypeComponent()->type;
		UIColor grayColor(175, 175, 175, 255);

		if (dimensionId == 0) {
			char coordsText[40];
			sprintf_s(coordsText, 40, "%.1f, %.1f, %.1f", (lpPos.x / 8.f), (lpPos.y - 1.6f), (lpPos.z / 8.f));
			std::string label = "Nether: ";
			std::string fullText = label + coordsText;

			UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());
			float fullWidth = D2D::getTextWidth(fullText, textSize, false);
			D2D::fillRectangle(Vec4<float>(posX, posY, posX + fullWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
			D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainColor, textSize, false);
			float labelWidth = D2D::getTextWidth(label, textSize, false);
			D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coordsText, grayColor, textSize, false);

			posY -= textHeight + (textPaddingY * 2.f);
			index++;
		}
		else if (dimensionId == 1) {
			char coordsText[40];
			sprintf_s(coordsText, 40, "%.1f, %.1f, %.1f", (lpPos.x * 8.f), (lpPos.y - 1.6f), (lpPos.z * 8.f));
			std::string label = "Overworld: ";
			std::string fullText = label + coordsText;

			UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());
			float fullWidth = D2D::getTextWidth(fullText, textSize, false);
			D2D::fillRectangle(Vec4<float>(posX, posY, posX + fullWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
			D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainColor, textSize, false);
			float labelWidth = D2D::getTextWidth(label, textSize, false);
			D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coordsText, grayColor, textSize, false);

			posY -= textHeight + (textPaddingY * 2.f);
			index++;
		}

		char coordsText[40];
		sprintf_s(coordsText, 40, "%.1f, %.1f, %.1f", lpPos.x, (lpPos.y - 1.6f), lpPos.z);
		std::string label = "Position: ";
		std::string fullText = label + coordsText;

		UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());
		float fullWidth = D2D::getTextWidth(fullText, textSize, false);
		D2D::fillRectangle(Vec4<float>(posX, posY, posX + fullWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainColor, textSize, false);
		float labelWidth = D2D::getTextWidth(label, textSize, false);
		D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), coordsText, grayColor, textSize, false);

		posY -= textHeight + (textPaddingY * 2.f);
		index++;
	}

	if (showDirection) {
		float rotation = localPlayer->rotation->presentRot.y;
		if (rotation < 0) {
			rotation += 360.0;
		}

		std::string directionValue;
		if ((0 <= rotation && rotation < 45) || (315 <= rotation && rotation < 360)) {
			directionValue = "South (+Z)";
		}
		else if (45 <= rotation && rotation < 135) {
			directionValue = "West (-X)";
		}
		else if (135 <= rotation && rotation < 225) {
			directionValue = "North (-Z)";
		}
		else if (225 <= rotation && rotation < 315) {
			directionValue = "East (+X)";
		}
		else {
			directionValue = "NULL";
		}

		std::string label = "Direction: ";
		std::string fullText = label + directionValue;
		UIColor grayColor(175, 175, 175, 255);
		UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());

		float fullWidth = D2D::getTextWidth(fullText, textSize, false);
		D2D::fillRectangle(Vec4<float>(posX, posY, posX + fullWidth + textPaddingX * 2.f, posY + textHeight + textPaddingY * 2.f), UIColor(0, 0, 0, opacity));
		D2D::drawText(Vec2<float>(posX + textPaddingX, posY + textPaddingY), label, mainColor, textSize, false);
		float labelWidth = D2D::getTextWidth(label, textSize, false);
		D2D::drawText(Vec2<float>(posX + textPaddingX + labelWidth, posY + textPaddingY), directionValue, grayColor, textSize, false);

		posY -= textHeight + (textPaddingY * 2.f);
		index++;
	}

	if (showEffects) {
		static Arraylist* arrayListMod = ModuleManager::getModule<Arraylist>();

		Vec2<float> EffectPos = Vec2<float>(windowSize.x - (float)offset, (float)offset);
		if (!arrayListMod->bottom)
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

				Vec4<float> eRectPos = Vec4<float>(
					EffectPos.x - D2D::getTextWidth(fullText, textSize, false) - (textPaddingX * 2.f),
					EffectPos.y,
					EffectPos.x,
					EffectPos.y + textHeight + (textPaddingY * 2.f));

				Vec2<float> eTextPos = Vec2<float>(eRectPos.x + textPaddingX, eRectPos.y + textPaddingY);
				Vec2<float> eDurationTextPos = Vec2<float>(eTextPos.x + D2D::getTextWidth(text1, textSize, true), eTextPos.y);

				UIColor mainColor = colorMod->getColor(index * colorMod->getSeperationValue());

				D2D::fillRectangle(eRectPos, UIColor(0, 0, 0, opacity));
				D2D::drawText(eTextPos, text1, mEffect->color, textSize, true);
				D2D::drawText(eDurationTextPos, text2, mainColor, textSize, false);

				EffectPos.y -= (textHeight + (textPaddingY * 2.f)) * (arrayListMod->bottom ? -1.f : 1.f);
				index++;
			}
		}
	}
}
