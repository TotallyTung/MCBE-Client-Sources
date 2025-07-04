#include "CrystalAuraGK.h"

CrystalAuraGK::CrystalAuraGK() : IModule(0, Category::COMBAT, "Blow up target with crystal!!!") {
	//registerBoolSetting("SilentSwitch", &SilentSwitch, SilentSwitch);
	registerIntSetting("range", &range, range, 1, 10);
	registerIntSetting("Radius", &rad, rad, 1, 10);
	registerBoolSetting("Autoplace", &autoplace, autoplace);
	//registerBoolSetting("onlyCrystal", &crystalCheck, crystalCheck);
	//registerBoolSetting("LockY", &yLock, yLock);
	registerIntSetting("delay", &delay, delay, 0, 20);
	registerBoolSetting("onClick", &isClick, isClick);
	registerBoolSetting("Multi", &doMultiple, doMultiple);
	registerBoolSetting("Silent", &silentGK, silentGK);
	registerBoolSetting("Render", &renderGK, renderGK);
	registerBoolSetting("Old render", &oldrdGK, oldrdGK);
	//registerBoolSetting("Obsidian", &obsidianGK, obsidianGK);
}
int crystalDelayidk = 0;
int crystalDelayidk2 = 0;
int crystalDelayidk3 = 0;

CrystalAuraGK::~CrystalAuraGK() {
}

const char* CrystalAuraGK::getModuleName() {
	return ("CrystalAura");
}
static std::vector<C_Entity*> targetListcc7;

void wtf1(C_Entity* currentEntity, bool isRegularEntity) {
	static auto CrystalAuraGKMod = moduleMgr->getModule<CrystalAuraGK>();

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
	if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f)  // Don't hit this pesky antibot on 2b2e.org
		return;
	if (currentEntity->getEntityTypeId() == 64)  // item
		return;
	if (currentEntity->getEntityTypeId() == 69)  // xp_orb
		return;
	if (currentEntity->getEntityTypeId() == 80)  // arrow
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	//how hard is it to play fair? add back the badman check if its hard

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < CrystalAuraGKMod->range) {
		targetListcc7.push_back(currentEntity);
	}
}

bool wtf2(vec3_t* block, C_Entity* ent) {
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
			//if (g_Data.getLocalPlayer()->region->getBlock(vec3i(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->blockId != 0) {
			//DrawUtils::drawBox(vec3(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), g_Data.getLocalPlayer()->region->getBlock(vec3i(corners[n]->x, corners[n]->y - 0.5f, corners[n]->z))->toLegacy()->aabb.upper, 2.f);
			//	DrawUtils::drawBox(vec3(floor(corners[n]->x), floor(corners[n]->y - 0.5f), floor(corners[n]->z)), vec3(floor(corners[n]->x) + 1.f, floor(corners[n]->y - 0.5f) + 1.f, floor(corners[n]->z) + 1.f), 2.f);
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

bool checkSurrounded222(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;

	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		if (!wtf2(&blocks->toVector3(), ent)) {
			return false;
		}
	}
	return true;
}

std::vector<vec3_t*> wtf3(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<vec3_t*> finalBlocks;
	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();
	finalBlocks.clear();
	if (blockChecks.empty()) {
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		auto blkID = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blkIDL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
		auto blkIDLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
		auto blkIDLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
		auto blkIDLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;

		if (!wtf2(&blocks->toVector3(), ent)) {  //very efficient coding here

			if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			}

			/*if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 1, blocks->z));
			} else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && (blkIDLLL == 7 || blkIDLLL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 2, blocks->z));
			} else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && & blkIDLLL == 0 && (blkIDLLLL == 7 || blkIDLLLL == 49)) {
				finalBlocks.push_back(new vec3(blocks->x, blocks->y - 3, blocks->z));
			}*/
			static auto GK = moduleMgr->getModule<CrystalAuraGK>();

			if (GK->doMultiple) {
				for (int x = entPos.x - GK->rad; x <= entPos.x + GK->rad; x++) {
					for (int z = entPos.z - GK->rad; z <= entPos.z + GK->rad; z++) {
						int y = entPos.y;
						auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
						auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

						if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
							finalBlocks.push_back(new vec3_t(x, y, z));
						}
					}
				}
			} else {
				for (int x = entPos.x - GK->rad; x <= entPos.x + GK->rad; x++) {
					for (int z = entPos.z - GK->rad; z <= entPos.z + GK->rad; z++) {
						for (int y = entPos.y - 4; z <= entPos.y + 4; z++) {
							//int y = entPos.y;
							auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
							auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

							if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
								finalBlocks.push_back(new vec3_t(x, y, z));
							}
						}
					}
				}
			}
		}
		return finalBlocks;
	}
}

