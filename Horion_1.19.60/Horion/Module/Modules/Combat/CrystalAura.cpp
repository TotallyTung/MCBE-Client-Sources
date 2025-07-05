#include "CrystalAura.h"

CrystalAura::CrystalAura() : IModule(VK_NUMPAD0, Category::COMBAT, "Automatically places and destroys crystals near other players") {
	registerFloatSetting("Range", &this->range, this->range, 1.f, 10.f);
	registerIntSetting("Place Delay", &this->placeDelay, this->placeDelay, 0, 10);
	registerIntSetting("Attack Delay", &this->attackDelay, this->attackDelay, 0, 10);
	registerBoolSetting("Auto Place", &this->autoPlace, this->autoPlace);
	registerBoolSetting("Multi Place", &this->multiPlace, this->multiPlace);
}

CrystalAura::~CrystalAura() {
}

const char* CrystalAura::getModuleName() {
	return ("CrystalAura");
}

std::string CrystalAura::getModeText() {
	return multiPlace ? "Multi" : "Single";
}

int Odelay = 0;//Place Delay
int Ddelay = 0;//Attack Delay
static std::vector<Entity*> targetList;
static std::vector<Entity*> CrystalList;
void FindValidEnts(Entity* currentEntity) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");

	if (!currentEntity)
		return;

	if (currentEntity == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntity))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;
	
	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < CrystalAuraMod->range) {
		if (Target::isValidTarget(currentEntity, false, false))
				targetList.push_back(currentEntity);

		if (currentEntity->getEntityTypeId() == 71)
			CrystalList.push_back(currentEntity);
	}
}

void CrystalAura::onEnable() {
	Odelay = 0;
	Ddelay = 0;
}

std::vector<Vec3i> getCrystalPlacementsFromTarget(Entity* ent) {
	static auto CrystalAuraMod = moduleMgr->getModule<CrystalAura>("CrystalAura");
	Vec3 entPos = ent->getPos()->floor();
	entPos.y -= 1.f;

	std::vector<Vec3i> ValidPlacements;

	for (int x = (int)entPos.x - 5; x < entPos.x + 5; x++) {
		for (int z = (int)entPos.z - 5; z < entPos.z + 5; z++) {
			for (int y = (int)entPos.y - 2; y < entPos.y + 1; y++) {
				auto blockid = Game.getRegion()->getBlock(Vec3i(x, y, z))->toLegacy()->blockId;
				if (blockid == 49 || blockid == 7) {
					Vec3i blockPos = Vec3i(x, y, z);
					if (Game.getRegion()->getBlock(blockPos.add(0, 1, 0))->toLegacy()->blockId != 0 &&
						Game.getRegion()->getBlock(blockPos.add(0, 2, 0))->toLegacy()->blockId != 0)  //stops from placing in one by one holes
						continue;

					AABB PlacementAABB(blockPos.toVec3t(), blockPos.toVec3t().add(1, 2, 1));
					bool shouldContinue = false;
					for (auto crystals : CrystalList) {//check if the placement position is already occupied
						if (PlacementAABB.intersects(crystals->aabbShape->aabb)) {
							shouldContinue = true;
							break;
						}
					}
					if (shouldContinue) continue;
					ValidPlacements.push_back(blockPos);
				}
			}
		}
	}
	return ValidPlacements;
}

float DamageToEntity(Entity* Target, Vec3i PlacementPos) {
	//not sure if this is actually how you calculate the damage, but someone on a
	//youtube video said this is how its done, and it works so :troll:
	float DmgRange = 12.f;
	Vec3 pos = *Target->getPos();
	float impact = (1 - pos.dist(PlacementPos.toVec3t()) / DmgRange);
	return ((impact * impact + impact) / 2) * 8 * DmgRange + 1;
}

struct CompareDmg {
	bool operator()(Vec3i lhs, Vec3i rhs) {
		return DamageToEntity(targetList[0], lhs) > DamageToEntity(targetList[0], rhs);
	}
};

struct CompareCrystaltEnArray {
	bool operator()(Entity* lhs, Entity* rhs) {
		return (*lhs->getPos()).dist(*targetList[0]->getPos()) < (*rhs->getPos()).dist(*targetList[0]->getPos());
	}
};


void CrystalAura::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (!player) return;
	
	targetList.clear();
	CrystalList.clear();
	Game.forEachEntity(FindValidEnts);
	if (targetList.empty()) return;

	if (autoPlace) {
		std::vector<Vec3i> placements = getCrystalPlacementsFromTarget(targetList[0]);
		if (Odelay > placeDelay && !placements.empty()) {
			Odelay = 0;
			auto supplies = Game.getLocalPlayer()->getSupplies();
			int slot = supplies->selectedHotbarSlot;
			auto SelectedSlot = supplies->inventory->getItemStack(slot);
			std::sort(placements.begin(), placements.end(), CompareDmg());
			if (SelectedSlot->isValid() && strcmp(SelectedSlot->getItem()->tileName.c_str(), "item.end_crystal") == 0)//crystal id moves alot so yea
			for (auto& place : placements) {
				gm->buildBlock(place, 1);
				if (!multiPlace) break;
			}
		}
		Odelay++;
	}

	if (Ddelay > placeDelay && !CrystalList.empty()) {
		Ddelay = 0;
		std::sort(CrystalList.begin(), CrystalList.end(), CompareCrystaltEnArray());
		if (player->getPos()->dist(*CrystalList[0]->getPos()) <= range) {
			player->swing();
			gm->attack(CrystalList[0]);
		}
	}
	Ddelay++;
}