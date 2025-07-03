#define NOMINMAX // fixes da std::min eror
#include "AutoCrystalRewrite.h"
#include "../../Utils/TargetUtil.h"
#include "../../ModuleManager.h"
#include <chrono>
#include <vector>
#include <thread>
SettingEnum logicType;
SettingEnum swing;
bool doPlace = true;
float increaseDelay = 0;
float incrDel = 0;
float lerpSpeed = 0.3f;
using namespace std;
bool phaseEnabled = false;
bool silentSwap;
bool swapEC;
AutoCrystalRewrite::AutoCrystalRewrite() : IModule(0, Category::COMBAT, "Automatically places and breaks end crystals.")
{
	// Place
	protocol = SettingEnum(this)
		.addEntry(EnumEntry("Nukkit", 0)) // Originally java but nukkit makes more sense as most geyser java servers are 1.13+ so we use the 1.13 placement as the java protocol.
		.addEntry(EnumEntry("Vanilla", 1))
		.addEntry(EnumEntry("Java", 2))
	    .addEntry(EnumEntry("Multi", 3));
	registerEnumSetting("Placement", &this->protocol, 0, "Control the clients placements and calculations");
	registerBoolSetting("Place", &doPlace, doPlace, "Place end crystals");

	logicType = SettingEnum(this)
		.addEntry(EnumEntry("Place/Break", 0))
		.addEntry(EnumEntry("Break/Place", 1));
	registerEnumSetting("Logic", &logicType, 0, "What should the client place and break logic should be. This may vary on Ping");
	registerBoolSetting("Synced", &this->synced, this->synced, "Make placing and breaking synced. this may slow down crystalaura speed by a little bit but it will make it more consistent");
	registerIntSetting("Place Delay", &this->delay, this->delay, 0, 25, "The delay of each placements");
	registerIntSetting("Proximity", &this->maxProximity, this->maxProximity, 1, 8, "The maximum distance can a crystal be placed from a person before switching axis");
	//registerIntSetting("Waste Amount", &this->amount, 0, 0, 5, "How many crystals we should waste per place");
	registerFloatSetting("Place Range", &this->placeRange, this->placeRange, 0.f, 12.f, 0.1f, "Range to place end crystals");
	registerFloatSetting("Wall Range", &this->thruWallsR, this->thruWallsR, 0, 10, 0.1f, "Range to place end crystals through walls");
	//registerFloatSetting("Post Wall Range", &this->postWallsR, this->postWallsR, 0.f, 10.f, 0.1f, "Maximum distance to place after going through a wall");

	// Break
	registerBoolSetting("Break", &this->shouldBreak, this->shouldBreak, "Break end crystals");
	registerIntSetting("Break Delay", &this->breakDelay, this->breakDelay, 0, 25, "The delay of each break attemps");
	registerFloatSetting("Break Range", &breakRange, breakRange, 1.f, 10.f, 0.1f, "Minimum range to place end crystal");
	
	// Target
	priority = SettingEnum(this)
		.addEntry(EnumEntry("Distance", 0))
		.addEntry(EnumEntry("Health", 1));
	registerEnumSetting("Priority", &this->priority, 0, "Decides how targets are sorted");
	registerBoolSetting("Multiple Enemy", &this->attackMulti, this->attackMulti, "Attack more than one enemy");
	registerFloatSetting("Enemy Range", &this->range, this->range, 5, 15, 0.f, "Range for enemies to be added to the targetlist");
	registerFloatSetting("Enemy Damage", &this->minEnemDmg, this->minEnemDmg, 0.f, 20.f, 0.1f, "Minimum damage to deal to an enemy");

	// Misc
	registerBoolSetting("Safety", &this->safetyFirst, this->safetyFirst, "Will prioritize safety over damage");
	registerBoolSetting("Ignore Eat", &ignoreEat, ignoreEat, "Will place anyways even if eating");
	rotations = SettingEnum(this)
		.addEntry(EnumEntry("None", 0))
		.addEntry(EnumEntry("Full", 1))
		.addEntry(EnumEntry("Packet", 2));
	registerEnumSetting("Rotations", &this->rotations, 0, "Controls the clients rotations");
	rotationsMode = SettingEnum(this)
		.addEntry(EnumEntry("Always", 0))
		.addEntry(EnumEntry("Phased", 1));
	registerEnumSetting("Yaw Call", &this->rotationsMode, 0, "Controls the clients rotations");
	registerFloatSetting("Place Health", &this->minHealth, this->minHealth, 0.f, 20.f, 0.01f, "If health is lesser than this then autocrystal will stop");
	registerFloatSetting("Break Health", &this->breakHealth, this->breakHealth, 0.f, 20.f, 0.1f, "If health is less than this then the client will stop breaking");
	registerFloatSetting("Self Damage", &this->maxSelfDmg, this->maxSelfDmg, 0.f, 20.f, 0.1f, "If the end crystal will deal more damage to u than this it will search for a new block");

	registerBoolSetting("Switch", &swapEC, swapEC, "Swap to end crystals");
	registerBoolSetting("SwitchBack", &silentSwap, silentSwap, "Switch back to end crystal making it silent");
	swing = SettingEnum(this)
		.addEntry(EnumEntry("Post", 0))
		.addEntry(EnumEntry("Pre", 1));
	registerEnumSetting("Swing", &swing, 0, "Control client swings");

	facePlaceType = SettingEnum(this)
		.addEntry(EnumEntry("True", 0))
		.addEntry(EnumEntry("False", 1));
	registerEnumSetting("Faceplace", &this->facePlaceType, 0, "Should place on their faces");
	registerFloatSetting("FP Health", &this->fpThresh, this->fpThresh, 0.f, 20.f, 0.1f, "Minimum health the target needs to have to faceplace, set to 20 to always faceplace");
	registerFloatSetting("FP Damage", &this->dmgAtThresh, this->dmgAtThresh, 0.f, 20.f, 0.1f, "Minimum faceplace damage");

	// Render
	renderType = SettingEnum(this)
		.addEntry(EnumEntry("Both", 0))
		.addEntry(EnumEntry("Fill", 1))
		.addEntry(EnumEntry("Box", 2))
		.addEntry(EnumEntry("2D", 3))
		.addEntry(EnumEntry("None", 4));
	registerEnumSetting("Render", &this->renderType, 0, "Controls how the client will render");
	registerFloatSetting("Alpha", &this->Opacity, 0.1f, 0.f, 1.f, 0.1f, "Opacity of the fill renders");
	registerFloatSetting("Slide Speed", &lerpSpeed, lerpSpeed, 0.f, 1.f, 0.1f, "Speed of slide");
	registerBoolSetting("Render Damage", &this->drawEnemDmg, this->drawEnemDmg, "Calculate based on the Protocol option and shows how much damage you will deal to an enemy");
}

