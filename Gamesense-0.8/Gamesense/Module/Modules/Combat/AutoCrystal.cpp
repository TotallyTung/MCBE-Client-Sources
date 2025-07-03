#include "AutoCrystal.h"
#include "../../../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
static bool mobs = false;
static uintptr_t HiveRotations1 = Utils::getBase() + 0x8F3895;
static uintptr_t HiveRotations2 = Utils::getBase() + 0x8F87C7;
AutoCrystal::AutoCrystal() : IModule(0, Category::COMBAT, "Automatically places and breaks crystals. by Tony") {

	// Main
	registerBoolSetting("Place", &placeCrystals, placeCrystals, "Places end crystals");
	registerBoolSetting("Break", &breakCrystals, breakCrystals, "Breaks end crystals");
	protocol = SettingEnum(this)
		.addEntry(EnumEntry("Nukkit", 0))
		.addEntry(EnumEntry("Vanilla", 1))
	    .addEntry(EnumEntry("Java", 2));
	registerEnumSetting("Protocol", &protocol, 0, "Control the clients placements and calculations");
	registerFloatSetting("Self Damage", &maxSelfDmg, maxSelfDmg, 0.f, 20.f);
	// Delays
	registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 25, "Delay of each Place interval");
	registerIntSetting("Break Delay", &breakDelay, breakDelay, 0, 25, "Delay of each break interval");
	// Ranges
	registerIntSetting("Proximity", &maxProximity, maxProximity, 1, 8, "The maximum distance can a crystal be placed from a person before switching axis");
	registerFloatSetting("Enemy Range", &targetRange, targetRange, 5, 15, 0.1f, "The distance we need to detect a valid target");
	registerFloatSetting("Place Range", &placeRange, placeRange, 0.f, 12.f, 0.1f, "The distance we need from the target to place end crystals");
	registerFloatSetting("Break Range", &breakRange, breakRange, 0.f, 12.f, 0.1f, "The distance we need from the target to break end crystals");
	registerFloatSetting("Wall Range", &thruWallsR, thruWallsR, 0, 10, 0.1f, "Range to place end crystals through walls");
	//registerFloatSetting("Post Wall Range", &postWallsR, postWallsR, 0.f, 10.f);
	// Calculations
	registerFloatSetting("Enemy Damage", &minEnemDmg, minEnemDmg, 0.f, 20.f, 0.1f, "Minimum damage to deal to enemy, if crystal damage is less than this then it will find a new placement");
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Health", 1));
	registerEnumSetting("Sort", &priority, 0, "Decides how targets are sorted");
	mode = SettingEnum(this)
		.addEntry(EnumEntry("Single", 0))
		.addEntry(EnumEntry("Multi", 1));
	registerEnumSetting("Mode", &mode, 0, "How many targets will we attack");
	// Safety
	registerBoolSetting("Safety", &safetyFirst, safetyFirst, "Prioritize safety over damage");
	registerBoolSetting("Ignore Eat", &ignoreEat, ignoreEat, "Autocrystals anyways even if eating");
	registerFloatSetting("Minimum Health", &minHealth, minHealth, 0.f, 20.f, 0.1f, "If health is less than this then we stop placing");
	// Swap
	switchType = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Regular", 1))
		.addEntry(EnumEntry("Silent", 2));
	registerEnumSetting("Swap", &switchType, 0, "How we will swap to end crystals");
	// Faceplace
	registerFloatSetting("Faceplace Health", &fpThresh, fpThresh, 0.f, 20.f, 0.1f, "Minimum health to faceplace, set to 0 for false");
	registerFloatSetting("Faceplace Damage", &dmgAtThresh, dmgAtThresh, 0.f, 20.f, 0.1f, "Minimum damage of faceplace");
	// Render
	renderType = SettingEnum(this)
		.addEntry(EnumEntry("Normal", 0))
		.addEntry(EnumEntry("Fill", 1))
		.addEntry(EnumEntry("Box", 2))
		.addEntry(EnumEntry("2D", 3))
		.addEntry(EnumEntry("None", 4));
	registerEnumSetting("Render", &renderType, 0);
	registerFloatSetting("Alpha", &alpha, alpha, 0.f, 1.f);

}

AutoCrystal::~AutoCrystal() {};

