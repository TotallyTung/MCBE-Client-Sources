#include "HUD.h"
#include "../../../Client.h"

HUD::HUD() : Module("Hud", "Shows Client Hud.", Category::CLIENT) {
	//boolsetting("Swearing", "Say nigga without #", &SAYNIGGGA);
	boolsetting("FPS", "Displays your fps.", &showFps);
	boolsetting("Position", "Displays your current position.", &showCoords);
	boolsetting("BPS", "Displays your speed.", &showSpeed);
	boolsetting("Effects", "NULL", &showEffects);
	boolsetting("Hotbar", "Lerp between your hotbar position", &Hotbar);
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
	slider<float>("Size", "Changes HUD size", ValueType::FLOAT_T, &size, 0.5f, 1.5f);

	//colorsetting("Color", "NULL", &color);
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
		float textScale = size;
		Vec3<float> pos = *localPlayer->getPosition();
		std::string xyzText = std::to_string((int)floorf(pos.x)) + std::string(" ") + std::to_string((int)floorf(pos.y)) + std::string(" ") + std::to_string((int)floorf(pos.z));
		float xyzTextWidth = ImGuiUtils::getTextWidth(xyzText, textScale);
		Vec3<float> netherPos = { pos.x / 8.0f, pos.y / 8.0f, pos.z / 8.0f };
		std::string netherText = std::to_string((int)floorf(netherPos.x)) + std::string(" ") + std::to_string((int)floorf(netherPos.y)) + std::string(" ") + std::to_string((int)floorf(netherPos.z));
		float netherTextWidth = ImGuiUtils::getTextWidth(netherText, textScale);
		float labelTextWidth = ImGuiUtils::getTextWidth("Position: ", textScale);
		float netherLabelTextWidth = ImGuiUtils::getTextWidth("Nether: ", textScale);
		float totalWidthOriginal = labelTextWidth + xyzTextWidth;
		float totalWidthNether = netherLabelTextWidth + netherTextWidth;
		float textHeight = ImGuiUtils::getTextHeight(textScale);
		float extraPadding = 2.4f * size;
		Vec4<float> rect(
			xOffset + 15 - extraPadding,
			yOffset - 15 - extraPadding,
			xOffset + 15 + totalWidthOriginal + extraPadding,
			yOffset - 15 + textHeight + extraPadding
		);
		ImGuiUtils::fillRectangle(rect, UIColor(0, 0, 0, 127.5));
		ImGuiUtils::drawText(Vec2<float>(xOffset + 15, yOffset - 15), "Position: ", whiteColor, textScale);
		ImGuiUtils::drawText(Vec2<float>(xOffset + 15 + labelTextWidth, yOffset - 15), xyzText, semiGrayColor, textScale);
		yOffset -= textHeight + extraPadding; 
		Vec4<float> netherRect(
			xOffset + 15 - extraPadding,
			yOffset - 15 - extraPadding,
			xOffset + 15 + totalWidthNether + extraPadding,
			yOffset - 15 + textHeight + extraPadding
		);
		ImGuiUtils::fillRectangle(netherRect, UIColor(0, 0, 0, 127.5));
		ImGuiUtils::drawText(Vec2<float>(xOffset + 15, yOffset - 15), "Nether: ", whiteColor, textScale);
		ImGuiUtils::drawText(Vec2<float>(xOffset + 15 + netherLabelTextWidth, yOffset - 15), netherText, semiGrayColor, textScale);
		yOffset -= textHeight + extraPadding;
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
