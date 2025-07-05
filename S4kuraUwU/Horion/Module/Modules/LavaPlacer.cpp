#include "LavaPlacer.h"


LavaPlacer::LavaPlacer() : IModule(0, Category::COMBAT, "added this cause funni in new anarchy servers") {
	registerIntSetting("Range", &this->range, this->range, 2, 10);
	registerIntSetting("Dwell time", &this->dwellTime, this->dwellTime, 0, 25);
	//registerBoolSetting("Predict", &this->predict, this->predict);
}

LavaPlacer::~LavaPlacer(){};

const char* LavaPlacer::getModuleName() {
	return ("LavaPlacer");
}

static std::vector<C_Entity*> targetList;

void locTargEntity(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return;
	if (curEnt == g_Data.getLocalPlayer()) return;  // Skip Local player
	if (!curEnt->isAlive()) return;
	if (!g_Data.getLocalPlayer()->isAlive()) return;
	if (curEnt->getEntityTypeId() == 71) return;  // endcrystal
	if (curEnt->getEntityTypeId() == 66) return;  // falling block
	if (curEnt->getEntityTypeId() == 64) return;  // item
	if (curEnt->getEntityTypeId() == 69) return;  // xp orb
	if (!Target::isValidTarget(curEnt)) return;

	float dist = (*curEnt->getPos()).dist(*g_Data.getLocalPlayer()->getPos());
	if (dist < moduleMgr->getModule<AnvilAura>()->AArange) {
		targetList.push_back(curEnt);
	}
}

bool placeLava(vec3_t blkPlacement) {
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

void LavaPlacer::placeDecisions(vec3_t placeHere) {
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 363)  // lava bucket
		placeLava(placeHere);
	else if (g_Data.getLocalPlayer()->getSelectedItemId() == 360) {  // empty, must wait until dwell time is finished to pick up lava
		if (pDelay >= dwellTime) {
			//g_Data.getCGameMode()->useItemOn();
			placeLava(placeHere);
			pDelay = 0;
		} else {
			pDelay++;
			return;
		}
	}
}

void getLBucket() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	if (g_Data.getLocalPlayer()->getSelectedItemId() == 363)
		return;

	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 363) {  // select lavabucket
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void LavaPlacer::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	targetList.clear();

	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	getLBucket();
}

void LavaPlacer::onTick(C_GameMode* gm) {
	if ((g_Data.getLocalPlayer() == nullptr) || (!g_Data.canUseMoveKeys()))
		return;

	if (!(g_Data.getLocalPlayer()->getSelectedItemId() == 363 || g_Data.getLocalPlayer()->getSelectedItemId() == 360)) {  // 363 = lava, 360 = empty
		clientMessageF("Bucket not found, disabling!");
		this->setEnabled(false);
	}
	g_Data.forEachEntity(locTargEntity);

	if (!targetList.empty()) {
		auto targThis = targetList[0];

		vec3_t enemyLoc = (targThis->eyePos0).floor();

		float speed = targThis->velocity.magnitudexz();  // shamelessly copied from scaffold module
		vec3_t vel = targThis->velocity;
		vel = vel.normalize();  // Only use values from 0 - 1

		//clientMessageF("Lava is suppossed to be placed on %f %f %f", enemyLoc.x, enemyLoc.y, enemyLoc.z); debug purposes

		placeDecisions(enemyLoc);

		//if (speed < .05f) { // enemy not moving; place directly on feet
		//}
	}
}

void LavaPlacer::onDisable() {
	g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlot;
}