const char* AutoCrystal::getRawModuleName() {
	return ("AutoCrystal");
}
const char* AutoCrystal::getModName()
{
	switch (protocol.getSelectedValue())
	{
	case 0:
		name = " Nukkit";
		break;
	case 1:
		name = " Vanilla";
		break;
	case 2:
		name = " Java";
		break;
	}
	return name;
}
static int slotCA = 0;

/*
int findRelativeAngle(vec3_t playerVec, vec3_t enemyVec) {

	vec3_t deltaVec = playerVec.floor().sub(enemyVec.floor());

	if (deltaVec == vec3_t(0, 0, 0))
		return -1;
	if (deltaVec.x == 0 && deltaVec.z > 0)
		return 0;
	if (deltaVec.x > 0 && deltaVec.z == 0)
		return 90;
	if (deltaVec.x == 0 && deltaVec.z < 0)
		return 180;
	if (deltaVec.x < 0 && deltaVec.z == 0)
		return 270;

	float degreeOffset = atan2(deltaVec.x, deltaVec.z);
	degreeOffset = degreeOffset * (180 / PI);

	if ((deltaVec.x > 0 && deltaVec.z > 0) || (deltaVec.x > 0 && deltaVec.z < 0))
		return degreeOffset;
	else if ((deltaVec.x < 0 && deltaVec.z < 0) || (deltaVec.x < 0 && deltaVec.z > 0))
		return degreeOffset + 360;
}
*/

static std::vector<C_Entity*> tgtList;
static bool locateEntCrPl(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (curEnt == g_Data.getLocalPlayer()) return false;
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;
	if (curEnt->getEntityTypeId() == 66) return false;
	if (curEnt->getEntityTypeId() == 64) return false;
	if (curEnt->getEntityTypeId() == 69) return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;
	if (!TargetUtil::isValidTarget(curEnt)) return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<AutoCrystal>()->targetRange) {
		tgtList.push_back(curEnt);
		return true;
	}
	return false;
}

static std::vector<C_Entity*> entLocator120;
static bool processEntForentLocator120(C_Entity* curEnt, bool isRegularEntity) {
	if (curEnt == nullptr) return false;
	if (!curEnt->isAlive()) return false;
	if (!g_Data.getLocalPlayer()->isAlive()) return false;
	if (curEnt->getEntityTypeId() == 71) return false;
	if (curEnt->getEntityTypeId() == 69) return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f) return false;
	//if (curEnt->getEntityTypeId() != 307) // NPC
		//return false;
	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<AutoCrystal>()->targetRange) {
		entLocator120.push_back(curEnt);
		return true;
	}
	return false;
}
void regularCrystalSwap() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory; // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 637)
			{ // select crystal
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void silentCrystalSwap() {
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 637)
			{
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
void AutoCrystal::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;

	Memory.clear();
	tgtList.clear();
	entLocator120.clear();
	return;
}

bool AutoCrystal::isPlaceValid(vec3_t location, C_Entity* atkObj) {
	auto* localPlayer = g_Data.getLocalPlayer();
	auto* region = localPlayer->region;
	auto& aabb = localPlayer->aabb;

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(
		FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));

	static std::unordered_map<vec3_ti, int, Vec3Hash> cachedBlockTypes;

	auto getCachedBlockType = [&](const vec3_ti& pos) -> int {
		auto it = cachedBlockTypes.find(pos);
		if (it != cachedBlockTypes.end()) {
			return it->second;
		}

		int blockType = region->getBlock(pos)->toLegacy()->blockId;
		cachedBlockTypes[pos] = blockType;
		return blockType;
	};

	auto blockType = getCachedBlockType(location);
	if (blockType != 0)
		return false;

	if (protocol.getSelectedValue() != 2) {
		auto upperPos = location.add(0, 1, 0);
		auto upperBlockType = getCachedBlockType(upperPos);
		if (upperBlockType != 0)
			return false;

		if (checkCornerHitboxCollision(upperPos, atkObj))
			return false;
	}

	if (checkCornerHitboxCollision(location, atkObj))
		return false;

	auto lowerPos = location.sub(0, 1, 0);
	auto lowerBlockType = getCachedBlockType(lowerPos);
	if (lowerBlockType != 49 && lowerBlockType != 7)
		return false;

	float distToLoc = location.dist(localPlayer->getHumanPos());
	if (distToLoc > placeRange)
		return false;

	if (localPlayer->getHealth() < minHealth)
		return false;

	float possibleDmg = computeExplosionDamage(location, localPlayer, region);
	if (possibleDmg > maxSelfDmg && maxSelfDmg != 20.0f)
		return false;

	if (atkObj->getHumanPos().floor().y >= location.y) {
		float targetSeenPercent = getSeenPercent(atkObj->region, location.add(0.5f, 0.0f, 0.5f), atkObj->aabb);
		float enemyDmg = computeExplosionDamage(location, atkObj, atkObj->region);

		if (enemyDmg < minEnemDmg && minEnemDmg != 0.0f && targetSeenPercent == 0.0f)
			return false;
	}

	

	g_Data.forEachEntity(processEntForentLocator120);
	for (auto i : entLocator120) {
		if (i == atkObj)
			continue;
		if (i->getHumanPos().floor() == location)
			return false;
	}

	if (localPlayer->getHumanPos().floor() == location)
		return false;

	return true;
}

