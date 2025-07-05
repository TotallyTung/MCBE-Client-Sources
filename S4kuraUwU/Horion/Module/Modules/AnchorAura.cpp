#include "AnchorAura.h"

#include "../../../SDK/Tag.h"

AnchorAura::AnchorAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Give people PTSD from nukes even before WW3") {
	this->registerIntSetting("range", &this->range, this->range, 3, 10);
	this->registerIntSetting("anchorDelay", &this->anchordelay, this->anchordelay, 0, 10);
	this->registerIntSetting("gsDelay", &this->gsdelay, this->gsdelay, 0, 10);
	this->registerIntSetting("Break delay", &this->breakdelay, this->breakdelay, 0, 10);
	this->registerBoolSetting("Airplace", &this->airplace, this->airplace);
}
AnchorAura::~AnchorAura() {
}
const char* AnchorAura::getModuleName() {
	return ("AnchorAura");
}

static std::vector<C_Entity*> targetList;

void AnchorAura::onEnable() {
	targetList.clear();
}

bool findEntByC(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;  // falling block
	if (curEnt->getEntityTypeId() == 64) return false;  // item
	if (curEnt->getEntityTypeId() == 69) return false;  // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist <= moduleMgr->getModule<AnchorAura>()->range) {
		targetList.push_back(curEnt);
		return true;
	}
	return false;
}

bool placeBloKKK(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}

void chargeAnchor(vec3_t gsplace) {
	gsplace = gsplace.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(gsplace));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(gsplace);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = g_Data.getLocalPlayer()->region->getBlock(gsplace)->toLegacy()->blockId == 527;  //only target respawn anchors
			if (Y) {
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i, true);  // place on face with respawn anchor
			return;
		}
	}
	return;
}

