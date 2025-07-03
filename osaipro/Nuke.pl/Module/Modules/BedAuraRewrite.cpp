#include "BedAuraRewrite.h"
#include <chrono>
#include <thread>
using namespace std;

// Place Rendering
bool place1 = false;
bool place2 = false;
bool place3 = false;
bool place4 = false;
bool place5 = false;
bool place6 = false;
bool place7 = false;
bool place8 = false;
bool place9 = false;
bool aboveplace = false;
bool placeabove = true;
bool yaw1 = false;
bool yaw2 = false;
bool yaw3 = false;
bool yaw4 = false;
bool debug = false;
bool xIsOpen = false;
bool xzIsOpen = false;
bool place8Open = false;
bool southIsOpen = false;
bool placeOnLow = true;
bool isPhased = false;
bool resetPhaseCheck = false;
// Enums
SettingEnum basePlace;
#pragma region TargetList1
Vec3i blok;
BedAuraRewrite::BedAuraRewrite() : IModule(0x0, Category::COMBAT, "Places and breaks beds") {
	// Placement
	registerEnumSetting("Protocol", &rotations, 0);
	rotations.addEntry("Vanilla", 0);
	rotations.addEntry("Nukkit", 1);
	registerEnumSetting("Rotation", &rotationType, 0);
	rotationType.addEntry("Normal", 0);
	rotationType.addEntry("Silent", 1);

	registerBoolSetting("Break Rotate", &rotateToPlayer, rotateToPlayer);
	registerBoolSetting("Place", &autoplace, autoplace);
	registerBoolSetting("Explode", &explode, explode);
	registerEnumSetting("BasePlace", &basePlace, 0);
	basePlace.addEntry("None", 0);
	basePlace.addEntry("Z+", 1);
	basePlace.addEntry("X+", 2);
	basePlace.addEntry("ZX+", 3);
	basePlace.addEntry("Flat", 4);

	// Safety
	registerBoolSetting("Safety", &safeMode, safeMode);
	registerFloatSetting("Min Health", &minHealth, minHealth, 0.f, 20.f);
	// Target
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Health", 1));
	registerEnumSetting("Priority", &priority, 0);

	// Ranges and Delays
	registerBoolSetting("Instant", &instant, instant);
	registerIntSetting("Place Range", &range, range, 1, 10);
	registerIntSetting("Explode Range", &eRange, eRange, 1, 10);
	registerIntSetting("Place Delay", &delay, delay, 0, 20);
	registerIntSetting("Explode Delay", &breakDelay, breakDelay, 0, 20);
	// Swap
	switchType = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Normal", 1));
	registerEnumSetting("Swap", &this->switchType, 0);

	// Renders
	registerFloatSetting("Opacity", &opacity1, opacity1, 0.f, 1.f);

	// Developer Mode
	registerBoolSetting("Debug", &debug, debug);
}
int bedDelay1 = 0;
int bedDelay21 = 0;
int bedDelay31 = 0;

