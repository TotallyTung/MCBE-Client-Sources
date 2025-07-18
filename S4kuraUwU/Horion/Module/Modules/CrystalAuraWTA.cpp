#include "CrystalAuraWTA.h"

CrystalAuraWTA::CrystalAuraWTA() : IModule(0x0, Category::COMBAT, "CrystalAura Developed by WellThatsAwkward") {
	registerIntSetting("range", &range, range, 1, 10);
	registerBoolSetting("autoplace", &autoplace, autoplace);
	//registerBoolSetting("onlyCrystal", &crystalCheck, crystalCheck);
	//registerBoolSetting("LockY", &yLock, yLock);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("onClick", &isClick, isClick);
	registerBoolSetting("Multi", &doMultiple, doMultiple);
	registerBoolSetting("Silent Ca", &spoof, spoof);
}
int crystalDelay = 0;
int crystalDelay2 = 0;
int crystalDelay3 = 0;

CrystalAuraWTA::~CrystalAuraWTA() {
}

const char* CrystalAuraWTA::getModuleName() {
	return ("CrystalAuraWTA+");
}
struct CompareTargetDist {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
static std::vector<C_Entity*> targetList7;
void findEntity3(C_Entity* currentEntity, bool isRegularEntity) {
	static auto CrystalAuraWTAMod = moduleMgr->getModule<CrystalAuraWTA>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 71)  // crystal
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	//if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
	//return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
		return;
	if (currentEntity->width <= 0.f || currentEntity->height <= 0.f)  // Don't hit this pesky antibot on 2b2e.org
		return;
	if (currentEntity->getEntityTypeId() == 64)  // item
		return;
	if (currentEntity->getEntityTypeId() == 69)  // xp_orb
		return;
	if (currentEntity->getEntityTypeId() == 80)  // arrow
		return;

	if (!Target::isValidTarget(currentEntity))
		return;

	//how hard is it to play fair? add back the badman check if its hard

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < CrystalAuraWTAMod->range) {
		targetList7.push_back(currentEntity);
	}
}

bool checkTargCollision(vec3_t* block, C_Entity* ent) {
	std::vector<vec3_t*> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
	int n = 0;
	if (!corners.empty())
		for (auto corner : corners) {
			//	DrawUtils::drawText(DrawUtils::worldToScreen(*corners[n]), &std::to_string(n + 1), MC_Color(1.f, 1.f, 1.f));
			//DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);
			//if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->blockId != 0) {
			//DrawUtils::drawBox(vec3_t(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), g_Data.getLocalPlayer()->region->getBlock(vec3_ti(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->aabb.upper, 2.f);
			//	DrawUtils::drawBox(vec3_t(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), vec3_t(floor(corners[n]->x) + 1.f, floor(corners[n]->y - 0.5f) + 1.f, floor(corners[n]->z) + 1.f), 2.f);
			n++;

			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
				//	DrawUtils::setColor(1.f, 0.f, 0.f, 0.5f);
				//	DrawUtils::drawBox(block->floor(), {floor(block->x) + 1.f, floor(block->y) + 1.f, floor(block->z) + 1.f}, 0.7f, false);
				return true;
				//	}
			}
		}

	return false;
}

bool checkSurrounded2(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 0;

	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y+1, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y+1, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y+1, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y+1, entPos.z));
	}

	for (auto blocks : blockChecks) {
		if (!checkTargCollision(&blocks->toVec3t(), ent)) {
			return false;
		}
	}
	return true;
}

std::vector<vec3_t*> getGucciPlacement2(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 0;
	std::vector<vec3_t*> finalBlocks;
	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();
	finalBlocks.clear();
	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y + 1, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y + 1, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y + 1, entPos.z));
	}

	for (auto blocks : blockChecks) {
		auto blkID = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blkIDL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y -1, blocks->z))->toLegacy()->blockId;
		auto blkIDLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y -2, blocks->z))->toLegacy()->blockId;
		auto blkIDLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y +1, blocks->z))->toLegacy()->blockId;
		auto blkIDLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y +2, blocks->z))->toLegacy()->blockId;

		if (!checkTargCollision(&blocks->toVec3t(), ent)) {  //very efficient coding here

			if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			}

			/*if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && (blkIDLLL == 7 || blkIDLLL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 2, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && & blkIDLLL == 0 && (blkIDLLLL == 7 || blkIDLLLL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 3, blocks->z));
			}*/
		} else {
			for (int x = entPos.x - 2; x <= entPos.x + 2; x++) {
				for (int z = entPos.z - 2; z <= entPos.z + 2; z++) {
					int y = entPos.y;
					auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
					auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

					if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
						finalBlocks.push_back(new vec3_t(x, y, z));
					}
				}
			}
		}
	}
	return finalBlocks;
}