void getAnchor() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == -272)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == -272) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void getGS() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 89)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 89) {  // select anchor
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void AnchorAura::onTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	if (!g_Data.canUseMoveKeys())
		return;
	targetList.clear();
	g_Data.forEachEntity(findEntByC);

	int place = 0;

	if (!targetList.empty()) {
		if (!hasPlacedAnchor) {  // NOT placed anchor
			if (!takenAnchor) {
				getAnchor();
				takenAnchor = true;
				return;
			}
			vec3_t enemyLoc = (targetList[0]->eyePos0).floor();

			//ground level
			bottom1 = enemyLoc.add(1, 0, 0);
			bottom2 = enemyLoc.add(-1, 0, 0);
			bottom3 = enemyLoc.add(0, 0, 1);
			bottom4 = enemyLoc.add(0, 0, -1);
			//top level
			neckBreaker = enemyLoc;
			neckBreaker.y += targetList[0]->height + 1;
			//middle level  ~ last priority because it is the least effective
			mid1 = enemyLoc.add(1, 1, 0);
			mid2 = enemyLoc.add(-1, 1, 0);
			mid3 = enemyLoc.add(0, 1, -1);
			mid4 = enemyLoc.add(0, 1, 1);

			if (g_Data.getLocalPlayer()->region->getBlock(bottom1)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(bottom1);
				else
					gm->buildBlock(&vec3_ti(bottom1), 0, true);
				Option = 1;
			} else if (g_Data.getLocalPlayer()->region->getBlock(bottom2)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(bottom2);
				else
					gm->buildBlock(&vec3_ti(bottom2), 0, true);
				Option = 2;
			} else if (g_Data.getLocalPlayer()->region->getBlock(bottom3)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(bottom3);
				else
					gm->buildBlock(&vec3_ti(bottom3), 0, true);
				Option = 3;
			} else if (g_Data.getLocalPlayer()->region->getBlock(bottom4)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(bottom4);
				else
					gm->buildBlock(&vec3_ti(bottom4), 0, true);
				Option = 4;
			} else if (g_Data.getLocalPlayer()->region->getBlock(neckBreaker)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(neckBreaker);
				else
					gm->buildBlock(&vec3_ti(neckBreaker), 0, true);
				Option = 5;
			} else if (g_Data.getLocalPlayer()->region->getBlock(mid1)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(mid1);
				else
					gm->buildBlock(&vec3_ti(mid1), 0, true);
				Option = 6;
			} else if (g_Data.getLocalPlayer()->region->getBlock(mid2)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(mid2);
				else
					gm->buildBlock(&vec3_ti(mid2), 0, true);
				Option = 7;
			} else if (g_Data.getLocalPlayer()->region->getBlock(mid3)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(mid3);
				else
					gm->buildBlock(&vec3_ti(mid3), 0, true);
				Option = 8;
			} else if (g_Data.getLocalPlayer()->region->getBlock(mid4)->toLegacy()->blockId == 0) {
				if (airplace)
					placeBloKKK(mid4);
				else
					gm->buildBlock(&vec3_ti(mid4), 0, true);
				Option = 9;  // god, that was hell writing that shit
			}
			hasPlacedAnchor = true;
			//clientMessageF("Placed anchor!");
		}

		if (tickTimer >= anchordelay && !DhasPlacedAnchor) {
			tickTimer = 0;
			DhasPlacedAnchor = true;
		} else if (tickTimer < anchordelay && !DhasPlacedAnchor) {
			tickTimer++;
			return;
		}

		if (!hasCharged) {
			if (!takenGS) {
				getGS();
				takenGS = true;
				return;
			}

			switch (Option) {
			case 1:
				gm->buildBlock(&vec3_ti(bottom1), 0, true);
				break;
			case 2:
				gm->buildBlock(&vec3_ti(bottom2), 0, true);
				break;
			case 3:
				gm->buildBlock(&vec3_ti(bottom3), 0, true);
				break;
			case 4:
				gm->buildBlock(&vec3_ti(bottom4), 0, true);
				break;
			case 5:
				gm->buildBlock(&vec3_ti(neckBreaker), 0, true);
				break;
			case 6:
				gm->buildBlock(&vec3_ti(mid1), 0, true);
				break;
			case 7:
				gm->buildBlock(&vec3_ti(mid2), 0, true);
				break;
			case 8:
				gm->buildBlock(&vec3_ti(mid3), 0, true);
				break;
			case 9:
				gm->buildBlock(&vec3_ti(mid4), 0, true);
				break;
			}

			hasCharged = true;
			takenAnchor = false;
			//clientMessageF("Charging!");
		}

		if (tickTimer >= gsdelay && !DhasCharged) {
			tickTimer = 0;
			DhasCharged = true;
		} else if (tickTimer < gsdelay && !DhasCharged) {
			tickTimer++;
			return;
		}

		if (!takenAnchor) {
			getAnchor();
			takenAnchor = true;
			return;
		}

		if (!hasDetonated) {
			switch (Option) {
			case 1:
				gm->buildBlock(&vec3_ti(bottom1), 0, true);
				break;
			case 2:
				gm->buildBlock(&vec3_ti(bottom2), 0, true);
				break;
			case 3:
				gm->buildBlock(&vec3_ti(bottom3), 0, true);
				break;
			case 4:
				gm->buildBlock(&vec3_ti(bottom4), 0, true);
				break;
			case 5:
				gm->buildBlock(&vec3_ti(neckBreaker), 0, true);
				break;
			case 6:
				gm->buildBlock(&vec3_ti(mid1), 0, true);
				break;
			case 7:
				gm->buildBlock(&vec3_ti(mid2), 0, true);
				break;
			case 8:
				gm->buildBlock(&vec3_ti(mid3), 0, true);
				break;
			case 9:
				gm->buildBlock(&vec3_ti(mid4), 0, true);
				break;
			}
			hasDetonated = true;
		}
		//clientMessageF("Detonated!");

		if (tickTimer >= breakdelay) {  // reset variables; prepare for another loop
			hasPlacedAnchor = false;
			hasCharged = false;
			hasDetonated = false;
			DhasPlacedAnchor = false;
			DhasCharged = false;
			tickTimer = 0;
			takenAnchor = false;
			takenGS = false;
			return;
		} else {
			tickTimer++;
			return;
		}
	}
}

void AnchorAura::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
}

void AnchorAura::onDisable() {
}