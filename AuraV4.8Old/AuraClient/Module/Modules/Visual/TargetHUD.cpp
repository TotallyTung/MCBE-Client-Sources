#include "TargetHUD.h"
#include "../pch.h"

using namespace std;
TargetHUD::TargetHUD() : IModule(0, Category::VISUAL, "Displays information about your target") {
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
	registerFloatSetting("Range", &rangeTGH, rangeTGH, 3.f, 50.f);
	registerBoolSetting("Outline", &outlinetgh, outlinetgh);
}

const char* TargetHUD::getModuleName() { return ("TargetHUD"); }

#pragma region TargetList
static bool entityChanged = false;
static vector<C_Entity*> targetList;
void findPlayers_TargetHUD(C_Entity* currentEntity, bool isRegularEntity) {
	auto tgh = moduleMgr->getModule<TargetHUD>();
	if (currentEntity == nullptr) return;
	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity->getNameTag()->getTextLength() <= 1) return;
	if (!TargetUtil::isValidTarget(currentEntity)) return;
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < 100) targetList.push_back(currentEntity);
}
#pragma endregion

#pragma region TargetArray
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
#pragma endregion

void TargetHUD::onTick(C_GameMode* gm) {
	targetList.clear();
	g_Data.forEachEntity(findPlayers_TargetHUD);
	targetListEmpty = targetList.empty();
	sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty()) entityChanged = true;
	else entityChanged = false;
}

float dbrr1 = 0;
float dbrr2 = 0;