bool hasPlaced = false;
void CrystalAuraWTA::onEnable() {
	crystalDelay = 0;
	hasPlaced = false;
}



vec3_t espPosLower;
vec3_t espPosUpper;
vec3_t crystalPos;
std::vector<vec3_t*> placeArr;
std::vector<vec3_t*> hitArr;
void CrystalAuraWTA::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (isClick && !g_Data.isRightClickDown()) return;

	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259) return;

	targetList7.clear();

	g_Data.forEachEntity(findEntity3);
	hitArr.clear();
	placeArr.clear();
	if (spoof) {
		__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		int slot = supplies->selectedHotbarSlot;
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->itemId == 637) {
				slot = n;
			}
			supplies->selectedHotbarSlot = slot;
		}
	}
	if (autoplace)
		if ((crystalDelay >= this->delay) && !(targetList7.empty())) {
			crystalDelay = 0;
			if (!checkSurrounded2(targetList7[0])) {
				std::vector<vec3_t*> gucciPositions = getGucciPlacement2(targetList7[0]);
			
				auto supplies = g_Data.getLocalPlayer()->getSupplies();
				auto inv = supplies->inventory;
				C_ItemStack* item = supplies->inventory->getItemStack(0);

				//615 = normal id for crystal || 616 = crystal id for nukkit servers
				if (!gucciPositions.empty())
					if (g_Data.getLocalPlayer()->getSelectedItemId() == 637 || g_Data.getLocalPlayer()->getSelectedItemId() == 637) {
						placeArr.clear();
						for (auto place : gucciPositions) {
							if (hasPlaced && !doMultiple) break;
							if (targetList7.empty()) return;
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 2, false);
							placeArr.push_back(new vec3_t(place->x, place->y - 1, place->z));
							hasPlaced = true;
						}
					}

				gucciPositions.clear();
			}
		} else if (!targetList7.empty()) {
			crystalDelay++;
		}

	//if (crystalDelay2 >= 20) {
	//		hasPlaced = false;
	//}

	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (targetList7.empty()) return;
		int id = ent->getEntityTypeId();
		int range = moduleMgr->getModule<CrystalAuraWTA>()->range;
		if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
			g_Data.getCGameMode()->attack(ent);
			hasPlaced = true;

			if (!moduleMgr->getModule<NoSwing>()->isEnabled())
				g_Data.getLocalPlayer()->swingArm();
		}
	});  //*/
	for (auto it = toPlace.begin(); it != toPlace.end(); ++it) {
		__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	}
}

void CrystalAuraWTA::onDisable() {
	crystalDelay = 0;
	hasPlaced = false;
	// g_Data.getClientInstance()->getGuiData()->displayClientMessageF("<%sKek.Club+%s> we aint %sgaming %sno more", GREEN, WHITE, RED, RESET);
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
}
/*
* 
* //this right here is stuff i was working on but havent been bothered to finish*/

//*/

void CrystalAuraWTA::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	static auto ThemeMod = moduleMgr->getModule<Theme>();
	int a = 0;
	int b = 0;
	int c = 0;
	static float rcolors2[4];
	static float disabledRcolors2[4];
	static float currColor2[4];
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
	currColor2[0] += 80.f / a * c;
	Utils::ColorConvertHSVtoRGB(currColor2[0], currColor2[1], currColor2[2], currColor2[0], currColor2[1], currColor2[2]);
	if (localPlayer != nullptr && GameData::canUseMoveKeys()) {
		if (!placeArr.empty()) {
			for (auto postt : placeArr) 
			if (ThemeMod->rainbow) {
					DrawUtils::setColor(currColor2[0], currColor2[1], currColor2[2], 1);
					DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, true);
				}
			else{
				DrawUtils::setColor(Theme::rFloat, Theme::gFloat, Theme::bFloat,1);
				DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, true);
			}
		}
		if (!hitArr.empty()) {
			for (auto postt : hitArr) {
				DrawUtils::setColor(Theme::rFloat, Theme::gFloat, Theme::bFloat,1);
				DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, true);
			}
		}
	}
}