AutoCrystalRewrite::~AutoCrystalRewrite() {};

const char* AutoCrystalRewrite::getRawModuleName()
{
	return ("AutoCrystal");
}

const char* AutoCrystalRewrite::getModName()
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
	case 3:
		name = " Multi";
		break;
	}
	return name;
}

void findSomeEndCrystals()
{
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

static std::vector<C_Entity*> tgtList;
bool locateEntCrPl(C_Entity* curEnt, bool isRegularEntity)
{
	if (curEnt == nullptr)
		return false;
	if (curEnt == g_Data.getLocalPlayer())
		return false;
	if (!curEnt->isAlive())
		return false;
	if (!g_Data.getLocalPlayer()->isAlive())
		return false;
	if (curEnt->getEntityTypeId() == 71)
		return false;
	if (curEnt->getEntityTypeId() == 66)
		return false;
	if (curEnt->getEntityTypeId() == 64)
		return false;
	if (curEnt->getEntityTypeId() == 69)
		return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f)
		return false;
	if (!TargetUtil::isValidTarget(curEnt))
		return false;

	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<AutoCrystalRewrite>()->range)
	{
		tgtList.push_back(curEnt);
		return true;
	}
	return false;
}

static std::vector<C_Entity*> entLocator120;
bool processEntForentLocator120(C_Entity* curEnt, bool isRegularEntity)
{
	if (curEnt == nullptr)
		return false;
	if (!curEnt->isAlive())
		return false;
	if (!g_Data.getLocalPlayer()->isAlive())
		return false;
	if (curEnt->getEntityTypeId() == 71)
		return false;
	if (curEnt->getEntityTypeId() == 69)
		return false;
	if (curEnt->width <= 0.01f || curEnt->height <= 0.01f)
		return false;
	float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (dist <= moduleMgr->getModule<AutoCrystalRewrite>()->range)
	{
		entLocator120.push_back(curEnt);
		return true;
	}
	return false;
}