void TargetHUD::onPostRender(C_MinecraftUIRenderContext* renderCtx) {
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;


	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI) {
		if (entityChanged) {
			if ((targetList[0]->getPos()->dist(*g_Data.getLocalPlayer()->getPos())) < this->rangeTGH) {
				positionX += ((windowSize.x / 2 + 20) - positionX) * 0.15f; positionY += ((windowSize.y / 2 + 15) - positionY) * 0.15f;
			}
			else {
				positionX += (windowSize.x + 1 - positionX) * 0.15f; positionY += ((windowSize.y / 2 + 15) - positionY) * 0.15f;
			}
		}
		//else { positionX = windowSize.x; positionY = windowSize.y / 2 + 15; }
		else
		{
			positionX += (windowSize.x + 1 - positionX) * 0.15f; positionY += ((windowSize.y / 2 + 15) - positionY) * 0.15f;
		}

		if (!targetListEmpty) {
			vec3_t* pos = targetList[0]->getPos();
			auto health = targetList[0]->getHealth();
			float dist = (*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
			float defaultRectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			defaultRectHeight = (5, 2) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			string position = "Position: " + to_string((int)floorf(pos->x)) + " " + to_string((int)floorf(pos->y)) + " " + to_string((int)floorf(pos->z));
			string distance = "Distance: " + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10)) + "m";

			string targetName = targetList[0]->getNameTag()->getText();
			targetName = Utils::sanitize(targetName);
			targetName = targetName.substr(0, targetName.find('\n'));

			if (!targetListEmpty) {
				defaultRectHeight = (7, 2) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
				string name = targetList[0]->getNameTag()->getText();
				name = Utils::sanitize(name);
				string namestr = "Name: " + name;
				namestr = namestr.substr(0, namestr.find('\n'));
				if (namestr.length() < position.length()) targetLen = DrawUtils::getTextWidth(&position, 1) + 6.5;
				else targetLen = DrawUtils::getTextWidth(&namestr, 1) + 6.5;
				vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 10);
				//testRect = vec4_t(positionX, positionY, /*targetLen + positionX + windowSize.x */windowSize.x , positionY + defaultRectHeight * 2.5f + 25);
				testRect = vec4_t(positionX, positionY, targetLen + positionX + 90, positionY + defaultRectHeight * 2.5f + 25);
				if (!(targetList[0]->damageTime > 1)) DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
				else DrawUtils::fillRoundRectangle(testRect, MC_Color(255, 0, 0, opacity), true);
				auto cccc = ColorUtil::interfaceColor(1);
				if (outlinetgh) DrawUtils::drawRoundRectangle(testRect, MC_Color(cccc), false);
				dbrr1 = targetLen;
				dbrr2 = defaultRectHeight;
			}

			for (auto& i : targetList) {
				// Old

				string name = targetList[0]->getNameTag()->getText();
				name = Utils::sanitize(name);
				string namestr = "Name: " + name;
				namestr = namestr.substr(0, namestr.find('\n'));
				static const float rectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
				if (namestr.length() < position.length()) targetLen = DrawUtils::getTextWidth(&position, 1) + 6.5;
				else targetLen = DrawUtils::getTextWidth(&namestr, 1) + 6.5;
				vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + rectHeight);
				vec2_t namePos = vec2_t(testRect.x + 4, testRect.y + 5);
				vec2_t distPos = vec2_t(testRect.x + 4, testRect.y + 15);
				vec2_t distPos2 = vec2_t(testRect.x + 4, testRect.y + 25);
				vec2_t armorPos = vec2_t(testRect.x + 4, testRect.y + 25);
				vec2_t armorPos2 = vec2_t(testRect.z + 70, positionY);
				DrawUtils::drawText(distPos2, &distance, MC_Color(255, 255, 255), 1, 1, true);
				DrawUtils::drawText(distPos, &position, MC_Color(255, 255, 255), 1, 1, true);
				DrawUtils::drawText(namePos, &namestr, MC_Color(255, 255, 255), 1, 1, true);
				if (i->getEntityTypeId() == 319) {

					static float constexpr opacity = 10;
					float scale = 4 * 0.26f;
					float spacing = scale + 15.f + 2;
					float spacing2 = (testRect.w - testRect.y) / 5 + scale + 2.5;
					auto* player = reinterpret_cast<C_Player*>(targetList[0]);
					for (int t = 0; t < 4; t++) {
						C_ItemStack* stack = player->getArmor(t);
						if (stack->isValid()) {
							float dura1 = stack->getItem()->getMaxDamage();
							float dura2 = stack->getItem()->getDamageValue(stack->tag);
							float dura3 = dura1 - dura2;
							int dura4 = dura3 / dura1 * 100;
							std::string durastr = std::to_string((int)dura4) + "%";
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
							float itemhai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * (scale - 0.5);
							DrawUtils::drawItem(stack, vec2_t(armorPos2), 1, scale, false);
							DrawUtils::drawText(vec2_t(armorPos2.x - 25, armorPos2.y + itemhai), &durastr, MC_Color(gamerColor), scale, 1.f, true);
							armorPos2.y += scale * spacing2;
						}
					}
					C_PlayerInventoryProxy* supplies = player->getSupplies();
					C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
					if (item->isValid()) {
						float dura1 = item->getItem()->getMaxDamage();
						float dura2 = item->getItem()->getDamageValue(item->tag);
						float dura3 = dura1 - dura2;
						int dura4 = dura3 / dura1 * 100;
						std::string durastr = std::to_string((int)dura4) + "%";
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
						DrawUtils::drawItem(item, vec2_t(armorPos2), 1, scale, false);
						float itemhai = DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight() * (scale - 0.5);
						if (!dura1 == 0) DrawUtils::drawText(vec2_t(armorPos2.x - 25, armorPos2.y + itemhai), &durastr, MC_Color(gamerColor), scale, 1.f, true);

					}
				}

			}
		}
		else
		{
			DrawUtils::fillRoundRectangle(vec4_t(positionX, positionY, dbrr1 + positionX + 90, positionY + dbrr2 * 2.5f + 25), MC_Color(0, 0, 0, opacity), true);
			DrawUtils::drawCenteredString(vec2_t((positionX * 2 + dbrr1 + 90) / 2, (positionY * 2 + dbrr2 * 2.5f + 25) / 2), &std::string("No targets found"), 1.f, MC_Color(255, 255, 255, 255), true);
		}
	}
}