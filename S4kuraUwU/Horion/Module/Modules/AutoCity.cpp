#include "AutoCity.h"

#include "../../../Utils/Target.h"

AutoCity::AutoCity() : IModule(0, Category::WORLD, "Un-surround a person") {
	registerFloatSetting("Entity Range", &this->entRange, this->entRange, 0.f, 10.f);
	registerFloatSetting("Break Range", &this->breakRange, this->breakRange, 0.f, 10.f);
	registerIntSetting("Addt. Wait", &this->addtWait, this->addtWait, 0, 60);
	registerBoolSetting("1.12 Mode", &this->onedot12, this->onedot12);
	registerBoolSetting("Turn on CA", &this->turnOnCA, this->turnOnCA);

}

AutoCity::~AutoCity() {
}

const char* AutoCity::getModuleName() {
	return ("AutoCity");
}

int AutoCity::ticksToMineAC(vec3_ti toMine) {
	float speedMultiplier;
	int pickSlot;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318) {  // there's a diamond pick
				pickSlot = n;
				speedMultiplier = 8;
			} else if (stack->getItem()->itemId == 606) {  // there's a netherite pick
				pickSlot = n;
				speedMultiplier = 9;
			}
		}
	}

	if (speedMultiplier != 8 && speedMultiplier != 9)
		return -1;

	// look at the pick's efficiency level
	speedMultiplier += powf((inv->getItemStack(pickSlot)->getEnchantValue(15)), 2) + 1;

	int id = g_Data.getLocalPlayer()->region->getBlock(toMine)->toLegacy()->blockId;
	float blockHardness = 1;
	bool correctBlk = false;

	switch (id) {
	case 145:
		blockHardness = 5.f;
		correctBlk = true;
		break;
	case 49:
		blockHardness = 170.f;
		correctBlk = true;
		break;
	case 130:
		blockHardness = 73.f;
		correctBlk = true;
		break;
	}

	float damage = speedMultiplier / blockHardness;
	damage /= 30;
	//clientMessageF("%i %i %i", (int)(round(1 / damage)), addtWait, (int)(round(1 / damage) + addtWait));
	return (int)(round(1 / damage) + addtWait);
}

void getPickaxeAC() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318 || stack->getItem()->itemId == 606) {  // select pick
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

bool doIHaveAPickAC() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 318) {  // there's a diamond pick
				return true;
			} else if (stack->getItem()->itemId == 606) {  // there's a netherite pick
				return true;
			}
		}
	}

	return false;
}

static std::vector<C_Entity*> niggerList;
bool niggerLooker69(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;  // Skip Local player
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;                    // endcrystal
	if (curEnt->getEntityTypeId() == 66) return false;                    // falling block
	if (curEnt->getEntityTypeId() == 64) return false;                    // item
	if (curEnt->getEntityTypeId() == 69) return false;                    // xp orb
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;  // Don't hit this pesky antibot on 2b2e.org
	if (!Target::isValidTarget(curEnt)) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<AutoCity>()->entRange) {
		niggerList.push_back(curEnt);
		return true;
	}
	return false;
}

bool AutoCity::isBlockGoodForBreaking(vec3_ti* blk, vec3_ti* personPos) {
	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(*blk);
	C_BlockLegacy* blockLegacy = (block->blockLegacy);

	if (blockLegacy->blockId == 7) {  // block is bedrock
		return false;
	}

	if (blk->toVec3t().dist(g_Data.getLocalPlayer()->getHumanPos()) > breakRange) return false; // block is too far

	if (onedot12) {
		if (!g_Data.getLocalPlayer()->region->getBlock(blk->toVec3t().add(0, 1, 0))->toLegacy()->material->isReplaceable) {  // top half is blocked

			vec3_ti delta = blk->sub(*personPos);
			vec3_ti checkMe = blk->add(delta);

			if (!g_Data.getLocalPlayer()->region->getBlock(checkMe.toVec3t())->toLegacy()->material->isReplaceable)  // next block after is also blocked
				return false;
		}
	}

	return true;
}


bool distSorterAC(C_Entity* E1, C_Entity* E2) {
	return g_Data.getLocalPlayer()->getHumanPos().dist(E1->getHumanPos()) < g_Data.getLocalPlayer()->getHumanPos().dist(E2->getHumanPos());
}

bool vecDistSorterAC(vec3_ti E1, vec3_ti E2) {
	return g_Data.getLocalPlayer()->getHumanPos().dist(E1.toVec3t()) < g_Data.getLocalPlayer()->getHumanPos().dist(E2.toVec3t());
}

int origSlutAC;
int hasPickAC = 0;