void AutoCrystalRewrite::onEnable()
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (swapEC && !silentSwap)
	{ // if switchType == regular
		origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		findSomeEndCrystals();
	}
	RewritePlaceArr.clear();
	tgtList.clear();
	entLocator120.clear();
	return;
}

bool AutoCrystalRewrite::isPlaceValid(vec3_t location, C_Entity* atkObj)
{

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));

	if (getSeenPercent(g_Data.getLocalPlayer()->region, location.add(0.5f, 0.f, 0.5f), g_Data.getLocalPlayer()->aabb) == 0.f &&
		thruWallsR != 10)
	{

		if (thruWallsR == 0)
			return false;

		lineResults lineRes = countBlksAlongLine(g_Data.getLocalPlayer()->eyePos0, location.add(0.5f));
		float amtOfShitInBtwn = lineRes.blockCount;

		if (amtOfShitInBtwn >= thruWallsR)
			return false;

		vec3_t lastKnownSolid = lineRes.lastSolidBlock;
		if (lastKnownSolid.dist(location) > postWallsR)
			return false;
	}

	if (g_Data.getLocalPlayer()->region->getBlock(location)->toLegacy()->blockId != 0)
		return false;
	if (protocol.getSelectedValue() != 2 && g_Data.getLocalPlayer()->region->getBlock(location.add(0, 1, 0))->toLegacy()->blockId != 0)
		return false;
	if (protocol.getSelectedValue() != 2 && checkCornerHitboxCollision2(location.add(0, 1, 0), atkObj))
		return false;
	if (checkCornerHitboxCollision2(location, atkObj))
		return false;
	if (g_Data.getLocalPlayer()->region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 49 &&
		g_Data.getLocalPlayer()->region->getBlock(location.sub(0, 1, 0))->toLegacy()->blockId != 7)
		return false;

	float distToLoc = (location).dist(g_Data.getLocalPlayer()->getHumanPos());
	if (distToLoc > placeRange)
		return false;
	auto friendly = g_Data.getLocalPlayer();

	if (friendly->getHealth() < minHealth && safetyFirst)
		return false;

	float possibleDmg = computeExplosionDamage(location, friendly, friendly->region);
	float enemyDmg = computeExplosionDamage(location, atkObj, atkObj->region);

	if (possibleDmg > maxSelfDmg && safetyFirst)
		return false;

	if (atkObj->getHumanPos().floor().y >= location.y)
		if (enemyDmg < minEnemDmg && minEnemDmg != 0.f)
			return false;

	if ((atkObj->getHumanPos().floor().y + 1) <= location.y && facePlaceType.GetSelectedEntry().GetValue() == 0)
	{
		if ((fpThresh < atkObj->getHealth()) && fpThresh != 20.f)
			return false;
		if (enemyDmg < dmgAtThresh && enemyDmg != 0.f)
			return false;
	}
	else if ((atkObj->getHumanPos().floor().y + 1) <= location.y && facePlaceType.GetSelectedEntry().GetValue() == 1)
		return false;

	g_Data.forEachEntity(processEntForentLocator120);
	for (auto i : entLocator120)
	{
		if (i == atkObj)
			continue;
		if (i->getHumanPos().floor() == location)
			return false;
	}
	if (g_Data.getLocalPlayer()->getHumanPos().floor() == location)
		return false;

	return true;
}
bool checkMultiHitbox(vec3_t* block, C_Entity* ent) {
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
float AutoCrystalRewrite::computeExplosionDamage(vec3_t crystalPos, C_Entity* target, C_BlockSource* reg)
{
	constexpr float explosionRadius = 12.0f;
	float dist = target->getHumanPos().dist(crystalPos) / explosionRadius;

	if (dist > 1.0f)
		return 0.0f;

	using getSeenPercent_t = float(__fastcall*)(C_BlockSource*, vec3_t const&, AABB const&);
	static getSeenPercent_t getSeenPercent = reinterpret_cast<getSeenPercent_t>(
		FindSignature("40 53 55 41 56 48 81 EC ?? ?? ?? ?? 48 8B 05 ?? ?? ?? ?? 48 33 C4"));

	static std::unordered_map<int, int> armorMapping = {
		{573, 2}, {335, 1}, {336, 3}, {337, 2}, {338, 1}, {351, 2}, {352, 5}, {353, 3}, {354, 1},
		{339, 2}, {340, 5}, {341, 4}, {342, 1}, {343, 2}, {344, 6}, {345, 5}, {346, 2}, {347, 3},
		{609, 3}, {350, 3}, {612, 3}, {348, 8}, {610, 8}, {349, 6}, {611, 6}
	};

	float damage = 0.0f;
	if (protocol.GetSelectedEntry().GetValue() == 0 || protocol.GetSelectedEntry().GetValue() == 2 || protocol.GetSelectedEntry().GetValue() == 3) {
		float toughness = 0.0f;
		float distedSize = target->getHumanPos().dist(crystalPos) / explosionRadius;
		double blockDensity = getSeenPercent(reg, crystalPos.add(0.5f, 0.0f, 0.5f), target->aabb);
		double v = (1.0 - distedSize) * blockDensity;

		damage = static_cast<float>(static_cast<int>((v * v + v) / 2.0 * 7.0 * explosionRadius + 1.0));
		damage = damage * 1.5f;

		bool hasArmor = false;
		float armorPoints = 0.0f;
		float epf = 0.0f;

		for (int i = 0; i < 4; ++i) {
			C_ItemStack* armor = target->getArmor(i);

			if (armor->item != nullptr) {
				int armorId = armor->getItem()->itemId;
				armorPoints += armorMapping[armorId];
				hasArmor = true;
				epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
			}
		}

		if (hasArmor) {
			float F_epf = fminf(static_cast<float>(epf), 15.0f) / 25.0f;
			damage = damage * fminf(armorPoints / 5.0f, 1.0f / (armorPoints / 5.0f));
			damage = damage * (1 - F_epf);
		}
	}
	else if (protocol.GetSelectedEntry().GetValue() == 1) {
		float exposure = getSeenPercent(reg, crystalPos.add(0.5f, 0.0f, 0.5f), target->aabb);
		float impact = (1 - dist) * exposure;
		damage = (impact * impact * 3.5f + impact * 0.5f * 7.0f) * explosionRadius + 1.0f;

		float armorPoints = 0.0f;
		float epf = 0.0f;

		for (int i = 0; i < 4; ++i) {
			C_ItemStack* armor = target->getArmor(i);

			if (armor->item != nullptr) {
				armorPoints += armor->getItem()->getArmorValue();
				epf += static_cast<int>(getBlastDamageEnchantReduction(armor));
			}
		}

		damage -= damage * armorPoints * 0.04f;
		damage -= damage * std::min(ceilf(std::min(epf, 25.0f) * 75.0f / 100.0f), 20.0f) * 0.04f;
		damage = std::max(damage, 0.0f);
	}

	return damage;
}

float AutoCrystalRewrite::getBlastDamageEnchantReduction(C_ItemStack* armor)
{
	float epf = 0.f;

	if (protocol.GetSelectedEntry().GetValue() == 0 || protocol.getSelectedValue() == 2 || protocol.GetSelectedEntry().GetValue() == 3)
	{
		if (armor->getEnchantValue(0) != 4)
		{
			epf += armor->getEnchantValue(0);
		}
		else
			epf += 5;
		epf += armor->getEnchantValue(3) * 2.f;
	}

	else if (protocol.GetSelectedEntry().GetValue() == 1)
	{
		epf += armor->getEnchantValue(0);
		epf += armor->getEnchantValue(3) * 2.f;
	}

	return epf;
}

std::vector<CrystalPlacements> AutoCrystalRewrite::generateValidPlacements(C_Entity* target, int yOffset)
{ // damn look at the length of this shit
	vec3_t targetPos = target->getHumanPos().floor();
	targetPos = vec3_t(targetPos.x, targetPos.y + (float)yOffset, targetPos.z);
	int proximity = maxProximity;

	// just search a flat 2d plane for now -> detect valid placements -> filter out the bests ones later on
	std::vector<CrystalPlacements> bunchashit;
	C_Entity* ent;
	for (int x = -proximity; x < proximity; x++)
	{
		for (int z = -proximity; z < proximity; z++)
		{
			for (int y = - 4; z <= + 4; z++)
			{
				vec3_t search = targetPos.add(x, 0, z);
				if (search.dist(target->getHumanPos()) < proximity && target->getHumanPos().floor() != search)
				{
					if (isPlaceValid(search, target))
					{
						CrystalPlacements me;
						me.toPlace = search;
						me.enemyDmg = computeExplosionDamage(search, target, target->region);
						me.selfDmg = computeExplosionDamage(search, g_Data.getLocalPlayer(), g_Data.getLocalPlayer()->region);
						bunchashit.push_back(me);
					}
				}
			}
		}
	}
	return bunchashit;
}

bool cmpPlacements(CrystalPlacements E1, CrystalPlacements E2)
{
	bool cmpType = moduleMgr->getModule<AutoCrystalRewrite>()->safetyFirst;

	if (!cmpType)
	{ // Enem Dmg->high to low -> safetyFirst off
		return E1.enemyDmg > E2.enemyDmg;
	}

	if (cmpType)
	{ // Self Dmg->low to high -> safetyFirst on
		return E1.selfDmg < E2.selfDmg;
	}
}

bool cmpDup(CrystalPlacements E1, CrystalPlacements E2)
{
	bool cmpType = !(moduleMgr->getModule<AutoCrystalRewrite>()->safetyFirst);

	if (!cmpType)
	{
		return E1.enemyDmg > E2.enemyDmg;
	}

	if (cmpType)
	{
		return E1.selfDmg < E2.selfDmg;
	}
}

CrystalPlacements AutoCrystalRewrite::bestPlaceOnPlane(C_Entity* targ, int yLevel)
{

	std::vector<CrystalPlacements> validShit = generateValidPlacements(targ, yLevel);

	if (validShit.empty())
	{
		CrystalPlacements anEmptyValue;
		anEmptyValue.enemyDmg = -42069; // ima use this as the empty value
		return anEmptyValue;
	}

	std::sort(validShit.begin(), validShit.end(), cmpPlacements);

	// get duplicated values @ top

	float hl = 0.f;
	std::vector<CrystalPlacements> dups;

	if (safetyFirst)
	{
		for (const CrystalPlacements& i : validShit)
		{
			hl = i.selfDmg;

			if (i.selfDmg == hl)
			{
				dups.push_back(i);
			}
			else
				break;
		}
	}
	else if (!safetyFirst)
	{
		for (const CrystalPlacements& i : validShit)
		{
			hl = i.enemyDmg;

			if (i.enemyDmg == hl)
			{
				dups.push_back(i);
			}
			else
				break;
		}
	}

	if (dups.size() == 1)
		return dups[0];

	// sort dups
	int maxAdd = (amount < dups.size() ? amount : dups.size());
	std::sort(dups.begin(), dups.end(), cmpDup);
	return dups[0];
}
// Save time for  ontick calcs
int switchSlot;
bool grabNow = false;
// WTA WWWWWWWWWWWWWW
std::vector<vec3_t*> getMultiPlacement(C_Entity* ent) {
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<vec3_t*> multipleBlocks;
	std::vector<vec3_ti*> blockChecks;
	blockChecks.clear();
	multipleBlocks.clear();
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

		if (!checkMultiHitbox(&blocks->toVector3(), ent)) {
			if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7)) {
				multipleBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			}
			else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49)) {
				multipleBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			}
			else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && (blkIDLLL == 7 || blkIDLLL == 49)) {
				multipleBlocks.push_back(new vec3_t(blocks->x, blocks->y - 2, blocks->z));
			}
			else if (blkID == 0 && blkIDL == 0 && blkIDLL == 0 && &blkIDLLL == 0 && (blkIDLLLL == 7 || blkIDLLLL == 49)) {
				multipleBlocks.push_back(new vec3_t(blocks->x, blocks->y - 3, blocks->z));
			}
		}
		else {
			for (int x = entPos.x - 2; x <= entPos.x + 2; x++) {
				for (int z = entPos.z - 2; z <= entPos.z + 2; z++) {
					int y = entPos.y;
					auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
					auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

					if ((blk == 0 && (lBlk == 49 || lBlk == 7))) {
						multipleBlocks.push_back(new vec3_t(x, y, z));
					}
				}
			}
		}
	}
	return multipleBlocks;
}
std::vector<vec3_t*> multiPlaceMemory;
void AutoCrystalRewrite::placeCrystals() {
	C_GameMode* gm = g_Data.getCGameMode();
	auto cp = moduleMgr->getModule<AutoCrystalRewrite>();
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	slotMC = supplies->selectedHotbarSlot;
	if (swapEC && silentSwap)
		findSomeEndCrystals();
	if (swing.selected == 1)
		g_Data.getLocalPlayer()->swingArm();
	for (CrystalInfo& Place : RewritePlaceArr)
	{
		vec3_t placeMe = Place.CPlacements.toPlace;
		if (protocol.selected != 3)
			g_Data.getCGameMode()->buildBlock(&vec3_ti(placeMe).sub(0, 1, 0), 0);
		if (swing.selected == 0)
			g_Data.getLocalPlayer()->swingArm();
		hasBroken = false;
		hasPlaced = true;
	}
	std::vector<vec3_t*> getPlacement = getMultiPlacement(tgtList[0]);
	for (auto place : getPlacement) {
		multiPlaceMemory.clear();
		if (protocol.selected == 3)
			gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 2);
		multiPlaceMemory.push_back(new vec3_t(place->x, place->y - 1, place->z));
		hasPlaced = true;
	}
	return;
}
std::vector<vec3_t*> rotateMemory;
// Very long break code
void AutoCrystalRewrite::crystalBreak() {

	if (shouldBreak)
	{
		g_Data.forEachEntity([](C_Entity* ent, bool b)
			{
				int id = ent->getEntityTypeId();
				vec3_t entPos = ent->getPos()->floor();
				int Brange = moduleMgr->getModule<AutoCrystalRewrite>()->breakRange;
				auto playerHealth = g_Data.getLocalPlayer()->getHealth();
				auto minBreak = moduleMgr->getModule<AutoCrystalRewrite>()->breakHealth;
				auto safety = moduleMgr->getModule<AutoCrystalRewrite>()->safetyFirst;
				auto hasBroken = moduleMgr->getModule<AutoCrystalRewrite>()->hasBroken;
				auto hasPlaced = moduleMgr->getModule<AutoCrystalRewrite>()->hasPlaced;
				if (swing.selected == 1)
					g_Data.getLocalPlayer()->swingArm();
				if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= Brange) {
					if (!tgtList.empty()) {
						if (safety&& minBreak < playerHealth) {
							g_Data.getCGameMode()->attack(ent);
							if (swing.selected == 0)
								g_Data.getLocalPlayer()->swingArm();
						}
						else {
							g_Data.getCGameMode()->attack(ent);
							if (swing.selected == 0)
								g_Data.getLocalPlayer()->swingArm();
						}
						rotateMemory.push_back(new vec3_t(entPos));
						hasBroken = true;
						hasPlaced = false;
					}
				} 
			}
		);
	}
	return;
}
bool cmpAgain(CrystalPlacements E1, CrystalPlacements E2)
{
	return E1.enemyDmg > E2.enemyDmg;
}