#include <unordered_map>
float AutoCrystal::computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg) {
	constexpr float explosionRadius = 12.0f;
	float dist = target->getHumanPos().dist(crystalPos) / explosionRadius;

	if (dist > 1.0f)
		return 0.0f;

	float armorPoints = 0.0f;
	float epf = 0.0f;

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(
		FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));

	static std::unordered_map<int, int> armorMapping = {
		{573, 2}, {335, 1}, {336, 3}, {337, 2}, {338, 1}, {351, 2}, {352, 5}, {353, 3}, {354, 1},
		{339, 2}, {340, 5}, {341, 4}, {342, 1}, {343, 2}, {344, 6}, {345, 5}, {346, 2}, {347, 3},
		{609, 3}, {350, 3}, {612, 3}, {348, 8}, {610, 8}, {349, 6}, {611, 6}
	};

	if (protocol.GetSelectedEntry().GetValue() == 0 || protocol.GetSelectedEntry().GetValue() == 2) {
		float toughness = 0.0f;
		float distedSize = target->getHumanPos().dist(crystalPos) / explosionRadius;
		double blockDensity = getSeenPercent(reg, crystalPos.add(0.5f, 0.0f, 0.5f), target->aabb);
		double v = (1.0 - distedSize) * blockDensity;

		float damage = static_cast<float>(static_cast<int>((v * v + v) / 2.0 * 7.0 * explosionRadius + 1.0));
		damage = damage * 3.0 / 2.0;

		bool hasArmor = false;
		for (int i = 0; i < 4; ++i) {
			C_ItemStack* armor = target->getArmor(i);

			if (armor->item == nullptr)
				continue;

			int armorId = armor->getItem()->itemId;
			armorPoints += armorMapping[armorId];
			hasArmor = true;
			epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
		}

		if (hasArmor) {
			float F_epf = fminf(static_cast<float>(epf), 15.0f) / 25.0f;
			damage = damage * fminf(armorPoints / 5.0f, 1.0f / (armorPoints / 5.0f));
			damage = damage * (1 - F_epf);
		}
		return damage;
	}
	else if (protocol.GetSelectedEntry().GetValue() == 1) {
		float exposure = getSeenPercent(reg, crystalPos.add(0.5f, 0.0f, 0.5f), target->aabb);
		float impact = (1 - dist) * exposure;
		float damage = (impact * impact * 3.5f + impact * 0.5f * 7.0f) * explosionRadius + 1.0f;

		for (int i = 0; i < 4; ++i) {
			C_ItemStack* armor = target->getArmor(i);

			if (armor->item == nullptr)
				continue;

			armorPoints += armor->getItem()->getArmorValue();
			epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
		}

		damage -= damage * armorPoints * 0.04f;
		damage -= damage * std::min(ceilf(std::min(epf, 25.0f) * 75.0f / 100.0f), 20.0f) * 0.04f;
		return std::max(damage, 0.0f);
	}

	return 0.0f;
}