void AutoCity::onEnable() {
	hasPickAC = 0;
	cityTarg.shouldStart = false;
	cityTarg.mineTime = 0;
}

void AutoCity::onTick(C_GameMode* gm) {
	niggerList.clear();

	if (g_Data.getLocalPlayer() == nullptr) return;

	if (!doIHaveAPickAC()) {
		clientMessageF("Wow! I forgot to bring a pickaxe!!! - Disabling...");
		this->setEnabled(false);
		return;
	}

	g_Data.forEachEntity(niggerLooker69);

	if (niggerList.empty()) {
		clientMessageF("No Entities found within %i blocks! Disabling...", (int)(entRange));
		this->setEnabled(false);
		return;
	}

	std::sort(niggerList.begin(), niggerList.end(), distSorterAC);

	bool canAttack = false;
	while (!canAttack && !cityTarg.shouldStart) {
		vec3_ti loc = (niggerList[0]->getHumanPos().floor());
		bool guyExposed = false;

		const vec3_ti sides[4] = {
			loc.add(1, 0, 0),
			loc.sub(1, 0, 0),
			loc.sub(0, 0, 1),
			loc.add(0, 0, 1)
		};

		std::vector<vec3_ti> possibleCities;

		for (vec3_ti side : sides) {
			// check if for empty sides
			C_Block* block = g_Data.getLocalPlayer()->region->getBlock(side);
			C_BlockLegacy* blockLegacy = (block->blockLegacy);
			if (blockLegacy->material->isReplaceable) {
				if (!onedot12) {
					guyExposed = true;
					break;
				} else if (isBlockGoodForBreaking(&side, &loc)) {
					guyExposed = true;
					break;
				}
			}

			// check for non-bedrock sides
			if (!guyExposed && isBlockGoodForBreaking(&side, &loc)) {
				possibleCities.push_back(side);
			}
		}

		if (!possibleCities.empty()) {
			std::sort(possibleCities.begin(), possibleCities.end(), vecDistSorterAC); // sort to find the block with lowest dist
			clientMessageF("Attempting to city %s...", niggerList[0]->getNameTag()->getText());
			cityTarg.breaker = possibleCities[0];
			cityTarg.ent = niggerList[0];
			cityTarg.entPos = niggerList[0]->getHumanPos().floor();
			cityTarg.mineTime = ticksToMineAC(possibleCities[0]);
			cityTarg.shouldStart = true;
			canAttack = true;
		} else {
			if (niggerList.size() != 1) {
				niggerList.erase(niggerList.begin());  // try again with a different person
			} else {
				clientMessageF("No suitable persons for citying! Disabling...");
				this->setEnabled(false);
				return;
			}
		}
	}

	if (cityTarg.shouldStart) {
		if (cityTarg.ent->getHumanPos().floor() != cityTarg.entPos) {  // opponent moved; probably no longer in hole
			cityTarg.shouldStart = false;
			clientMessageF("Enemy has moved out of hole! Disabling...");
			this->setEnabled(false);
			return;
		} 
	}
}

void AutoCity::onWorldTick(C_GameMode* gm) {
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (cityTarg.shouldStart && cityTarg.mineTime > 0) {
		bool isDestroyed = false;
		gm->startDestroyBlock(cityTarg.breaker, 0, isDestroyed);
		gm->stopDestroyBlock(cityTarg.breaker);
		gm->destroyBlock(&cityTarg.breaker, 0);
		origSlutAC = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;

		cityTarg.mineTime--;
	}

	if (cityTarg.shouldStart && cityTarg.mineTime <= 0) {
		if (hasPickAC < 5) {
			getPickaxeAC();
		}
		if (hasPickAC >= 5) {
			g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlutAC;
			hasPickAC = 0;
			cityTarg.shouldStart = false;
			clientMessageF("Finished citying!");
			if (turnOnCA) {
				auto CBreak = moduleMgr->getModule<CrystalBreak>();
				if (!CBreak->isEnabled())
					CBreak->setEnabled(true);
			}

			this->setEnabled(false);
		}
		hasPickAC++;
	}
	return;
}

void AutoCity::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr || !cityTarg.shouldStart) return;

	if (cityTarg.mineTime > 10) {
		DrawUtils::setColor(.5411765f, .1058824f, 1.f, 1.f);  // 138,27,255
		DrawUtils::drawBox(cityTarg.breaker.toVec3t(), cityTarg.breaker.toVec3t().add(1), .5f, false);
	}
	if (cityTarg.mineTime <= 10) {
		DrawUtils::setColor(1.f, 0.f, 0.f, 1.f);  // 134,21,158
		DrawUtils::drawBox(cityTarg.breaker.toVec3t(), cityTarg.breaker.toVec3t().add(1), .4f, true);
	}
}