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
vec3_ti blok;
BedAuraRewrite::BedAuraRewrite() : IModule(0x0, Category::COMBAT, "Places and breaks beds") {
	// Placement
	registerEnumSetting("Protocol", &rotations, 0, "Protocol for bedaura logic");
	rotations.addEntry("Vanilla", 0);
	rotations.addEntry("Nukkit", 1);
	registerEnumSetting("Rotation", &rotationType, 0, "Control bedaura rotation type");
	rotationType.addEntry("Normal", 0);
	rotationType.addEntry("Silent", 1);

	registerBoolSetting("Break Rotate", &rotateToPlayer, rotateToPlayer, "Rotate when breaking automatically");
	registerBoolSetting("Place", &autoplace, autoplace, "Place automatically");
	registerBoolSetting("Explode", &explode, explode, "Explode automatically");
	registerBoolSetting("Place Above", &placeabove, placeabove, "Place above on their heads if no positions are available");
	registerEnumSetting("BasePlace", &basePlace, 0, "Place obsidian to place beds on");
	basePlace.addEntry("None", 0);
	basePlace.addEntry("Z+", 1);
	basePlace.addEntry("X+", 2);
	basePlace.addEntry("ZX+", 3);
	basePlace.addEntry("Flat", 4);

	// Safety
	registerBoolSetting("Safety", &safeMode, safeMode, "Take health into account");
	registerFloatSetting("Min Health", &minHealth, minHealth, 0.f, 20.f, 0.1f, "If health is less than this then the client will stop placing");
	registerBoolSetting("Ignore Eat", &ignoreEat, ignoreEat, "Will place anyways even if eating");
	// Target
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Health", 1));
	registerEnumSetting("Priority", &priority, 0, "Decides how targets are sorted");

	// Ranges and Delays
	registerBoolSetting("Instant", &instant, instant, "Place and Break instantly");
	registerIntSetting("Place Range", &range, range, 1, 10, "Range of placements");
	registerIntSetting("Explode Range", &eRange, eRange, 1, 10, "Range of explosion");
	registerIntSetting("Place Delay", &delay, delay, 0, 20, "Delay of placements");
	registerIntSetting("Explode Delay", &breakDelay, breakDelay, 0, 20, "Delay of bed explosions");
	// Swap
	switchType = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("NukkitSpoof", 2));
	registerEnumSetting("Swap", &this->switchType, 0, "Swap to Beds");

	// Renders
	renderMode = SettingEnum(this)
		.addEntry(EnumEntry("Both", 0))
		.addEntry(EnumEntry("Fill", 1))
		.addEntry(EnumEntry("Box", 2))
		.addEntry(EnumEntry("2D", 3))
		.addEntry(EnumEntry("None", 4));
	registerEnumSetting("Render", &this->renderMode, 0, "Controls how the client will render");
	registerFloatSetting("Alpha", &opacity1, opacity1, 0.f, 1.f, 0.01f, "Opacity of the fill renders");
	registerFloatSetting("Line Alpha", &OutOpacity, OutOpacity, 0.f, 1.f, 0.01f, "Opacity of the outlines");

	// Developer Mode
	registerBoolSetting("Debug", &debug, debug, "Do debug things");
	registerBoolSetting("Debug Phase", &resetPhaseCheck, resetPhaseCheck);
}
int bedDelay1 = 0;
int bedDelay21 = 0;
int bedDelay31 = 0;

BedAuraRewrite::~BedAuraRewrite() {
}
const char* BedAuraRewrite::getRawModuleName() {
	return "BedAura";
}
const char* BedAuraRewrite::getModName() {
	switch (rotations.getSelectedValue()) {
	case 0:
		return " Vanilla";
		break;
	case 1:
		return " Nukkit";
		break;
	case 2:
		return " Nukkit";
		break;
	}
}
static std::vector<C_Entity*> targetList71;
struct CompareTargetEnArray1 {
	bool operator()(C_Entity* lhs, C_Entity* rhs) {
		C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
		return (*lhs->getPos()).dist(*localPlayer->getPos()) < (*rhs->getPos()).dist(*localPlayer->getPos());
	}
};