float AutoCrystal::getBlastDamageEnchantReduction(C_ItemStack* armor) {
	float epf = 0.0f;

	if (protocol.GetSelectedEntry().GetValue() == 0 || protocol.GetSelectedEntry().GetValue() == 2 || protocol.GetSelectedEntry().GetValue() == 1) {
		epf += (armor->getEnchantValue(0) != 4) ? armor->getEnchantValue(0) : 5.0f;
		epf += armor->getEnchantValue(3) * 2.0f;
	}

	return epf;
}

#include <unordered_set>
static bool cmpAgain(CrystalPlacement E1, CrystalPlacement E2) {
	return E1.enemyDmg > E2.enemyDmg;
}
std::vector<CrystalPlacement> AutoCrystal::ValidPlacement(C_Entity* target, int yOffset) {
	vec3_t targetPos = target->getHumanPos().floor().add(0, static_cast<float>(yOffset), 0);

	std::vector<CrystalPlacement> validPlacements;

	// Define the range of positions to consider
	const int searchRange = 4;
#pragma omp parallel for
	for (int x = -searchRange; x <= searchRange; x++) {
		for (int z = -searchRange; z <= searchRange; z++) {
			for (int y = -searchRange; y <= searchRange; y++) {
				vec3_t search = targetPos.add(x, y, z);

				// Check if the position is within a reasonable distance
				if (search.dist(target->getHumanPos()) < maxProximity && targetPos.floor() != search) {
					// Check if the position is valid
					if (isPlaceValid(search, target)) {
						CrystalPlacement placement;
						placement.toPlace = search;
						placement.enemyDmg = computeExplosionDamage(search, target, target->region);
						placement.selfDmg = computeExplosionDamage(search, g_Data.getLocalPlayer(), g_Data.getLocalPlayer()->region);
#pragma omp critical
						{
							validPlacements.push_back(placement);
						}
					}
				}
			}
		}
	}

	// Sort placements based on damage
	std::sort(validPlacements.begin(), validPlacements.end(), cmpAgain);

	return validPlacements;
}

static bool cmpPlacements(CrystalPlacement E1, CrystalPlacement E2) {
	bool cmpType = moduleMgr->getModule<AutoCrystal>()->safetyFirst;

	if (!cmpType) {
		return E1.enemyDmg > E2.enemyDmg;
	}

	if (cmpType) {
		return E1.selfDmg < E2.selfDmg;
	}
}

static bool cmpDup(CrystalPlacement E1, CrystalPlacement E2) {
	bool cmpType = not(moduleMgr->getModule<AutoCrystal>()->safetyFirst);

	if (!cmpType) {
		return E1.enemyDmg > E2.enemyDmg;
	}

	if (cmpType) {
		return E1.selfDmg < E2.selfDmg;
	}
}
static float getVisibility(const vec3_t& position, const AABB& targetAABB) {
	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(
		FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));
	vec3_t pos = vec3_t(0.5f, 0.f, 0.5f);
	return getSeenPercent(g_Data.getLocalPlayer()->region, position.add(pos), targetAABB);
}

CrystalPlacement AutoCrystal::BestPlacement(C_Entity* targ, int yLevel) {
	std::vector<CrystalPlacement> validPlacements = ValidPlacement(targ, yLevel);

	if (validPlacements.empty()) {
		CrystalPlacement emptyPlacement;
		emptyPlacement.enemyDmg = -42069;
		return emptyPlacement;
	}

	auto cmpCustom = [targ](const CrystalPlacement& E1, const CrystalPlacement& E2) {
		float distanceE1 = E1.toPlace.distanceTo(*targ->getPos());
		float distanceE2 = E2.toPlace.distanceTo(*targ->getPos());

		float visibility1 = getVisibility(E1.toPlace, targ->aabb);
		float visibility2 = getVisibility(E2.toPlace, targ->aabb);

		if (visibility1 != visibility2)
			return visibility1 > visibility2;

		if (distanceE1 != distanceE2)
			return distanceE1 < distanceE2;

		return E1.enemyDmg > E2.enemyDmg;
	};

	std::sort(validPlacements.begin(), validPlacements.end(), cmpCustom);

	float highestDamage = 0.0f;
	std::vector<CrystalPlacement> duplicates;

	if (safetyFirst) {
		for (const CrystalPlacement& placement : validPlacements) {
			highestDamage = placement.selfDmg;
			if (placement.selfDmg == highestDamage) {
				duplicates.push_back(placement);
			}
			else {
				break;
			}
		}
	}
	else {
		for (const CrystalPlacement& placement : validPlacements) {
			highestDamage = placement.enemyDmg;
			if (placement.enemyDmg == highestDamage) {
				duplicates.push_back(placement);
			}
			else {
				break;
			}
		}
	}

	if (duplicates.size() == 1)
		return duplicates[0];

	std::sort(duplicates.begin(), duplicates.end(), cmpDup);

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(
		FindSignature("40 53 55 41 56 48 81 ec ? ? ? ? 48 8b 05 ? ? ? ? 48 33 c4 48 89 84 24"));

	for (const CrystalPlacement& duplicate : duplicates) {
		vec3_t block = duplicate.toPlace;

		float exposure = getSeenPercent(g_Data.getLocalPlayer()->region, block.add(0.5f, 0.f, 0.5f), targ->aabb);

		if (exposure > 0.5f && !checkCornerHitboxCollision(block, targ)) {
			return duplicate;
		}
	}

	return duplicates[0];
}