BedAuraRewrite::~BedAuraRewrite() {
}
const char* BedAuraRewrite::getModuleName() {
	return "BedAura";
}
const char* BedAuraRewrite::getModName() {
	switch (rotations.getSelectedValue()) {
	case 0:
		return "Vanilla";
		break;
	case 1:
		return "Nukkit";
		break;
	}
	return "None";
}
static std::vector<Entity*> targetList71;
struct CompareTargetEnArray1 {
	bool operator()(Entity* lhs, Entity* rhs) {
		LocalPlayer* localPlayer = Game.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void findEntity31(Entity* currentEntity, bool isRegularEntity) {
	static auto BedAuraRewriteMod = moduleMgr->getModule<BedAuraRewrite>();
	if (currentEntity == nullptr) return;
	if (!Game.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!Game.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity->getNameTag()->getTextLength() <= 1) return;
	if (currentEntity->isLocalPlayer()) return;
	std::string invalids[6] = {
	"-", ".", "/",
	"+", ",", "#"
	};
	for (auto invalid : invalids) {
		size_t invalidChar = std::string(currentEntity->getNameTag()->getText()).find(invalid);
		if (invalidChar != std::string::npos) return;
	}
	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < BedAuraRewriteMod->range) {
		targetList71.push_back(currentEntity);
		sort(targetList71.begin(), targetList71.end(), CompareTargetEnArray1());
	}
}
bool checkForCollision(Vec3* block, Entity* ent) {
	std::vector<Vec3*> corners;
	corners.clear();

	corners.push_back(new Vec3(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new Vec3(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new Vec3(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new Vec3(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
	int n = 0;
	if (!corners.empty())
		for (auto corner : corners) {
			n++;

			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z))) {
				return true;
			}
		}

	return false;
}
bool isSurrounded(Entity* ent) {
	Vec3 entPos = ent->getPos()->floor();
	entPos.y -= 1;

	std::vector<Vec3i*> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty()) {
		blockChecks.push_back(new Vec3i(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new Vec3i(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new Vec3i(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new Vec3i(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		if (!checkForCollision(&blocks->toVec3t(), ent)) {
			return false;
		}
	}
	return true;
}
bool isTargetPhased(const Vec3& block, Entity* ent) {
	AABB targetAABB = (ent->getEntityTypeId() != 319) ?
		ent->aabb :
		AABB(ent->getHumanPos().add(-0.3f, 0, -0.3f), ent->getHumanPos().add(0.3f, 1.8f, 0.3f));

	for (int i = 0; i < (ent->getEntityTypeId() != 319 ? 4 : 8); ++i) {
		Vec3 corner = (ent->getEntityTypeId() != 319) ?
			Vec3(i % 2 == 0 ? targetAABB.lower.x : targetAABB.upper.x,
				targetAABB.lower.y, (i < 2) ? targetAABB.lower.z : targetAABB.upper.z) :
			targetAABB.centerPoint().add(targetAABB.lower.sub(targetAABB.upper).mul(0.5f));

		if (!targetAABB.intersectsXZ(AABB(block, 1.0f, 1.0f, 1.0f))) {
			return false; // No intersection, not phased
		}
	}
	return true; // Intersection detected, is phased
}
std::vector<Vec3*> getBestBasePlace(Entity* ent) {
	Vec3 entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<Vec3*> baseBlocks;
	std::vector<Vec3i*> blockChecks2;
	blockChecks2.clear();
	baseBlocks.clear();
	if (blockChecks2.empty()) {
		blockChecks2.push_back(new Vec3i(entPos.x, entPos.y, entPos.z + 1));
		blockChecks2.push_back(new Vec3i(entPos.x, entPos.y, entPos.z - 1));
		blockChecks2.push_back(new Vec3i(entPos.x + 1, entPos.y, entPos.z));
		blockChecks2.push_back(new Vec3i(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks2 : blockChecks2) {
		auto blockY1 = Game.getLocalPlayer()->region->getBlock(*blocks2)->toLegacy()->blockId;
		auto blockY2 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks2->x, blocks2->y - 1, blocks2->z))->toLegacy()->blockId;
		auto blockY3 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks2->x, blocks2->y - 2, blocks2->z))->toLegacy()->blockId;
		auto blockY4 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks2->x, blocks2->y - 3, blocks2->z))->toLegacy()->blockId;
		auto blockY5 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks2->x, blocks2->y - 4, blocks2->z))->toLegacy()->blockId;
		auto surrX = Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49;
		auto surrZ = Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49;
		auto surrXm = Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49;
		auto surrZm = Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49;

		if (!checkForCollision(&blocks2->toVec3t(), ent)) {
			// Z+
			if (basePlace.getSelectedValue() == 1 || basePlace.getSelectedValue() == 3) {
				if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId == 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z + 2));
				}
			}
			// X+
			if (basePlace.getSelectedValue() == 2 || basePlace.getSelectedValue() == 3) {
				if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x + 2, entPos.y, entPos.z))->toLegacy()->blockId == 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z));
				}
			}
			// Flat
			if (basePlace.getSelectedValue() == 4) {
				if (surrX && surrZ && surrXm && surrZm) {
					// X+
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z));
					// X-
					baseBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z));
					baseBlocks.push_back(new Vec3(entPos.x - 2, entPos.y, entPos.z));
					// Z+
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z + 2));
					// Z-
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z - 2));
					// XZ+
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z + 2));
					// XZ-
					baseBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new Vec3(entPos.x - 2, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new Vec3(entPos.x - 2, entPos.y, entPos.z - 1));
					// Merged
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new Vec3(entPos.x + 2, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new Vec3(entPos.x - 2, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new Vec3(entPos.x - 2, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z + 1));
				}
			}
			if (placeabove && !southIsOpen) {
				if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId != 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new Vec3(entPos.x + 1, entPos.y + 1, entPos.z + 1));
				}
				if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId != 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new Vec3(entPos.x, entPos.y + 1, entPos.z + 1));
				}
			}
		}
	}
	return baseBlocks;
}
std::vector<Vec3*> placeArr2;
std::vector<Vec3*> placeArr1;
std::vector<Vec3*> getBestPlacements(Entity* ent) {
	Vec3 entPos = ent->getPos()->floor();
	entPos.y -= 1;
	Vec3* currPos = &ent->getHumanPos();
	float realX = float(currPos->x) - int(currPos->x);
	float realZ = float(currPos->z) - int(currPos->z);
	std::vector<Vec3*> finalBlocks;
	std::vector<Vec3i*> blockChecks;
	blockChecks.clear();
	finalBlocks.clear();
	if (blockChecks.empty()) {
		blockChecks.push_back(new Vec3i(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new Vec3i(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new Vec3i(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new Vec3i(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks) {
		auto blockY1 = Game.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blockY2 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
		auto blockY3 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
		auto blockY4 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
		auto blockY5 = Game.getLocalPlayer()->region->getBlock(Vec3i(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;
		if (!checkForCollision(&blocks->toVec3t(), ent)) {
			// X+
			if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 0 || Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new Vec3(entPos.x + 1, entPos.y, entPos.z));
				yaw3 = true;
				yaw2, yaw1, yaw4 = false;
				place1 = true;
				place2, place3, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					Game.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 1");
			}
			// Z+
			else if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 0 || Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z + 1));
				yaw3 = true;
				yaw1, yaw2, yaw4 = false;
				place2 = true;
				place1, place3, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					Game.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 2");
			}
			// X-
			else if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 0 || Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new Vec3(entPos.x - 1, entPos.y, entPos.z));
				yaw1 = true;
				yaw3, yaw2, yaw4 = false;
				place3 = true;
				place2, place1, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					Game.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 3");
			}
			// Z-
			else if (Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 0 || Game.getLocalPlayer()->region->getBlock(Vec3i(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new Vec3(entPos.x, entPos.y, entPos.z - 1));
				yaw1 = true;
				yaw2, yaw4, yaw3 = false;
				place4 = true;
				place2, place3, place1, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					Game.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 4");
			}
		}
	}
	return finalBlocks;
}
void BedAuraRewrite::onPlayerTick(Player* plr) {

	if (rotationType.getSelectedValue() == 0) {
		if (yaw1) {
			plr->yaw = 90;

		}
		if (yaw2) {
			plr->yaw = 180;

		}
		if (yaw3) {
			plr->yaw = -90;

		}
		if (yaw4) {
			plr->yaw = 0;

		}
	}
	if (rotateToPlayer && rotateNow && rotationType.getSelectedValue() == 0) {
		playerPos = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList71[0]->getPos());
		Vec2 angle = Game.getLocalPlayer()->getPos()->CalcAngle(*targetList71[0]->getPos());
		plr->setRot(playerPos);
		plr->yawUnused1 = angle.y;
	}
}
bool hasPlaced1 = false;
void BedAuraRewrite::onEnable() {
	bedDelay1 = 0;
	hasPlaced1 = false;
}
void BedAuraRewrite::findBestPlace() {
	GameMode* gm = Game.getGameMode();
	bedDelay1++;
	if ((bedDelay1 >= this->delay) && !(targetList71.empty())) {
		if (!isSurrounded(targetList71[0])) {
			std::vector<Vec3*> bestPositions = getBestPlacements(targetList71[0]);
			if (!bestPositions.empty()) {
				int prev = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
				PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
				Inventory* inv = supplies->inventory;
				int slot = supplies->selectedHotbarSlot;
				if (switchType.getSelectedValue() == 2) {
					for (int n = 0; n < 36; n++) {
						ItemStack* stack = inv->getItemStack(n);
						if (stack->item != NULL && (*stack->item)->itemId == 26) {
							slot = n;
						}
						supplies->selectedHotbarSlot = slot;
					}
				}
				if (Game.getLocalPlayer()->getSelectedItemId() == 26 || Game.getLocalPlayer()->getSelectedItemId() == 355) {
					placeArr1.clear();
					for (auto place : bestPositions) {
						Vec3 entPos = Game.getLocalPlayer()->getPos()->floor();
						entPos.y -= 1;
						if (targetList71.empty()) return;
						placeArr2.push_back(&Vec3(place->x, place->y - 1, place->z));
						gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 1, true);
						if (instant) {
							gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 1, true);
							gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 1, true);
							gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 1, true);
							gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 1, true);
						}
						blok = Vec3i(place->x, place->y, place->z);
						placeArr1.push_back(new Vec3(place->x, place->y - 1, place->z));
						hasPlaced1 = false;
					}
				}
				if (switchType.getSelectedValue() == 2) {
					Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prev;
				}
			}
			bestPositions.clear();
			placeArr1.clear();
		}
		bedDelay1 = 0;
	}
	return;
}
void BedAuraRewrite::explodeBeds() {
	GameMode* gm = Game.getGameMode();
	int health = Game.getLocalPlayer()->getHealth();
	bool noDelay = false;
	if (breakDelay != 0)
		noDelay = false;
	else if (breakDelay == 0)
		noDelay = true;
	if (safeMode && health < minHealth)
		return;
	if (!explode)
		return;
	// Find the best bed to break
	bDelay++;
	if (bDelay >= breakDelay) {
		std::vector<Vec3i> bedlist;
		Vec3* pos = gm->player->getPos();
		for (int x = (int)pos->x - eRange; x < pos->x + eRange; x++) {
			for (int z = (int)pos->z - eRange; z < pos->z + eRange; z++) {
				for (int y = (int)pos->y - eRange; y < pos->y + eRange; y++) {
					Vec3i pos = Vec3i(x, y, z);
					Block* block = gm->player->region->getBlock(pos);
					if (block != nullptr && Game.canUseMoveKeys()) {
						auto id = gm->player->region->getBlock(pos)->toLegacy()->blockId;
						if (id == 26) {
							rotateNow = true;
							gm->buildBlock(&pos, 0, true);
							bedlist.push_back(pos);
							if (instant) {
								gm->buildBlock(&pos, 0, true);
								gm->buildBlock(&pos, 0, true);
								gm->buildBlock(&pos, 0, true);
								gm->buildBlock(&pos, 0, true);
							}
						}
					}
				}
			}
		}
		rotateNow = false;
		bDelay = 0;
	}
	return;
}
void BedAuraRewrite::onTick(GameMode* gm) {
	targetList71.clear();
	auto playerHealth = Game.getLocalPlayer()->getHealth();
	int health = Game.getLocalPlayer()->getHealth();
	if (safeMode && health < minHealth && minHealth != 0)
		return;
	moduleMgr->AutoDesync();
	if (Game.getLocalPlayer() == nullptr) return;
	Game.forEachEntity(findEntity31);
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	int BedSlot = supplies->selectedHotbarSlot;
	int slotOBBY = supplies->selectedHotbarSlot;
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != NULL && (*stack->item)->itemId == 26) {
			supplies->selectedHotbarSlot = n;
			break;
		}
	};
	if (autoplace)
		findBestPlace();
	if (explode)
		explodeBeds();
	if (moduleMgr->getModule<Silent>()->isEnabled()) {
		Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = BedSlot;
	}
	// Baseplace
	if (!targetList71.empty()) {
		if (!isSurrounded(targetList71[0])) {
			if (basePlace.getSelectedValue() != 0 || placeabove) {
				std::vector<Vec3*> bestPlace = getBestBasePlace(targetList71[0]);
				int prev2 = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
				if (!bestPlace.empty()) {
					for (int n = 0; n < 36; n++) {
						ItemStack* stack = inv->getItemStack(n);
						if (stack->item != NULL && (*stack->item)->itemId == 49) {
							slotOBBY = n;
						}
						supplies->selectedHotbarSlot = slotOBBY;
					}
					if (!moduleMgr->getModule<Silent>()->isEnabled()) {
						Game.getLocalPlayer()->switchHotbar(slotOBBY);
					}
					for (auto place : bestPlace) {
						Vec3 entPos = Game.getLocalPlayer()->getPos()->floor();
						entPos.y -= 1;
						if (targetList71.empty()) return;
						gm->buildBlock(&Vec3i(place->x, place->y - 1, place->z), 0, true);
					}
				}
				if (moduleMgr->getModule<Silent>()->isEnabled()) {
					Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prev2;
				}
			}
		}
	}
}
void BedAuraRewrite::onDisable() {
	bedDelay1 = 0;
	hasPlaced1 = false;
}
void BedAuraRewrite::onSendPacket(Packet* pkt)
{
	if (pkt->isInstanceOf<PlayerAuthInputPacket>())
	{
		if (rotationType.getSelectedValue() == 1)
		{
			PlayerAuthInputPacket* packet = (PlayerAuthInputPacket*)pkt;
			if (yaw1) {
				packet->yaw = 90;
				packet->yawUnused = 90;

			}
			if (yaw2) {
				packet->yaw = 180;
				packet->yawUnused = 180;

			}
			if (yaw3) {
				packet->yaw = -90;
				packet->yawUnused = -90;

			}
			if (yaw4) {
				packet->yaw = 0;
				packet->yawUnused = 0;

			}
		}
	}
	if (pkt->isInstanceOf<C_MovePlayerPacket>())
	{
		if (rotationType.getSelectedValue() == 1 && rotations.getSelectedValue() == 1)
		{
			auto* packet = (C_MovePlayerPacket*)pkt;
			if (yaw1) {
				packet->yaw = 90;
				packet->headYaw = 90;

			}
			if (yaw2) {
				packet->yaw = 180;
				packet->headYaw = 180;

			}
			if (yaw3) {
				packet->yaw = -90;
				packet->headYaw = -90;

			}
			if (yaw4) {
				packet->yaw = 0;
				packet->headYaw = 0;

			}
		}
		if (rotateToPlayer && rotateNow && rotationType.getSelectedValue() == 1) {
			auto* packet = (C_MovePlayerPacket*)pkt;
			Vec2 anglefd = Game.getLocalPlayer()->getPos()->CalcAngle(targetList71[0]->eyePos0);
			packet->headYaw = anglefd.y;
			packet->yaw = anglefd.y;
		}
	}
}
Vec3 prevRenderPos1;
void BedAuraRewrite::onLevelRender() {
	LocalPlayer* localPlayer = Game.getLocalPlayer();
	if (localPlayer != nullptr && GameData::canUseMoveKeys())
	{
		if (!targetList71.empty())
		{
			if (!placeArr2.empty())
			{
				for (auto postt : placeArr2)
				{
					// Vectors
					Vec3 block = postt->floor().add(0.5, 0.5, 0.5);
					// Variables
					auto interfaceColor = moduleMgr->getModule<Colors>()->getColorV2();
					auto render = renderMode.getSelectedValue();

					// Start Place Check
					// Place 1: X+
					// Place 2: Z+
					// Place 3: X-
					// Place 4: Z-
					// // Place 5: Z+
					// // Place 6: Z+
					// // Place 7: X+
					// // Place 8: XZ+
					// // Place 9: Z-
					// End Place Check

					// The render
					// Outline
					postt->floor() = Vec3(
						prevRenderPos1.x + (postt->floor().x - prevRenderPos1.x) * 0.3f,
						prevRenderPos1.y + (postt->floor().y - prevRenderPos1.y) * 0.3f,
						prevRenderPos1.z + (postt->floor().z - prevRenderPos1.z) * 0.3f
					);
					if (render == 2 || render == 0)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox3d(postt->floor().add(0, 1.f, 0), Vec3(floor(postt->x) + 2.f, floor(postt->y) + 1.5f, floor(postt->z) + 1.f), 1.f, true);
					}
					// Fill
					if (render == 0 || render == 1)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, opacity1);
						DrawUtils::drawBox3dFilled(postt->floor().add(0, 1.f, 0), Vec3(floor(postt->x) + 2.f, floor(postt->y) + 1.5f, floor(postt->z) + 1.f), 0.15, true, 1);
					}
					// 2D
					if (render == 3)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox(postt->floor().add(0, 0.999f, 0), Vec3(floor(postt->x) + 2.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 1.f, false, 1);
					}
					prevRenderPos1 = postt->floor();
					placeArr1.clear();
				}
			}
		}
	}
}
void BedAuraRewrite::onJoiningServer() {
	targetList71.clear();
}