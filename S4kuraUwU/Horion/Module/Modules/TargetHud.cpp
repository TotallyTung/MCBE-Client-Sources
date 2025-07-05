#include "TargetHUD.h"
#include "../../DrawUtils.h"
#include "../../../SDK/CAttribute.h"
TargetHUD::TargetHUD() : IModule(0, Category::COMBAT, "TargetHUD") {
}

TargetHUD::~TargetHUD() {
}

const char* TargetHUD::getModuleName() {
	return ("TargetHUD");
}

static std::vector<C_Entity*> targetList3;

void findPlayers2(C_Entity* currentEntity, bool isRegularEntity) {
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() != 319) return;  // Players
	if (!Target::isValidTarget(currentEntity)) return;
	if (currentEntity->getEntityTypeId() == 1677999) return;  // Villager
	if (currentEntity->getEntityTypeId() == 51) return; // NPC

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < 10) {
		targetList3.push_back(currentEntity);
	}
}


struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void TargetHUD::onTick(C_GameMode* gm) {
	// Loop through all our players and retrieve their information
	targetList3.clear();

	g_Data.forEachEntity(findPlayers2);

	for (auto& i : targetList3) {
		if (!(i->damageTime > 1 && hurttime)) {
			targethud++;
		} else
			targethud = 0;
	}
}