#include <algorithm>


CrystalPlacement AutoCrystal::Main(C_Entity* target) {
	const int maxVOff = 3;
	const int minVOff = -5;

	struct CandidatePlacement {
		CrystalPlacement placement;
		float distance;
		float visibility;
	};

	std::vector<CandidatePlacement> candidates;

	auto calculatePlacement = [this](C_Entity* target, int vOffset) -> CandidatePlacement {
		CrystalPlacement placement = BestPlacement(target, vOffset);

		if (placement.enemyDmg != -42069) {
			float distance = placement.toPlace.distanceTo(*target->getPos());
			float visibility = getVisibility(placement.toPlace, target->aabb);
			return { placement, distance, visibility };
		}
		else {
			return {};
		}
	};

#pragma omp parallel for reduction(+:candidates)
	for (int vOffset = minVOff; vOffset < maxVOff; ++vOffset) {
		CandidatePlacement candidate = calculatePlacement(target, vOffset);

#pragma omp critical
		{
			if (candidate.placement.enemyDmg != -42069) {
				candidates.push_back(candidate);
			}
		}
	}

	if (candidates.empty()) {
		return {};
	}

	std::sort(candidates.begin(), candidates.end(), [](const CandidatePlacement& c1, const CandidatePlacement& c2) {
		if (c1.visibility != c2.visibility) {
			return c1.visibility > c2.visibility;
		}

		if (c1.distance != c2.distance) {
			return c1.distance < c2.distance;
		}

		return c1.placement.enemyDmg > c2.placement.enemyDmg;
		});

	return candidates[0].placement;
}


static bool compareTargListVec(C_Entity* E1, C_Entity* E2) {
	int whatToCompare = moduleMgr->getModule<AutoCrystal>()->priority.GetSelectedEntry().GetValue();

	if (whatToCompare == 0) {
		return g_Data.getLocalPlayer()->getHumanPos().dist(E1->getHumanPos()) < g_Data.getLocalPlayer()->getHumanPos().dist(E2->getHumanPos());
	}
	else if (whatToCompare == 1) {
		return E1->getHealth() < E2->getHealth();
	}
}
static int switchSlot;
static bool grabNow = false;
static bool hasRotten = false;
static bool packetSent = false;
static bool isPlayerAuthInput = false;
static vec3_t postorot;
#include "../pch.h"
#include "../../../../Utils/Logger.h"

