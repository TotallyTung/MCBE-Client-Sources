#include "TargetHUD.h"

using namespace std;
TargetHUD::TargetHUD() : IModule(0, Category::VISUAL, "Displays information about your target")
{
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry("Simple", 0);
	mode.addEntry("Pretty", 7);
	registerBoolSetting("Animation", &animation, animation);
	registerBoolSetting("Items", &showItems, showItems);
	registerIntSetting("Opacity", &opacity, opacity, 0, 255);
}

const char *TargetHUD::getRawModuleName() { return ("TargetHUD"); }

#pragma region TargetList
static bool entityChanged = false;
static vector<C_Entity *> targetList;
void findPlayers_TargetHUD(C_Entity *currentEntity, bool isRegularEntity)
{
	if (currentEntity == nullptr)
		return;
	//	if (currentEntity == g_Data.getLocalPlayer()) return;
	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;
	if (!g_Data.getLocalPlayer()->isAlive())
		return;
	if (!currentEntity->isAlive())
		return;
	if (currentEntity->getEntityTypeId() != 319)
		return;
	if (currentEntity->getNameTag()->getTextLength() <= 1)
		return;
	if (!TargetUtil::isValidTarget(currentEntity))
		return;
	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < 8)
		targetList.push_back(currentEntity);
}
#pragma endregion