CrystalPlacements AutoCrystalRewrite::AutoCrystalMain(C_Entity* target)
{
	std::vector<CrystalPlacements> on3d;

	int maxVOff = (facePlaceType.GetSelectedEntry().GetValue() == 1 ? 1 : 3); // faceplace disabled-> maxOffset = 1

	for (int vOffset = -5; vOffset < maxVOff; vOffset++)
	{

		CrystalPlacements bestPlace;
		bestPlace = bestPlaceOnPlane(target, vOffset);

		if (bestPlace.enemyDmg != -42069)
		{
			on3d.push_back(bestPlace);
		}
	}

	if (on3d.empty())
	{
		CrystalPlacements anEmptyValue;
		anEmptyValue.enemyDmg = -42069;
		return anEmptyValue;
	}

	// force sorting on3d into the highest enemy damage
	std::sort(on3d.begin(), on3d.end(), cmpAgain);
	return on3d[0];
}

bool compareTargListVec(C_Entity* E1, C_Entity* E2)
{
	int whatToCompare = moduleMgr->getModule<AutoCrystalRewrite>()->priority.GetSelectedEntry().GetValue();

	if (whatToCompare == 0)
	{ // distance comparison -> low to high
		return g_Data.getLocalPlayer()->getHumanPos().dist(E1->getHumanPos()) < g_Data.getLocalPlayer()->getHumanPos().dist(E2->getHumanPos());
	}
	else if (whatToCompare == 1)
	{ // health comparison -> low to high
		return E1->getHealth() < E2->getHealth();
	}
}