void placeCrystal(const vec3_t& position) {
	g_Data.getCGameMode()->buildBlock(&vec3_ti(position).sub(0, 1, 0), 0);
	g_Data.getCGameMode()->continueBuildBlock(vec3_ti(position).sub(0, 1, 0), 0);
}
bool hasPlaced = false;
void AutoCrystal::breakCrystal() {
	if (breakCrystals)
	{
		g_Data.forEachEntity([](C_Entity* ent, bool b)
			{
				int id = ent->getEntityTypeId();
				int Brange = moduleMgr->getModule<AutoCrystal>()->breakRange;
				auto playerHealth = g_Data.getLocalPlayer()->getHealth();
				auto minBreak = moduleMgr->getModule<AutoCrystal>()->minHealth;
				auto safety = moduleMgr->getModule<AutoCrystal>()->safetyFirst;
				if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= Brange) {
					if (!tgtList.empty()) {
						if (safety && minBreak < playerHealth) {
							g_Data.getCGameMode()->attack(ent);
								g_Data.getLocalPlayer()->swingArm();
						}
						else {
							g_Data.getCGameMode()->attack(ent);
								g_Data.getLocalPlayer()->swingArm();
						}
					}
				}
			}
		);
	}
	return;
}
void AutoCrystal::onTick(C_GameMode* gm) {
	hasRotten = false;
	rotUpNow = true;
	packetSent = false;
	isPlayerAuthInput = false;
	auto player = g_Data.getLocalPlayer();

	placeArrEmpty = Memory.empty();
	entLocator120.clear();
	tgtList.clear();
	Memory.clear();

	g_Data.forEachEntity(locateEntCrPl);

	if (tgtList.empty()) return;
	if (!ignoreEat)
	{
		if (player->getSelectedItemId() == 259 || player->getSelectedItemId() == 300 || player->getSelectedItemId() == 426 && g_Data.isRightClickDown())
			return;
	}

	std::sort(tgtList.begin(), tgtList.end(), compareTargListVec);

	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	slotCA = supplies->selectedHotbarSlot;
	C_ItemStack* item = supplies->inventory->getItemStack(0);

	if (mode.selected == 0) {
		if (!tgtList.empty()) {

			CrystalPlacement placeInfo = Main(tgtList[0]);
			if (placeInfo.enemyDmg != -42069) {
				CrystalInfomation a2vec;
				a2vec.CPlacements = placeInfo;
				a2vec.ent = tgtList[0];
				Memory.push_back(a2vec);
			}
		}
	}
	else if (mode.selected == 1) {
		for (auto& enemy : tgtList) {

			CrystalPlacement placeInfo = Main(enemy);
			if (placeInfo.enemyDmg != -42069) {
				CrystalInfomation a2vec;
				a2vec.CPlacements = placeInfo;
				a2vec.ent = enemy;
				Memory.push_back(a2vec);
			}
		}
	}

	ctr++;
	increasingBreak++;
	if (increasingBreak >= breakDelay) {
		breakCrystal();
		increasingBreak = 0;
	}
	if (ctr >= placeDelay) {
		if (switchType.selected == 1 && g_Data.isRightClickDown()) {
			origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
			regularCrystalSwap();
		}
		if (switchType.selected == 2)
			silentCrystalSwap();
		for (CrystalInfomation& Place : Memory)
		{
			if (!hasPlaced)
			{
				vec3_t placeMe = Place.CPlacements.toPlace;
				if (placeCrystals)
				placeCrystal(placeMe);
			}
		}
		ctr = 0;
		if (switchType.selected == 2)
		supplies->selectedHotbarSlot = slotCA;
	}
}

# define M_PI 3.14159265358979323846 /* pi */

vec2_t GetRotations(vec3_t playerEyePos, vec3_t* targetPos) {
	vec3_t delta = *targetPos - playerEyePos;
	float yaw = atan2(delta.z, delta.x) * 180.0f / M_PI;
	float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / M_PI;
	return { -pitch, yaw - 90 };
}
std::vector<CrystalInfomation> CJTWDPlaceArr1;
void AutoCrystal::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) return;

	for (auto& i : tgtList) {
		CrystalPlacement placeInfo1 = Main(i);
		if (placeInfo1.enemyDmg != -42069) {
			CrystalInfomation a2vec1;
			a2vec1.CPlacements = placeInfo1;
			a2vec1.ent = i;
			CJTWDPlaceArr1.push_back(a2vec1);
			for (CrystalInfomation& Place1 : CJTWDPlaceArr1)
			{
				vec3_t placeMe = Place1.CPlacements.toPlace;
				auto rot = GetRotations(plr->currentPos, &placeMe);
				plr->yawUnused1 = rot.y;
				plr->pitch = rot.x;
				plr->bodyYaw = rot.y;

			}
		}
	}
}
void AutoCrystal::onWorldTick(C_GameMode* gm)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (switchType.GetSelectedEntry().GetValue() == 1 && g_Data.isRightClickDown())
	{
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
		return;
	}
	return;
}
void AutoCrystal::onDisable() {
	if (switchType.GetSelectedEntry().GetValue() == 1)
	{
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
		return;
	}
	Utils::patchBytes((unsigned char*)HiveRotations1, (unsigned char*)"\x89\x41\x18", 3);
	Utils::patchBytes((unsigned char*)HiveRotations2, (unsigned char*)"\xC7\x40\x18\x03\x00\x00\x00", 7);
}

