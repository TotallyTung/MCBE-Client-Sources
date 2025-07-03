// Created by Tony on 2024-10-10 10:01:54

#include "HUD.h"
#include "../../../Client.h"

HUD::HUD() : Module("HUD", "Shows Client Hud.", Category::CLIENT) {
	//addBoolCheck("Swearing", "Say nigga without #", &SAYNIGGGA);
	addBoolCheck("FPS", "Displays your fps.", &showFps);
	addBoolCheck("Position", "Displays your current position.", &showCoords);
	addBoolCheck("BPS", "Displays your speed.", &showSpeed);
	addBoolCheck("Effects", "NULL", &showEffects);
	addBoolCheck("Hotbar", "Lerp between your hotbar position", &Hotbar);
	effectList.emplace_back(new EffectHud("Speed", UIColor(124, 175, 198), 1));
	effectList.emplace_back(new EffectHud("Slowness", UIColor(90, 108, 129), 2));
	effectList.emplace_back(new EffectHud("Haste", UIColor(217, 192, 67), 3));
	effectList.emplace_back(new EffectHud("Mining Fatigue", UIColor(74, 66, 23), 4));
	effectList.emplace_back(new EffectHud("Strength", UIColor(147, 36, 35), 5));
	// 6, 7 is INSTANT_HEALTH and INSTANT_DAMAGE
	effectList.emplace_back(new EffectHud("Jump Boost", UIColor(34, 255, 76), 8));
	effectList.emplace_back(new EffectHud("Nausa", UIColor(85, 29, 74), 9));
	effectList.emplace_back(new EffectHud("Regeneration", UIColor(205, 92, 171), 10));
	effectList.emplace_back(new EffectHud("Resistance", UIColor(153, 69, 58), 11));
	effectList.emplace_back(new EffectHud("Fire Resistance", UIColor(228, 154, 58), 12));
	effectList.emplace_back(new EffectHud("Water Breathing", UIColor(46, 82, 153), 13));
	effectList.emplace_back(new EffectHud("Invisibility", UIColor(127, 131, 146), 14));
	effectList.emplace_back(new EffectHud("Blindness", UIColor(31, 31, 35), 15));
	effectList.emplace_back(new EffectHud("Night Vision", UIColor(31, 31, 161), 16));
	effectList.emplace_back(new EffectHud("Hunger", UIColor(88, 118, 83), 17));
	effectList.emplace_back(new EffectHud("Weakness", UIColor(72, 77, 72), 18));
	effectList.emplace_back(new EffectHud("Poison", UIColor(78, 147, 49), 19));
	effectList.emplace_back(new EffectHud("Wither", UIColor(53, 42, 39), 20));
	effectList.emplace_back(new EffectHud("Health Boost", UIColor(248, 125, 35), 21));
	effectList.emplace_back(new EffectHud("Absorption", UIColor(37, 82, 165), 22));
	addSlider<float>("Size", "Changes HUD size", ValueType::FLOAT_T, &size, 0.5f, 1.5f);

	//addColorPicker("Color", "NULL", &color);
}

HUD::~HUD() {
	for (EffectHud* effectHud : effectList) {
		delete effectHud;
	}
}

std::string HUD::intToRoman(int num) {
	static const std::vector<int> values = { 5, 4, 1 };
	static const std::vector<std::string> numerals = {  "V", "IV", "I" };

	std::string result = "";
	for (int i = 0; i < values.size(); i++) {
		while (num >= values[i]) {
			num -= values[i];
			result += numerals[i];
		}
	}

	return result;
}

bool HUD::sortByAlphabet(EffectHud*& a1, EffectHud*& a2) {
	return a1->getEffectName() < a2->getEffectName();
}

Vec3<float> posPrev;
float currentBPS = 0;

void HUD::onNormalTick(Actor* actor) {
	auto clientinstnce = mc.getClientInstance();
	/*if (SAYNIGGGA) {
		clientinstnce->getProfanityContext()->enabled = true;
	}
	else {
		clientinstnce->getProfanityContext()->enabled = false;
	}*/
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer == nullptr) return;
	float tps = clientinstnce->minecraft->mctimer->tps;
	Vec3<float> pos = *localPlayer->getPosition();
	currentBPS = sqrt((pos.x - posPrev.x) * (pos.x - posPrev.x) +
		(pos.y - posPrev.y) * (pos.y - posPrev.y) +
		(pos.z - posPrev.z) * (pos.z - posPrev.z));
	currentBPS *= tps;
	posPrev = pos;
}