void AutoCrystalRewrite::onTick(C_GameMode* gm)
{
	auto cp = moduleMgr->getModule<AutoCrystalRewrite>();
	auto player = g_Data.getLocalPlayer();

	if (g_Data.getLocalPlayer() == nullptr || !g_Data.canUseMoveKeys())
		return;

	if (!ignoreEat)
	{
		if (player->getSelectedItemId() == 259 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 300 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 426 && g_Data.isRightClickDown())
			return;
	}
	ctr++;
	OBdelay++;
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	slotMC = supplies->selectedHotbarSlot;

	placeArrEmpty = RewritePlaceArr.empty();

	entLocator120.clear();
	tgtList.clear();
	RewritePlaceArr.clear();

	g_Data.forEachEntity(locateEntCrPl);

	if (tgtList.empty())
		return;

	std::sort(tgtList.begin(), tgtList.end(), compareTargListVec);

	if (!attackMulti)
	{
		bool canAttack = false;
		while (!canAttack)
		{
			CrystalPlacements placeInfo = AutoCrystalMain(tgtList[0]);
			if (placeInfo.enemyDmg != -42069)
			{
				CrystalInfo a2vec;
				a2vec.CPlacements = placeInfo;
				a2vec.ent = tgtList[0];
				RewritePlaceArr.push_back(a2vec);
				canAttack = true;
			}
			else
			{
				if (tgtList.size() != 1)
				{
					tgtList.erase(tgtList.begin());
				}
				else
					canAttack = true;
			}
		}
	}
	else if (attackMulti)
	{
		for (auto& enemy : tgtList)
		{

			CrystalPlacements placeInfo = AutoCrystalMain(enemy);
			if (placeInfo.enemyDmg != -42069)
			{
				CrystalInfo a2vec;
				a2vec.CPlacements = placeInfo;
				a2vec.ent = enemy;
				RewritePlaceArr.push_back(a2vec);
			}
		}
	}
	auto phase = moduleMgr->getModule<AutoClip>();
	bool pressed = MoveUtil::keyPressed();
	if (phase->isEnabled() && !pressed)
		phaseEnabled = true;
	if (!phase->isEnabled())
		phaseEnabled = false;
	if (pressed)
		phaseEnabled = false;
	// Main shit
	if (logicType.getSelectedValue() == 1)
		crystalBreak();
	if (ctr >= delay)
	{
		if (doPlace)
		placeCrystals();
		if (logicType.getSelectedValue() == 0 && OBdelay >= breakDelay) {
			crystalBreak();
		}
		if (swapEC && silentSwap)
			supplies->selectedHotbarSlot = slotMC;
		ctr = 0;
		OBdelay = 0;
	}
	return;
}
void AutoCrystalRewrite::onWorldTick(C_GameMode* gm)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (swapEC && !silentSwap && !g_Data.isRightClickDown()) // if switchType == regular; always grab crystals
		findSomeEndCrystals();
	if (swapEC && !silentSwap && g_Data.isRightClickDown())
	{
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
		return;
	}
	return;
}