void findEntity31(C_Entity* currentEntity, bool isRegularEntity) {
	static auto BedAuraRewriteMod = moduleMgr->getModule<BedAuraRewrite>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 26)  // crystal
		return;

	if (currentEntity == g_Data.getLocalPlayer())  // Skip Local player
		return;
	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 319)
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

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < BedAuraRewriteMod->range) {
		targetList71.push_back(currentEntity);
		sort(targetList71.begin(), targetList71.end(), CompareTargetEnArray1());
	}
}
bool checkForCollision(vec3_t* block, C_Entity* ent) {
	std::vector<vec3_t*> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
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
bool isSurrounded(C_Entity* ent) {
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
		if (!checkForCollision(&blocks->toVector3(), ent)) {
			return false;
		}
	}
	return true;
}
bool isTargetPhased(const vec3_t& block, C_Entity* ent) {
	AABB targetAABB = (ent->getEntityTypeId() != 319) ?
		ent->aabb :
		AABB(ent->getHumanPos().add(-0.3f, 0, -0.3f), ent->getHumanPos().add(0.3f, 1.8f, 0.3f));

	for (int i = 0; i < (ent->getEntityTypeId() != 319 ? 4 : 8); ++i) {
		vec3_t corner = (ent->getEntityTypeId() != 319) ?
			vec3_t(i % 2 == 0 ? targetAABB.lower.x : targetAABB.upper.x,
				targetAABB.lower.y, (i < 2) ? targetAABB.lower.z : targetAABB.upper.z) :
			targetAABB.centerPoint().add(targetAABB.lower.sub(targetAABB.upper).mul(0.5f));

		if (!targetAABB.intersectsXZ(AABB(block, 1.0f, 1.0f, 1.0f))) {
			return false; // No intersection, not phased
		}
	}
	return true; // Intersection detected, is phased
}
std::vector<vec3_t*> getBestBasePlace(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<vec3_t*> baseBlocks;
	std::vector<vec3_ti*> blockChecks2;
	blockChecks2.clear();
	baseBlocks.clear();
	if (blockChecks2.empty()) {
		blockChecks2.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks2.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks2.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks2.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks2 : blockChecks2) {
		auto blockY1 = g_Data.getLocalPlayer()->region->getBlock(*blocks2)->toLegacy()->blockId;
		auto blockY2 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks2->x, blocks2->y - 1, blocks2->z))->toLegacy()->blockId;
		auto blockY3 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks2->x, blocks2->y - 2, blocks2->z))->toLegacy()->blockId;
		auto blockY4 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks2->x, blocks2->y - 3, blocks2->z))->toLegacy()->blockId;
		auto blockY5 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks2->x, blocks2->y - 4, blocks2->z))->toLegacy()->blockId;
		auto surrX = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49;
		auto surrZ = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49;
		auto surrXm = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 49;
		auto surrZm = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49;

		if (!checkForCollision(&blocks2->toVector3(), ent)) {
			// Z+
			if (basePlace.getSelectedValue() == 1 || basePlace.getSelectedValue() == 3) {
				if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId == 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 2));
				}
			}
			// X+
			if (basePlace.getSelectedValue() == 2 || basePlace.getSelectedValue() == 3) {
				if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y, entPos.z))->toLegacy()->blockId == 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z));
				}
			}
			// Flat
			if (basePlace.getSelectedValue() == 4) {
				if (surrX && surrZ && surrXm && surrZm) {
					// X+
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z));
					// X-
					baseBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z));
					baseBlocks.push_back(new vec3_t(entPos.x - 2, entPos.y, entPos.z));
					// Z+
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 2));
					// Z-
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z - 2));
					// XZ+
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z + 2));
					// XZ-
					baseBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 2, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 2, entPos.y, entPos.z - 1));
					// Merged
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z - 1));
					baseBlocks.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 2, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 2, entPos.y, entPos.z + 1));
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z - 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z + 2));
					baseBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z + 1));
				}
			}
			if (placeabove && !southIsOpen) {
				if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId != 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y + 1, entPos.z + 1));
				}
				if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId != 0 && surrX && surrZ && surrXm && surrZm) {
					baseBlocks.push_back(new vec3_t(entPos.x, entPos.y + 1, entPos.z + 1));
				}
			}
		}
	}
	return baseBlocks;
}
std::vector<vec3_t*> placeArr2;
std::vector<vec3_t*> placeArr1;
std::vector<vec3_t*> getBestPlacements(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	vec3_t* currPos = &ent->getHumanPos();
	float realX = float (currPos->x) - int (currPos->x);
	float realZ = float (currPos->z) - int (currPos->z);
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
		auto blockY1 = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blockY2 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
		auto blockY3 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
		auto blockY4 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
		auto blockY5 = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;
		if (!checkForCollision(&blocks->toVector3(), ent)) {
			placeArr2.clear();
			// Renders
			// FeetPlace
			if (place1 || place2 || place3)
				placeArr2.push_back(new vec3_t(entPos.x, entPos.y - 1, entPos.z));
			if (place4)
				placeArr2.push_back(new vec3_t(entPos.x, entPos.y - 1, entPos.z - 1));
			// FacePlace
			if (place5 || place6)
				placeArr2.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 1));
			if (aboveplace)
				placeArr2.push_back(new vec3_t(entPos.x, entPos.y + 1, entPos.z + 1));
			// Checks
			// 2 checks, 1 for bed and 1 for air so it stops multimplacing!, combining 0,26 blockid breaks it idk why
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId != 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId != 0) {
				southIsOpen = false;
				//if (debug)
					//g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura South: Unavailable");
			}
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId != 26 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId != 26) {
				southIsOpen = false;
			}
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 1))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 2))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId != 0, 26) {
				xzIsOpen = false;
				//if (debug)
					//g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura NE: Unavailable");
			}
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y, entPos.z))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z))->toLegacy()->blockId != 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y + 1, entPos.z))->toLegacy()->blockId != 0) {
				place8Open = false;
			}
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y, entPos.z))->toLegacy()->blockId != 49, 7 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z))->toLegacy()->blockId != 26 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y + 1, entPos.z))->toLegacy()->blockId != 26) {
				place8Open = false;
			}
			if (currPos->x > 0 && currPos->z > 0) {
				if (realX >= 0.70 && realZ < 0.30) {
					if (debug)
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Phase: 1");
				}
				if (realX >= 0.70 && realZ >= 0.70) {
					if (debug)
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Phase: 2");
				}
				if (realX <= 0.70 && realZ > 0.30) {
					if (debug)
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Phase: 3");
				}
				if (realX > 0.30 && realZ >= 0.70) {
					if (debug)
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Phase: 4");
				}
				if (realX < 0.30 && realZ <= 0.70) {
					if (debug)
						g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Phase: 5");
				}
			}
			// X+
			if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z));
				yaw3 = true;
				yaw2, yaw1, yaw4 = false;
				place1 = true;
				place2, place3, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 1");
			}
			// Z+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z + 1));
				yaw3 = true;
				yaw1, yaw2, yaw4 = false;
				place2 = true;
				place1, place3, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 2");
			}
			// X-
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y, entPos.z))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new vec3_t(entPos.x - 1, entPos.y, entPos.z));
				yaw1 = true;
				yaw3, yaw2, yaw4 = false;
				place3 = true;
				place2, place1, place4, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 3");
			}
			// Z-
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 0 || g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z - 1))->toLegacy()->blockId == 26) {

				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z));
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y, entPos.z - 1));
				yaw1 = true;
				yaw2, yaw4, yaw3 = false;
				place4 = true;
				place2, place3, place1, place5, place6, place7, place8, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 4");
			}

			// Face place
			// Z+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y, entPos.z + 2))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId == 0) {
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y + 1, entPos.z + 1));
				yaw4 = true;
				yaw2, yaw3, yaw1 = false;
				place5 = true;
				place2, place3, place4, place6, place1, place7, place8, place9, aboveplace = false;
				southIsOpen = true;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 5");
			}
			// XZ+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z + 2))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId == 0 && !southIsOpen) {

				finalBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y + 1, entPos.z + 1));
				yaw1, yaw3, yaw4 = false;
				yaw2 = true;
				place6 = true;
				place2, place3, place4, place1, place5, place7, place8, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 6");
			}
			// XZ-
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z - 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z - 1))->toLegacy()->blockId == 0 && !southIsOpen && !xzIsOpen && !place8Open) {

				finalBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y + 1, entPos.z - 1));
				placeArr2.push_back(new vec3_t(entPos.x + 1, entPos.y, entPos.z - 1));
				yaw1, yaw3, yaw4 = false;
				yaw2 = true;
				place7 = true;
				place2, place3, place4, place1, place6, place5, place8, place9, aboveplace = false;
				if (debug)
			   		g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 7");
			}
			// X+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y, entPos.z))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y, entPos.z))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 2, entPos.y + 1, entPos.z))->toLegacy()->blockId == 0 && !southIsOpen && !xzIsOpen && placeOnLow) {
				finalBlocks.push_back(new vec3_t(entPos.x + 1, entPos.y + 1, entPos.z));
				placeArr2.push_back(new vec3_t(entPos.x + 2, entPos.y, entPos.z));
				yaw4 = true;
				yaw2, yaw3, yaw1 = false;
				xIsOpen = true;
				place8 = true;
				place8Open = true;
				place2, place3, place4, place5, place6, place1, place7, place9, aboveplace = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 8");
			}
			// Above Head
			// Z+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 2, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x, entPos.y + 2, entPos.z + 2))->toLegacy()->blockId == 0) {
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y + 2, entPos.z + 1));
				yaw4 = true;
				yaw2, yaw3, yaw1 = false;
				aboveplace = true;
				place2, place3, place4, place6, place1, place7, place8, place9, place5 = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 9");
			}
			// ZX+
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 2, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x + 1, entPos.y + 2, entPos.z + 2))->toLegacy()->blockId == 0) {
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y + 2, entPos.z + 1));
				yaw4 = true;
				yaw2, yaw3, yaw1 = false;
				aboveplace = true;
				place2, place3, place4, place6, place1, place7, place8, place9, place5 = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 10");
			}
			// ZX-
			else if (g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y + 1, entPos.z + 1))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y + 1, entPos.z + 2))->toLegacy()->blockId == 49, 7 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y + 2, entPos.z + 1))->toLegacy()->blockId == 0 && g_Data.getLocalPlayer()->region->getBlock(vec3_ti(entPos.x - 1, entPos.y + 2, entPos.z + 2))->toLegacy()->blockId == 0) {
				finalBlocks.push_back(new vec3_t(entPos.x, entPos.y + 2, entPos.z + 1));
				yaw1 = true;
				yaw2, yaw4, yaw3 = false;
				aboveplace = true;
				place2, place3, place4, place6, place1, place7, place8, place9, place5 = false;
				if (debug)
					g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[Debug] BedAura Place: 11");
			}
		}
	}
	return finalBlocks;
}
void BedAuraRewrite::onPlayerTick(C_Player* plr) {

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
		playerPos = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList71[0]->getPos());
		vec2_t angle = g_Data.getLocalPlayer()->getPos()->CalcAngle(*targetList71[0]->getPos());
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
	C_GameMode* gm = g_Data.getCGameMode();
	bedDelay1++;
	if ((bedDelay1 >= this->delay) && !(targetList71.empty())) {

		if (!isSurrounded(targetList71[0])) {
			std::vector<vec3_t*> bestPositions = getBestPlacements(targetList71[0]);
			if (!bestPositions.empty()) {
				int prev = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
				C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
				C_Inventory* inv = supplies->inventory;
				int slot = supplies->selectedHotbarSlot;
				if (switchType.getSelectedValue() == 2) {
					for (int n = 0; n < 9; n++) {
						C_ItemStack* stack = inv->getItemStack(n);
						if (stack->item != NULL && (*stack->item)->itemId == 26) {
							slot = n;
						}
						supplies->selectedHotbarSlot = slot;
					}
				}
				if (g_Data.getLocalPlayer()->getSelectedItemId() == 26 || g_Data.getLocalPlayer()->getSelectedItemId() == 355) {
					placeArr1.clear();
					for (auto place : bestPositions) {
						vec3_t entPos = g_Data.getLocalPlayer()->getPos()->floor();
						entPos.y -= 1;
						if (targetList71.empty()) return;
						gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
						if (instant) {
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
						}
						blok = vec3_ti(place->x, place->y, place->z);
						placeArr1.push_back(new vec3_t(place->x, place->y - 1, place->z));
						hasPlaced1 = false;
					}
				}
				if (switchType.getSelectedValue() == 2) {
					g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prev;
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
	C_GameMode* gm = g_Data.getCGameMode();
	int health = g_Data.getLocalPlayer()->getHealth();
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
		std::vector<vec3_ti> bedlist;
		vec3_t* pos = gm->player->getPos();
		for (int x = (int)pos->x - eRange; x < pos->x + eRange; x++) {
			for (int z = (int)pos->z - eRange; z < pos->z + eRange; z++) {
				for (int y = (int)pos->y - eRange; y < pos->y + eRange; y++) {
					vec3_ti pos = vec3_ti(x, y, z);
					C_Block* block = gm->player->region->getBlock(pos);
					if (block != nullptr && g_Data.canUseMoveKeys()) {
						auto id = gm->player->region->getBlock(pos)->toLegacy()->blockId;
						if (id == 26) {
							rotateNow = true;
							gm->buildBlock(&pos, 0);
							bedlist.push_back(pos);
							if (instant) {
								gm->buildBlock(&pos, 0);
								gm->buildBlock(&pos, 0);
								gm->buildBlock(&pos, 0);
								gm->buildBlock(&pos, 0);
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
void BedAuraRewrite::onTick(C_GameMode* gm) {
	auto playerHealth = g_Data.getLocalPlayer()->getHealth();
	int health = g_Data.getLocalPlayer()->getHealth();
	if (safeMode && health < minHealth && minHealth != 0)
		return;
	if (!ignoreEat)
	{
		auto player = g_Data.getLocalPlayer();
		if (player->getSelectedItemId() == 259 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 300 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 426 && g_Data.isRightClickDown())
			return;
	}
	if (g_Data.getLocalPlayer() == nullptr) return;
		targetList71.clear();

		g_Data.forEachEntity(findEntity31);
	if (autoplace)
		findBestPlace();
	if (explode)
		explodeBeds();
	// Baseplace
	if (!targetList71.empty()) {
		if (!isSurrounded(targetList71[0])) {
			if (basePlace.getSelectedValue() != 0 || placeabove) {
				std::vector<vec3_t*> bestPlace = getBestBasePlace(targetList71[0]);
				int prev2 = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
				if (!bestPlace.empty()) {
					C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
					C_Inventory* inv = supplies->inventory;
					int slotOBBY = supplies->selectedHotbarSlot;
					if (switchType.getSelectedValue() == 2) {
						for (int n = 0; n < 9; n++) {
							C_ItemStack* stack = inv->getItemStack(n);
							if (stack->item != NULL && (*stack->item)->itemId == 49) {
								slotOBBY = n;
							}
							supplies->selectedHotbarSlot = slotOBBY;
						}
					}
					for (auto place : bestPlace) {
						vec3_t entPos = g_Data.getLocalPlayer()->getPos()->floor();
						entPos.y -= 1;
						if (targetList71.empty()) return;
						gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
					}
				}
				if (switchType.getSelectedValue() == 2) {
					g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = prev2;
				}
			}
		}
	}
}
void BedAuraRewrite::onDisable() {
	bedDelay1 = 0;
	hasPlaced1 = false;
}
vec3_t prevRenderPos1;
void BedAuraRewrite::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	C_LocalPlayer* localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr && GameData::canUseMoveKeys())
	{
		if (!targetList71.empty())
		{
			if (!placeArr2.empty())
			{
				for (auto postt : placeArr2)
				{
					// Vectors
					vec3_t block = postt->floor().add(0.5, 0.5, 0.5);
					// Variables
					auto interfaceColor = ColorUtil::interfaceColor(1);
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
					postt->floor() = prevRenderPos1 + (postt->floor() - prevRenderPos1) * 0.3f;
					if (render == 2 || render == 0)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox(postt->floor().add(0, 1.f, 0), vec3_t(floor(postt->x) + 2.f, floor(postt->y) + 1.5f, floor(postt->z) + 1.f), 0.5f, false, 1);
					}
					// Fill
					if (render == 0 || render == 1)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, opacity1);
						DrawUtils::drawBox(postt->floor().add(0, 1.f, 0), vec3_t(floor(postt->x) + 2.f, floor(postt->y) + 1.5f, floor(postt->z) + 1.f), 0.f, true, 1);
					}
					// 2D
					if (render == 3)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox(postt->floor().add(0, 0.999f, 0), vec3_t(floor(postt->x) + 2.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 0.5f, false, 1);
					}
					prevRenderPos1 = postt->floor();
					placeArr1.clear();
				}
			}
		}
	}
}
void BedAuraRewrite::onSendPacket(C_Packet* pkt)
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
			vec2_t anglefd = g_Data.getLocalPlayer()->getPos()->CalcAngle(targetList71[0]->eyePos0);
			packet->headYaw = anglefd.y;
			packet->yaw = anglefd.y;
		}
	}
}