void HUD::onImGuiRender(ImDrawList* drawlist) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer == nullptr) return;
	if (!mc.canUseMoveKeys()) return;

	static ArrayList* arrayMod = (ArrayList*)client->moduleMgr->getModule("ArrayList");
	static AutoCrystal* AutoCrystalMod = (AutoCrystal*)client->moduleMgr->getModule("AutoCrystal");
	ImGuiIO& io = ImGui::GetIO();

	float textHeight = ImGuiUtils::getTextHeight(size);
	float textPadding = 1.f * size;
	float spacing = textHeight - 2.f;

	Vec2<float> windowSize(ImGui::GetIO().DisplaySize.x, ImGui::GetIO().DisplaySize.y);
	const float xOffset = 3.f;
	float yOffset = windowSize.y - textHeight - textPadding;

	float effectPosX = ImGui::GetIO().DisplaySize.x - 1.f;
	float effectPosY = textPadding;
	if (!arrayMod->bottom)
		effectPosY = windowSize.y - textHeight - textPadding;

	static UIColor whiteColor = UIColor(255, 255, 255, 255);
	static UIColor semiGrayColor = UIColor(200, 200, 200, 255);

	if (showCoords) {
		ImGuiUtils::drawText(Vec2<float>(xOffset, yOffset), "XYZ ", whiteColor, size);
		Vec3<float> pos = *localPlayer->getPosition();
		std::string xyzText = std::to_string((int)floorf(pos.x)) + std::string(" ") + std::to_string((int)floorf(pos.y)) + std::string(" ") + std::to_string((int)floorf(pos.z));
		ImGuiUtils::drawText(Vec2<float>(xOffset + ImGuiUtils::getTextWidth("XYZ ", size), yOffset), xyzText, semiGrayColor, size);
		yOffset -= textHeight + textPadding;
	}
	if (showSpeed) {
		ImGuiUtils::drawText(Vec2<float>(xOffset, yOffset), "BPS ", whiteColor, size);
		static char bpsText[30];
		sprintf_s(bpsText, 30, "%.1fm/s", currentBPS);
		ImGuiUtils::drawText(Vec2<float>(xOffset + ImGuiUtils::getTextWidth("BPS ", size), yOffset), bpsText, semiGrayColor, size);
		yOffset -= textHeight + textPadding;
	}
	if (showFps) {
		ImGuiUtils::drawText(Vec2<float>(xOffset, yOffset), "FPS ", whiteColor, size);
		ImGuiUtils::drawText(Vec2<float>(xOffset + ImGuiUtils::getTextWidth("FPS ", size), yOffset), std::to_string(int(ImGui::GetIO().Framerate)), semiGrayColor, size);
		yOffset -= textHeight + textPadding;
	}
	if (AutoCrystalMod->Crystalcounter) {
		ImGuiUtils::drawText(Vec2<float>(xOffset, yOffset), "Crystal ", whiteColor, size);
		char message[256];
		sprintf(message, "speed : %i", AutoCrystalMod->crystalSpeed);
		ImGuiUtils::drawText(Vec2<float>(xOffset + ImGuiUtils::getTextWidth("Crystal ", size), yOffset), message, semiGrayColor, size);
		yOffset -= textHeight + textPadding;
	}
	if (showEffects) {
		static bool sorted = false;
		if (!sorted) {
			std::sort(effectList.begin(), effectList.end(), HUD::sortByAlphabet);
			sorted = true;
		}
		for (EffectHud*& effectHud : effectList) {
			if (effectHud->getEffect() != nullptr && effectHud->getEffectTimeLeft() != 0) {
				std::string text1 = effectHud->getEffectName();
				if (effectHud->getEffectLevel() != 0)
				{
					if (effectHud->getEffectLevel() > 5)
					{
						text1 += " " + std::string("V");
					}
					else
					{
						text1 += " " + intToRoman(effectHud->getEffectLevel() + 1);
					}
					
				}
				text1 += " ";
				std::string text2 = effectHud->getEffectTimeLeftStr();
				std::string text3 = text1 + text2;
				ImGuiUtils::drawText(Vec2<float>(effectPosX - ImGuiUtils::getTextWidth(text3, size), effectPosY), text1, effectHud->getColor(), size);
				ImGuiUtils::drawText(Vec2<float>(effectPosX - ImGuiUtils::getTextWidth(text2, size), effectPosY), text2, whiteColor, size);
				if (!arrayMod->bottom)
					effectPosY -= spacing;
				else
					effectPosY += spacing;
			}
		}
	}
}