bool placeCheck = false;
void CrystalAuraGK::onEnable() {
	crystalDelayidk = 0;
	placeCheck = false;
}

std::vector<vec3_t*> placeRender1;
//std::vector<vec3_t*> placeRender2;

void findCrystalGK() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 637) {  // select crystal
				supplies->selectedHotbarSlot = n;
				//return true;
				return;
			}
		}
	}
	//return false;
}

void CrystalAuraGK::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr) return;
	if (isClick && !g_Data.isRightClickDown()) return;

	if (g_Data.getLocalPlayer()->getSelectedItemId() == 259 && g_Data.isRightClickDown()) return;

	targetListcc7.clear();

	g_Data.forEachEntity(wtf1);
	//placeRender2.clear();
	//placeRender1.clear();

	if (autoplace)
		if ((crystalDelayidk >= this->delay) && !(targetListcc7.empty())) {
			crystalDelayidk = 0;
			if (!checkSurrounded222(targetListcc7[0])) {
				std::vector<vec3_t*> gucciPositions = wtf3(targetListcc7[0]);

				auto supplies = g_Data.getLocalPlayer()->getSupplies();
				auto inv = supplies->inventory;
				slotGK = supplies->selectedHotbarSlot;

				//615 = normal id for crystal || 616 = crystal id for nukkit servers
				if (!gucciPositions.empty()) {
					if (silentGK) findCrystalGK();
					if (g_Data.getLocalPlayer()->getSelectedItemId() == 637) {
						placeRender1.clear();
						for (auto place : gucciPositions) {
							if (placeCheck && !doMultiple) break;
							if (targetListcc7.empty()) return;
							//if (silentGK) findCrystalGK();
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
							placeRender1.push_back(new vec3_t(place->x, place->y - 1, place->z));
							placeCheck = true;
						}
					}
					if (silentGK) supplies->selectedHotbarSlot = slotGK;
				}
				gucciPositions.clear();
			}
		} else if (!targetListcc7.empty()) {
			crystalDelayidk++;
		}

	//if (crystalDelayidk2 >= 20) {
	//		placeCheck = false;
	//}

	g_Data.forEachEntity([](C_Entity* ent, bool b) {
		if (targetListcc7.empty()) return;
		int id = ent->getEntityTypeId();
		int range = moduleMgr->getModule<CrystalAuraGK>()->range;
		if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= range) {
			g_Data.getCGameMode()->attack(ent);
			placeCheck = false;

			if (!moduleMgr->getModule<NoSwing>()->isEnabled())
				g_Data.getLocalPlayer()->swingArm();
		}
	});  //*/
	
}

void CrystalAuraGK::onDisable() {
	crystalDelayidk = 0;
	placeCheck = false;
}
/*
* 
* //this right here is stuff i was working on but havent been bothered to finish*/

//*/

void CrystalAuraGK::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (renderGK) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		auto cd = ColorUtil::interfaceColor(1);
		if (localPlayer != nullptr && GameData::canUseMoveKeys()) {

			if (!targetListcc7.empty()) {
				if (!placeRender1.empty()) {
					for (auto postt : placeRender1) {
						DrawUtils::setColor(cd.r, cd.g, cd.b, 1.f);
						if (!oldrdGK) {
							DrawUtils::drawBox(postt->floor().add(0.f, 0.999f, 0.f), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 0.5f, true);
						}
						else
						{
							DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 0.5f, true);
						}
						//DrawUtils::drawwtf(*postt, 0.5f);
					}
				}
			}
			/*
			if (!placeRender2.empty()) {
				for (auto postt : placeRender2) {
					DrawUtils::setColor(cd.r, cd.g, cd.b, 1.f);
					DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, true);
				}
			}  //*/
		}
	}
}