#pragma region TargetArray
struct CompareTargetEnArray
{
	bool operator()(C_Entity *lhs, C_Entity *rhs)
	{
		C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
#pragma endregion

int entScore(C_Entity *ent)
{
	int score = 0;
	// score += ent->getAbsorption();
	if (ent->getEntityTypeId() == 319)
	{
		C_Player *player = reinterpret_cast<C_Player *>(ent);
		for (int t = 0; t < 4; t++)
		{
			C_ItemStack *stack = player->getArmor(t);
			if (stack->isValid())
			{
				score += stack->getArmorValueWithEnchants();
			}
		}
		C_PlayerInventoryProxy *supplies = player->getSupplies();
		C_ItemStack *item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
		if (item->isValid())
			score += item->getAttackingDamageWithEnchants();
	}

	return score;
}

void TargetHUD::onTick(C_GameMode *gm)
{
	targetList.clear();
	g_Data.forEachEntity(findPlayers_TargetHUD);
	targetListEmpty = targetList.empty();
	sort(targetList.begin(), targetList.end(), CompareTargetEnArray());

	if (!targetList.empty())
	{
		entityChanged = true;
	}
	else
	{
		entityChanged = false;
		displayhealth = 0;
		displayhealth2 = 0;
		size = 30.f;
		animationsize = 0.f;
	}
}

void TargetHUD::onPostRender(C_MinecraftUIRenderContext *renderCtx)
{
	static auto clickGUI = moduleMgr->getModule<ClickGUIMod>();
	vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
	static auto interface = moduleMgr->getModule<Interface>();

	if (g_Data.canUseMoveKeys() && !clickGUI->hasOpenedGUI)
	{

		if (entityChanged && animation)
		{
			positionX += ((windowSize.x / 2 + 10) - positionX) * 0.045f;
			positionY += ((windowSize.y / 2 + 10) - positionY) * 0.045f;
		}
		else
		{
			positionX = windowSize.x / 2 + 10;
			positionY = windowSize.y / 2 + 10;
		}

		if (!targetListEmpty)
		{
			string percent = "100%";
			auto interfaceColor = ColorUtil::interfaceColor(1);
			vec3_t *pos = targetList[0]->getPos();
			auto health = targetList[0]->getHealth();
			int inthealth = health;
			int percentage = (health / 20) * 100;
			string displaypercent = to_string(percentage);
			if (displayhealth > health)
				displayhealth -= ((displayhealth - health) / 15);
			else if (displayhealth < health)
				displayhealth += ((health - displayhealth) / 15);
			if (displayhealth > health)
				displayhealth -= ((displayhealth - health) / 15);
			else if (displayhealth < health)
				displayhealth += ((health - displayhealth) / 15);
			string smoothtext = to_string(inthealth) + ".0 health left";
			float steveOpacity = (targetList[0]->damageTime * 80) / 8;
			float dist = (*targetList[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
			float defaultRectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			float novolineRectHeight = (3.25f, 3.25f) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			string position = "Position: " + to_string((int)floorf(pos->x)) + " " + to_string((int)floorf(pos->y)) + " " + to_string((int)floorf(pos->z));
			string distance = string(GRAY) + "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
			string distancev2 = "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10)) + "m";
			string targetName = targetList[0]->getNameTag()->getText();
			targetName = Utils::sanitize(targetName);
			targetName = targetName.substr(0, targetName.find('\n'));

			if (!targetListEmpty)
			{
				if (mode.getSelectedValue() == 0)
				{
					string distance = string(GRAY) + "         Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
					string name = string(GRAY) + "         Name: " + string(RESET);
					string rawName = targetList[0]->getNameTag()->getText();
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length())
						targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else
						targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX + 16.3f, positionY + defaultRectHeight);
					if (showItems)
						testRect = vec4_t(positionX, positionY, targetLen + positionX + 16.3f, positionY + defaultRectHeight + 10.5);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), false);
					vec4_t healthRect = vec4_t(testRect.x + 39, testRect.y + 32, testRect.x + ((targetLen + 8.5) / 20) * health, testRect.y + 34);
					if (showItems)
						healthRect = vec4_t(testRect.x + 4, testRect.y + 44, testRect.x + ((targetLen + 8.5) / 20) * health, testRect.y + 47);
					if (targetList[0]->damageTime > 1)
					{
						DrawUtils::drawRectangle(healthRect, MC_Color(255, 0, 0), 0.3f);
						DrawUtils::fillRectangleA(healthRect, MC_Color(255, 0, 0, steveOpacity));
					}
					else
					{
						DrawUtils::drawRectangle(healthRect, MC_Color(0, 255, 0), 0.3);
						DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
					}
					vec4_t absorbtionRect = vec4_t(testRect.x + 39, testRect.y + 26, testRect.x + ((targetLen + 8.5) / 20), testRect.y + 29);
					if (showItems)
						absorbtionRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + ((targetLen + 8.5) / 20), testRect.y + 41);
					vec4_t absorbtionSubRect = vec4_t(testRect.x + 39, testRect.y + 28, testRect.x + ((targetLen + 8.5) / 20), testRect.y + 30);
					if (showItems)
						absorbtionSubRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + ((targetLen + 8.5) / 20) * health, testRect.y + 41);
					//DrawUtils::fillRectangleA(absorbtionSubRect, MC_Color(25, 25, 25, 255));
					DrawUtils::drawImage("textures/entity/steve.png", vec2_t(positionX + 5, positionY + 5), vec2_t(30, 30), vec2_t(0.125f, 0.125f), vec2_t(0.125f, 0.125f));
					if (targetList[0]->damageTime > 1)
					{
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 5, 35 + positionX, positionY + 35), MC_Color(255, 0, 0, steveOpacity));
					}
				}
				if (mode.getSelectedValue() == 7)
				{
					float defaultRectHeight = (5.5, 5.5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
					float opacity2 = (targetList[0]->damageTime * 80) / 8;
					string name = "          ";
					string rawName = targetList[0]->getNameTag()->getText();
					string fakename = "Gamesensingggggg";
					auto damageTime = targetList[0]->damageTime;
					int smoothing = 5;
					int maxsize = 30;
					int minsize = 27;
					if (damageTime > 1)
					{
						if (damageTime > 5)
						{
							if (size > minsize)
								size -= ((size - minsize) / smoothing);
							else if (size < minsize)
								size += ((minsize - size) / smoothing);
						}
						else
						{
							if (size > maxsize)
								size -= ((size - maxsize) / smoothing);
							else if (size < maxsize)
								size += ((maxsize - size) / smoothing);
						}
					}
					float size2 = size / 30;
					rawName = fakename;
					rawName = Utils::sanitize(rawName);
					rawName = name + rawName;
					rawName = rawName.substr(0, rawName.find('\n'));
					if (rawName.length() < distance.length())
						targetLen = DrawUtils::getTextWidth(&distance, 1) + 10.5;
					else
						targetLen = DrawUtils::getTextWidth(&rawName, 1) + 6.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX + 7.f, positionY + defaultRectHeight);
					DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), true);
					DrawUtils::drawRoundRectangle(testRect, interfaceColor, 150);
					DrawUtils::drawSteve(vec4_t(positionX + 5.f, positionY + 2.f, positionX, positionY));
					vec4_t healthRect = vec4_t(testRect.x + 40, testRect.y + 23, testRect.x + ((targetLen - 4) / 20) * displayhealth, testRect.y + 30);
					vec4_t BlackRect = vec4_t(testRect.x + 40, testRect.y + 23, testRect.x + ((targetLen - 4) / 20) * 20, testRect.y + 30);
					DrawUtils::fillRoundRectangle(BlackRect, MC_Color(0, 0, 0, 255), true);
					DrawUtils::fillRoundRectangle(healthRect, interfaceColor, 255);
					vec2_t healthTextPos = vec2_t(healthRect.x, healthRect.y - 10.f);
					DrawUtils::drawText(healthTextPos, &smoothtext, MC_Color(255, 255, 255), 0.7, 0.7, true);
					if (targetList[0]->damageTime > 1)
					{
						DrawUtils::fillRectangleA(vec4_t(positionX + 5, positionY + 2, 35 + positionX, positionY + 32), MC_Color(255, 0, 0, steveOpacity));
					}
				}
			}

			for (auto &i : targetList)
			{
				if (mode.getSelectedValue() == 0)
				{
					string namestr = string(GRAY) + "Name: " + string(RESET) + targetName;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight);
					vec2_t namePos = vec2_t(testRect.x + 39, testRect.y + 5);
					vec2_t distPos = vec2_t(testRect.x + 39, testRect.y + 15);
					vec2_t armorPos = vec2_t(testRect.x + 39, testRect.y + 25);
					DrawUtils::drawText(distPos, &distance, MC_Color(255, 255, 255), 1, 1, true);
					DrawUtils::drawText(namePos, &namestr, MC_Color(255, 255, 255), 1, 1, true);
					if (i->getEntityTypeId() == 319 && showItems)
					{
						static float constexpr opacity = 10;
						float scale = 3 * 0.26f;
						float spacing = scale + 15.f + 2;
						auto *player = reinterpret_cast<C_Player *>(targetList[0]);
						for (int t = 0; t < 5; t++)
						{
							C_ItemStack *stack = player->getArmor(t);
							if (stack->isValid())
							{
								float dura1 = stack->getItem()->getMaxDamage();
								float dura2 = stack->getItem()->getDamageValue(stack->tag);
								float dura3 = dura1 - dura2;
								int dura4 = dura3 / dura1 * 100;
								std::string durastr = std::to_string((int)dura4) + std::string("");
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

								DrawUtils::drawCenteredString(vec2_t(armorPos.x + 8.1f, armorPos.y + 18.f), &durastr, 0.75f, MC_Color(gamerColor), true);
								DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
								armorPos.x += scale * spacing;
							}
						}
						C_PlayerInventoryProxy* supplies = player->getSupplies();
						C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
						C_ItemStack* stack = player->getSelectedItem();
						C_ItemStack* totem = player->getOffhandItem();
						if (item->isValid()) {
							float dura1 = stack->getItem()->getMaxDamage();
							float dura2 = stack->getItem()->getDamageValue(stack->tag);
							float dura3 = dura1 - dura2;
							int dura4 = dura3 / dura1 * 100;
							std::string durastr = std::to_string((int)dura4);
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
							// if (!dura1 == 0) DrawUtils::drawText(vec2_t(rectPos.z - 1.f - 15.f * scale, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 0.55f, 1.f, true);
							if (!dura1 == 0)
								DrawUtils::drawCenteredString(vec2_t(armorPos.x + 8.1f, armorPos.y + 18), &durastr, 0.75, MC_Color(gamerColor), true);
							DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
							if (!totem->item == NULL)
								DrawUtils::drawItem(totem, vec2_t(armorPos.x + 10.f, armorPos.y + 0), opacity, scale, item->isEnchanted());
						}
					}
				}

				if (mode.getSelectedValue() == 7)
				{
					sort(targetList.begin(), targetList.end(), CompareTargetEnArray());
					float targetLen = DrawUtils::getTextWidth(&targetName, 1) + 35.5;
					vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11.f);
					vec2_t armorPos = vec2_t(testRect.x + 15, testRect.y + 40);
					DrawUtils::drawText(vec2_t(testRect.x + 40, testRect.y + 5), &targetName, MC_Color(255, 255, 255), 1, 1, true);

					if (i->getEntityTypeId() == 319 && showItems)
					{
						static float constexpr opacity = 10;
						float scale = 4 * 0.26f;
						float spacing = scale + 15.f + 2;
						auto *player = reinterpret_cast<C_Player *>(targetList[0]);
						for (int t = 0; t < 5; t++)
						{
							C_ItemStack *stack = player->getArmor(t);
							if (stack->isValid())
							{
								float dura1 = stack->getItem()->getMaxDamage();
								float dura2 = stack->getItem()->getDamageValue(stack->tag);
								float dura3 = dura1 - dura2;
								int dura4 = dura3 / dura1 * 100;
								std::string durastr = std::to_string((int)dura4) + std::string("");
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

								DrawUtils::drawCenteredString(vec2_t(armorPos.x + 8.f, armorPos.y - 1.5f), &durastr, 0.80f, MC_Color(gamerColor), true);
								DrawUtils::drawItem(stack, vec2_t(armorPos), 1, scale, false);
								armorPos.x += scale * spacing;
							}
						}
						C_PlayerInventoryProxy *supplies = player->getSupplies();
						C_ItemStack *item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
						C_ItemStack* itemToCount = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
						C_ItemStack* stack = player->getSelectedItem();
						C_ItemStack* totem = player->getEquippedTotem();
						int totalCount = 0;
						totalCount += stack->count;
						if (item->isValid()) {
							std::string itemCount = to_string(totalCount);
							float dura1 = stack->getItem()->getMaxDamage();
							float dura2 = stack->getItem()->getDamageValue(stack->tag);
							float dura3 = dura1 - dura2;
							int dura4 = dura3 / dura1 * 100;
							std::string durastr = std::to_string((int)dura4);
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
							// if (!dura1 == 0) DrawUtils::drawText(vec2_t(rectPos.z - 1.f - 15.f * scale, y - itemHeight), &durastr, MC_Color(gamerColor), textSize * 0.55f, 1.f, true);
							if (!dura1 == 0)
								DrawUtils::drawCenteredString(vec2_t(armorPos.x + 8.1f, armorPos.y - 2), &durastr, 0.80, MC_Color(gamerColor), true);
							//if (dura1 == 0)
								//DrawUtils::drawText(vec2_t(armorPos.x, armorPos.y + 20.f), &itemCount, MC_Color(255, 255, 255), 1.f, 1.f, true); servers dont send stackcount data :(
							DrawUtils::drawItem(item, vec2_t(armorPos), opacity, scale, item->isEnchanted());
							if (!totem->item == NULL)
							DrawUtils::drawItem(totem, vec2_t(armorPos.x + 16.f, armorPos.y + 0), opacity, scale, item->isEnchanted());
						}
					}
				}
			}
		}

		if (!clickGUI->isEnabled() && visualize && targetList.size() == 0)
		{ // if theres no targets use local player
			auto absorbtion = 0.f /*g_Data.getLocalPlayer()->getAttribute(&attribute)->currentValue*/;
			float dist = 0;
			float defaultRectHeight = (5, 5) * DrawUtils::getFont(Fonts::SMOOTH)->getLineHeight();
			string distance = string(GRAY) + "Distance: " + string(RESET) + to_string((int)dist) + string(".") + to_string((int)(dist * 10) - ((int)dist * 10));
			string name = string(GRAY) + "Name: " + string(RESET);
			string rawName = g_Data.getLocalPlayer()->getNameTag()->getText();
			rawName = Utils::sanitize(rawName);
			string nlelel = string(GRAY) + "Name:          " + string(RESET) + rawName;
			rawName = name + rawName;
			rawName = rawName.substr(0, rawName.find('\n'));
			targetLen = DrawUtils::getTextWidth(&nlelel, 1) + 6.5;
			vec4_t testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight - 11);
			if (showItems)
				testRect = vec4_t(positionX, positionY, targetLen + positionX, positionY + defaultRectHeight);
			DrawUtils::fillRoundRectangle(testRect, MC_Color(0, 0, 0, opacity), false);
			vec4_t healthRect = vec4_t(testRect.x + 39, testRect.y + 32, testRect.x + ((targetLen - 4) / 20) * 20, testRect.y + 34);
			if (showItems)
				healthRect = vec4_t(testRect.x + 4, testRect.y + 44, testRect.x + ((targetLen - 4) / 20) * 20, testRect.y + 47);
			vec2_t namePos = vec2_t(testRect.x + 39, testRect.y + 5);
			vec2_t distPos = vec2_t(testRect.x + 39, testRect.y + 15);
			DrawUtils::drawText(distPos, &distance, MC_Color(255, 255, 255), 1, 1, true);
			DrawUtils::drawText(namePos, &rawName, MC_Color(255, 255, 255), 1, 1, true);
			DrawUtils::drawRectangle(healthRect, MC_Color(0, 255, 0), 0.3);
			DrawUtils::fillRectangleA(healthRect, MC_Color(0, 255, 0, 90));
			vec4_t absorbtionRect = vec4_t(testRect.x + 39, testRect.y + 26, testRect.x + absorbtion * 4.f, testRect.y + 29);
			if (showItems)
				absorbtionRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + absorbtion * 4.f, testRect.y + 41);
			vec4_t absorbtionSubRect = vec4_t(testRect.x + 39, testRect.y + 28, testRect.x + targetLen - 4.f, testRect.y + 30);
			if (showItems)
				absorbtionSubRect = vec4_t(testRect.x + 4, testRect.y + 38, testRect.x + targetLen - 4.f, testRect.y + 41);
			DrawUtils::fillRectangleA(absorbtionSubRect, MC_Color(25, 25, 25, 255));
			if (absorbtion > 1.f)
			{
				DrawUtils::drawRectangle(absorbtionRect, MC_Color(255, 230, 0), 0.3);
				DrawUtils::fillRectangleA(absorbtionRect, MC_Color(255, 230, 0, 255));
			}
			DrawUtils::drawSteve(vec4_t(positionX + 5, positionY + 5, positionX, positionY + 20));
		}
	}
}