void TargetHUD::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	// static auto partner = moduleMgr->getModule<Partner>();
	std::sort(targetList3.begin(), targetList3.end(), CompareTargetEnArray());
	if (g_Data.isInGame() && !targetList3.empty() && g_Data.canUseMoveKeys && g_Data.getLocalPlayer()->canOpenContainerScreen() && targethud >= 1 && targetList3[0]->isPlayer()) {
		// Atributes
		AbsorptionAttribute attribute = AbsorptionAttribute();
		HealthAttribute attribute2 = HealthAttribute();
		float Absorbtion = targetList3[0]->getHealth();
		float Health = targetList3[0]->getHealth();
		float HealthMax = targetList3[0]->getHealth();

		vec2_t res = g_Data.getClientInstance()->getGuiData()->windowSize;
		C_LocalPlayer* Player = g_Data.getClientInstance()->getLocalPlayer();
		if (Player != nullptr) {
			vec4_t rectPos((res.x / 2.f) + (res.x / 20.f), (res.y / 2.f) + (res.y / 24.f), (res.x / 2.f) + (res.x / 6.f), (res.y / 2.f) + (res.y / 8.f));  // vec4 rectPos((res.x / 2.f) + (res.x / 20.f), (res.y / 2.f) - (res.y / 24.f), (res.x / 2.f) + (res.x / 6.f), (res.y / 2.f) + (res.y / 24.f));
			float rectWidth = rectPos.z - rectPos.x;

			// counter for fade
			counter++;
			if (counter == 100)
				counter--;

			// all the displays

			// Gets the targets name, then makes it not go to next line
			std::string targetName;
			// auto Hud = moduleMgr->getModule<HudModule>();
			/* if (Hud->displaySecondHalf) {
				targetName = Utils::sanitize(targetList3[0]->getNameTag()->getText());
				Utils::replaceString(targetName, '\n', ' ');
			}*/
			targetName = Utils::sanitize(targetList3[0]->getNameTag()->getText());
			targetName = targetName.substr(0, targetName.find('\n'));

			std::string HealthString = std::to_string(((int)Health));
			std::string HealthMaxString = std::to_string(((int)HealthMax));
			std::string distance = "Distance: " + std::to_string((int)(*targetList3[0]->getPos()).dist(*g_Data.getLocalPlayer()->getPos()));
			//std::string healthDisplay = "Health: " + HealthString + "/" + HealthMaxString;
			//std::string armor = "Armor: ";
			std::string absorptionDisplay;
			float testF = 100.f;

			// The text
			{
				DrawUtils::drawText(vec2_t(rectPos.x + (res.x / 900.f), rectPos.y - 17 + (res.y / 35.f)), &targetName, MC_Color(255, 255, 255), 1.f, counter / 100.f);             // name
				DrawUtils::drawText(vec2_t(rectPos.x + (res.x / 900.f), rectPos.y - 9 + (res.y / 35.f)), &distance, MC_Color(255, 255, 255), 0.7f, counter / 100.f);               // distance
				DrawUtils::drawText(vec2_t(rectPos.x + 35 + (res.x / 900.f), rectPos.y - 4 + (res.y / 35.f)), &absorptionDisplay, MC_Color(255, 255, 85), 0.7f, counter / 100.f);  // absorbtion
				//DrawUtils::drawText(vec2_t(rectPos.x + (res.x / 900.f), rectPos.y - 4 + (res.y / 35.f)), &healthDisplay, MC_Color(255, 255, 255), 0.7f, counter / 100.f);          // health
				//DrawUtils::drawText(vec2_t(rectPos.x + (res.x / 900.f), rectPos.y + 1 + (res.y / 35.f)), &armor, MC_Color(255, 255, 255), 0.8f, counter / 100.f);                  // armor
				vec2_t armorPos = vec2_t(rectPos.x + (res.x / 250.f), rectPos.y + 0 + (res.y / 45.f));
				vec2_t armorPos2 = vec2_t(rectPos.x + (res.x / 250.f), rectPos.y + 0 + (res.y / 20.f));
				vec2_t ItemPos = vec2_t(rectPos.x + (res.x / 10.f), rectPos.y + 0 + (res.y / 45.f));
				vec2_t ItemPos2 = vec2_t(rectPos.x + (res.x / 10.f), rectPos.y + 0 + (res.y / 20.f));
				static float constexpr opacity = 10;
				float scale = 2 * 0.26f;
				float spacing = scale + 15.f + 2;

				static float constexpr scale2 = 1.f;
				static float constexpr opacity2 = 0.25f;
				static float constexpr spacing2 = scale2 + 17.f;
				vec2_t windowSize = g_Data.getClientInstance()->getGuiData()->windowSize;
				float x = windowSize.x / 2.f + 7.0f;
				float y = windowSize.y - 72.5f;
				float y1 = windowSize.y - 65.5f;
				auto* player = reinterpret_cast<C_Player*>(targetList3[0]);

				for (int t = 0; t < 4; t++) {
					C_ItemStack* stack = player->getArmor(t);
					if (stack->isValid()) {
						DrawUtils::drawItem(stack, vec2_t(armorPos), opacity, scale + 0.2, stack->isEnchanted());  // vec2_t(x,y)這裡改(+ or -) by:hank this is armor
						armorPos.x += scale * spacing + 5;
						C_Item* item = stack->getItem();
						if (item->getMaxDamage() > 0) {
							int armorDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
							std::string armorDamageText = std::to_string(armorDamage) + "%";
							int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
							DrawUtils::drawText(vec2_t(armorPos2), &armorDamageText, MC_Color(255 - i, i, 100), 0.5f, 1.f);  // vec2_t(x+ scale2,y1)這裡改(+ or -) by:hank this is armor耐久
						}
					}
					armorPos2.x += scale * spacing + 5;
				}
				C_ItemStack* stack = player->getSelectedItem();
				C_PlayerInventoryProxy* supplies = player->getSupplies();
				C_ItemStack* item = supplies->inventory->getItemStack(supplies->selectedHotbarSlot);
				if (item->isValid()) {
					DrawUtils::drawItem(stack, vec2_t(ItemPos), opacity, scale + 0.2, stack->isEnchanted()); //vec2_t(x,y)這裡改(+ or -) by:hank this is item
					C_Item* item = stack->getItem();
					if (item->getMaxDamage() > 0) {
						int itemDamage = ceil((float)(item->getMaxDamage() - item->getDamageValue(stack->tag)) / (float)item->getMaxDamage() * 100);
						std::string itemDamageText = std::to_string(itemDamage) + "%";
						int i = (int)round(255.f - (float)item->getDamageValue(stack->tag) * 255.f / (float)item->getMaxDamage());
						DrawUtils::drawText(vec2_t(ItemPos2), &itemDamageText, MC_Color(255 - i, i, 100), 0.5f, 1.f);  // vec2_t(x+ scale2,y1)這裡改(+ or -) by:hank this is item耐久
					}
				}
				DrawUtils::flush();

				// Health Bar
				{
					float healthBarWidth = (Health / HealthMax) * rectWidth;
					if (!(targetList3[0]->damageTime > 1)) {
						DrawUtils::fillRectangle(vec4_t(rectPos.x + .2f, rectPos.y + (res.y / 18.f) + ((rectPos.w - (rectPos.y + (res.y / 18.f))) / 2), rectPos.x + healthBarWidth, rectPos.w), MC_Color(0, 200, 0), counter / 200.f);
						// DrawUtils::drawRectangle(vec4_t(rectPos.x + .2f, rectPos.y + (res.y / 18.f) + ((rectPos.w - (rectPos.y + (res.y / 18.f))) / 2), rectPos.x + 1 * rectWidth, rectPos.w), MC_Color(0, 255, 0), counter / 100.f);
					} else {
						DrawUtils::fillRectangle(vec4_t(rectPos.x + .2f, rectPos.y + (res.y / 18.f) + ((rectPos.w - (rectPos.y + (res.y / 18.f))) / 2), rectPos.x + healthBarWidth, rectPos.w), MC_Color(255, 0, 0), counter / 200.f);
						// DrawUtils::drawRectangle(vec4_t(rectPos.x + .2f, rectPos.y + (res.y / 18.f) + ((rectPos.w - (rectPos.y + (res.y / 18.f))) / 2), rectPos.x + 1 * rectWidth, rectPos.w), MC_Color(255, 0, 0), counter / 100.f);
					}
				}
				rectPos.y += res.y / 12.f;
				rectPos.w += res.y / 12.f;
			}

			DrawUtils::flush();
		}
	}
}