static float roundoff(float value, unsigned char prec) {
	float pow_10 = pow(10.0f, (float)prec);
	return round(value * pow_10) / pow_10;
}

static std::string chopOffDigits(std::string STUFF, int digits) {
	bool afterDecimalPt = false;
	int dp = 0;
	std::string toOutput;

	for (int i = 0; i < STUFF.length(); ++i) {
		if (STUFF.at(i) == '.') {
			afterDecimalPt = true;
		}

		toOutput.append(std::string(1, STUFF.at(i)));

		if (afterDecimalPt) {
			dp++;

			if (dp > digits)
				break;
		}
	}

	return toOutput;
}
vec3_t prevRenderPos2;

void AutoCrystal::onPreRender(C_MinecraftUIRenderContext* renderCtx) {
	if (g_Data.getLocalPlayer() == nullptr) return;


	if (!g_Data.canUseMoveKeys() || tgtList.empty() || !g_Data.isInGame() || tgtList[0] == nullptr || tgtList.empty() || tgtList[0] == nullptr || !g_Data.getLocalPlayer()->isAlive()) {
		tgtList.clear();
		return;
	}
	if (Memory.empty()) return;

	if (renderType.getSelectedValue() != 4) {
		for (CrystalInfomation& indivCrystal : Memory) {
			if (Memory.empty() || tgtList.empty()) return;
			vec3_t placeLoc = indivCrystal.CPlacements.toPlace;
			placeLoc = prevRenderPos2 + (placeLoc - prevRenderPos2) * 0.3f;
			auto interfaceColor = ColorUtil::interfaceColor(1);
			float enemyDmg = indivCrystal.CPlacements.enemyDmg;
			if (renderType.getSelectedValue() == 0 || renderType.getSelectedValue() == 2) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, alpha + 0.2f);
				DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), .4f, false);
			}
			if (renderType.getSelectedValue() == 0 || renderType.getSelectedValue() == 1) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, alpha);
				DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), 0.f, true);
			}
			if (renderType.getSelectedValue() == 3) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, alpha + 0.2f);
				DrawUtils::drawBox(placeLoc.sub(0, 0.01, 0), placeLoc.add(1, 0, 1), .4f, false);

				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, alpha);
				DrawUtils::drawBox(placeLoc.sub(0, 0.01, 0), placeLoc.add(1, 0, 1), 0.f, true);
			}
			vec3_t posplace = indivCrystal.ent->getHumanPos().floor();

			std::string enem_dmg = std::to_string(roundoff(enemyDmg, 2));
			enem_dmg = chopOffDigits(enem_dmg, 2);
			if (renderText)
			DrawUtils::drawTextInWorld(&enem_dmg, placeLoc.sub(0, 1.2f, 0), 0.6f, vec3_ti(180, 180, 180), vec3_ti(12, 12, 12), .1f);

			prevRenderPos2 = placeLoc;
		}
	}

	return;
}
std::vector<CrystalInfomation> CJTWDPlaceArr11;
void AutoCrystal::onSendPacket(C_Packet* packet) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (!tgtList.empty()) {

		if (packet->isInstanceOf<C_MovePlayerPacket>() && !tgtList.empty()) {

			for (auto& i : tgtList) {
				CrystalPlacement placeInfo11 = Main(i);
				if (placeInfo11.enemyDmg != -42069) {
					CrystalInfomation a2vec11;
					auto* movePacket = reinterpret_cast<C_MovePlayerPacket*>(packet);
					a2vec11.CPlacements = placeInfo11;
					a2vec11.ent = i;
					CJTWDPlaceArr11.push_back(a2vec11);
					for (CrystalInfomation& Place1 : CJTWDPlaceArr11)
					{
						vec3_t placeMe = Place1.CPlacements.toPlace;
						auto rot = GetRotations(player->currentPos, &placeMe);
						movePacket->pitch = rot.x;
						movePacket->headYaw = rot.y;
						movePacket->yaw = rot.y;

					}
				}
			}
		}
	}
}