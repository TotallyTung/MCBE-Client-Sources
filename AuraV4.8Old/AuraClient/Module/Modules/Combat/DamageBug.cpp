#include "DamageBug.h"

DamageBug::DamageBug() : IModule(0x0, Category::COMBAT, "Bug damage in 2b2tmcpe and 2b2tpe") {
	registerFloatSetting("Range Attack", &range, range, 0, 20);
}
const char* DamageBug::getRawModuleName() {
	return "DamageBug";
}
const char* DamageBug::getModuleName() {
	return "DamageBug";
}
struct CompareTargetEnArray {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};
static std::vector<C_Entity*> targetListdb;
void findEntitydb(C_Entity* currentEntity, bool isRegularEntity) {
	auto dmgdb = moduleMgr->getModule<DamageBug>();
	if (currentEntity == nullptr)
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;

	if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
		return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getEntityTypeId() == 66) // falling block
		return;

	if (currentEntity->getEntityTypeId() == 69)  // XP
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < dmgdb->range) {
		targetListdb.push_back(currentEntity);
		sort(targetListdb.begin(), targetListdb.end(), CompareTargetEnArray());
	}
}
/*
void DamageBug::onPlayerTick(C_Player* plr) {
	auto checklgd = moduleMgr->getModule<Verify>();
	if (!(checklgd->passedTest)) {
		clientMessageF("%sYou need to verified to use Killaura", RED);
		this->setEnabled(false);
	}
	else {
		targetListEmptyd = targetListdb.empty();
		targetListdb.clear();
		g_Data.forEachEntity(findEntitydb);
		if (!targetListdb.empty()) {
			C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
			C_Inventory* inv = supplies->inventory;
			slotdb = supplies->selectedHotbarSlot;
			//supplies->selectedHotbarSlot = slotdb1;
			for (auto& idb : targetListdb) {
				supplies->selectedHotbarSlot = slotdb1;
				//g_Data.getCGameMode()->attack(idb);
				supplies->selectedHotbarSlot = slotdb2;
				g_Data.getCGameMode()->attack(idb);
				supplies->selectedHotbarSlot = slotdb;
			}

		}

	}
}
*/

void DamageBug::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	int dbrr = g_Data.getLocalPlayer()->getSelectedItemId();
	if (player == nullptr) return;

	targetListEmptyd = targetListdb.empty();

	targetListdb.clear();

	g_Data.forEachEntity(findEntitydb);
	if (!targetListdb.empty()) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		supplies->selectedHotbarSlot = random(0, 8);
		for (auto& i2cc : targetListdb) {
			gm->attack(i2cc);
		}
	}
}