void AutoCrystalRewrite::onDisable()
{
}

float roundoff(float value, unsigned char prec)
{
	float pow_10 = pow(10.0f, (float)prec);
	return round(value * pow_10) / pow_10;
}

std::string chopOffDigits(std::string STUFF, int digits)
{
	bool afterDecimalPt = false;
	int dp = 0;
	std::string toOutput;

	for (int i = 0; i < STUFF.length(); ++i)
	{
		if (STUFF.at(i) == '.')
		{
			afterDecimalPt = true;
		}

		toOutput.append(std::string(1, STUFF.at(i)));

		if (afterDecimalPt)
		{
			dp++;

			if (dp > digits)
				break;
		}
	}

	return toOutput;
}
vec3_t prevRenderPos;
void AutoCrystalRewrite::onPreRender(C_MinecraftUIRenderContext* renderCtx)
{
	if (g_Data.getLocalPlayer() == nullptr)
		return;

	if (!g_Data.canUseMoveKeys() || tgtList.empty() || !g_Data.isInGame() || tgtList[0] == NULL || tgtList.size() == 0 || tgtList[0] == nullptr || !g_Data.getLocalPlayer()->isAlive())
	{
		tgtList.clear();
		return;
	}
	// Place is off might aswell not render.
	if (!doPlace)
		return;

	if (RewritePlaceArr.empty())
		return;

	if (renderType.getSelectedValue() != 4) {
		for (CrystalInfo& indivCrystal : RewritePlaceArr) {
			if (RewritePlaceArr.empty() || tgtList.empty()) return;
			vec3_t placeLoc = indivCrystal.CPlacements.toPlace;
			placeLoc = prevRenderPos + (placeLoc - prevRenderPos) * lerpSpeed;
			auto interfaceColor = ColorUtil::interfaceColor(1);
			float enemyDmg = indivCrystal.CPlacements.enemyDmg;
			if (renderType.getSelectedValue() == 0 || renderType.getSelectedValue() == 2) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity + 0.2f);
				DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), .4f, false);
			}
			if (renderType.getSelectedValue() == 0 || renderType.getSelectedValue() == 1) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
				DrawUtils::drawBox(placeLoc.sub(0, 1, 0), placeLoc.add(1, 0, 1), 0.f, true);
			}
			if (renderType.getSelectedValue() == 3) {
				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity + 0.2f);
				DrawUtils::drawBox(placeLoc.sub(0, 0.01, 0), placeLoc.add(1, 0, 1), .4f, false);

				DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
				DrawUtils::drawBox(placeLoc.sub(0, 0.01, 0), placeLoc.add(1, 0, 1), 0.f, true);
			}
			vec3_t posplace = indivCrystal.ent->getHumanPos().floor();

			std::string enem_dmg = std::to_string(roundoff(enemyDmg, 2));
			enem_dmg = chopOffDigits(enem_dmg, 2);
			if (drawEnemDmg)
				DrawUtils::drawTextInWorld(&enem_dmg, placeLoc.sub(0, 1.2f, 0), 0.6f, vec3_ti(180, 180, 180), vec3_ti(12, 12, 12), .1f);

			prevRenderPos = placeLoc;
		}
	}
	return;
}
void AutoCrystalRewrite::onPlayerTick(C_Player* plr) {
	C_GameMode* gm = g_Data.getCGameMode();
	if (rotations.selected == 1) {
		for (auto rotateData : rotateMemory) {
			// Break Rotations
			if (rotationsMode.selected == 1 && phaseEnabled) {
				plr->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
				plr->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
			}
			if (rotationsMode.selected == 0) {
				plr->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
				plr->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
			}
		}
	}
}

void AutoCrystalRewrite::onSendPacket(C_Packet* pkt)
{
	if (pkt->isInstanceOf<PlayerAuthInputPacket>())
	{
		PlayerAuthInputPacket* packet = (PlayerAuthInputPacket*)pkt;
		C_GameMode* gm = g_Data.getCGameMode();
		if (rotations.selected == 2) {
			for (auto rotateData : rotateMemory) {
				if (rotationsMode.selected == 1 && phaseEnabled) {
					packet->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->yawUnused = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
				}
				if (rotationsMode.selected == 0) {
					packet->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->yawUnused = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
				}
			}
		}
	}
	if (pkt->isInstanceOf<C_MovePlayerPacket>())
	{
		auto* packet = (C_MovePlayerPacket*)pkt;
		C_GameMode* gm = g_Data.getCGameMode();
		if (rotations.selected == 2) {
			for (auto rotateData : rotateMemory) {
				// Break Rotations
				if (rotationsMode.selected == 1 && phaseEnabled) {
					packet->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->headYaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
				}
				if (rotationsMode.selected == 0) {
					packet->yaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->headYaw = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).y;
					packet->pitch = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(*rotateData).x;
				}
			}
		}
	}
}