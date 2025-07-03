#include "CrystalAuraGK.h"

bool fastMode = false;

using namespace std;

CrystalAuraGK::CrystalAuraGK() : IModule(0, Category::COMBAT, "Automatically places and destroyes crystals")
{
	// Safety
	registerBoolSetting("Safety", &safety, safety, "Prioritizes safety");
	registerFloatSetting("Place Health", &placeHealth, placeHealth, 0.f, 20.f, 0.01f, "If health is less than this then autocrystal will stop");
	registerFloatSetting("Break Health", &breakHealth, breakHealth, 0.f, 20.f, 0.1f, "If health is less than this then the client will stop breaking");
	registerBoolSetting("Ignore Eat", &ignoreEat, ignoreEat, "Will place anyways even if eating");
	registerBoolSetting("Stop Tool", &stopOnTool, stopOnTool, "Stops placing when holding tools");

	// Ranges
	registerIntSetting("Target Range", &targetRange, targetRange, 1, 15, "Minimum range to grab a target");
	registerIntSetting("Place Range", &range, range, 1, 10, "Minimum range to place end crystal");
	registerIntSetting("Break Range", &breakRange, breakRange, 1, 10, "Minimum range to place end crystal");
	registerIntSetting("Waste Amount", &rad, rad, 1, 10, "How much multiplace should waste(2 = 2x2 3 = 3x3 and so on)");
	registerIntSetting("Proximity", &maxProximity, maxProximity, 1, 8, "The maximum distance can a crystal be placed from a person before switching axis");

	// Place
	placement = SettingEnum(this)
					.addEntry(EnumEntry("Java", 0))
					.addEntry(EnumEntry("Vanilla", 1));
	registerEnumSetting("Placement", &this->placement, 0, "Control the clients placements and calculations");
	registerBoolSetting("Place", &autoplace, autoplace, "Should place");
	registerIntSetting("Place Delay", &placeDelay, placeDelay, 0, 20, "The delay of each place attemps");
	registerBoolSetting("MultiPlace", &doMultiple, doMultiple, "Should place multiple crystals at once");

	// Break
	registerBoolSetting("Break", &shouldBreak, shouldBreak, "Should break end crystals");
	registerIntSetting("Break Delay", &breakDelay, breakDelay, 0, 20, "The delay of each break attemps");

	// Swap
	switchType = SettingEnum(this)
					 .addEntry(EnumEntry("None", 0))
					 .addEntry(EnumEntry("Regular", 1))
					 .addEntry(EnumEntry("NukkitSpoof", 2));
	registerEnumSetting("Swap", &this->switchType, 0, "Switch to end crystal");
	// Render
	renderType = SettingEnum(this)
					 .addEntry(EnumEntry("Both", 0))
					 .addEntry(EnumEntry("Fill", 1))
					 .addEntry(EnumEntry("Box", 2))
					 .addEntry(EnumEntry("2D", 3))
					 .addEntry(EnumEntry("None", 4));
	registerEnumSetting("Render", &this->renderType, 0, "Controls how the client will render");
	registerFloatSetting("Outline-Opacity", &this->OutOpacity, 0.1f, 0.f, 1.f, 0.1f, "Opacity of the outline");
	registerFloatSetting("Fill-Opacity", &this->Opacity, 0.1f, 0.f, 1.f, 0.1f, "Opacity of the fill");
}
int crystalDelayidk = 0;
int crystalDelayidk2 = 0;
int crystalDelayidk3 = 0;

CrystalAuraGK::~CrystalAuraGK()
{
}

const char *CrystalAuraGK::getRawModuleName()
{
	return ("AutoCrystal");
}

const char *CrystalAuraGK::getModName()
{
	if (doMultiple)
		name = " Multi";
	else
		name = " Single";
	return name;
}

static std::vector<C_Entity *> targetListcc7;

void wtf1(C_Entity *currentEntity, bool isRegularEntity)
{
	static auto CrystalAuraGKMod = moduleMgr->getModule<CrystalAuraGK>();

	if (currentEntity == nullptr)
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 71) // crystal
		return;

	if (currentEntity == g_Data.getLocalPlayer()) // Skip Local player
		return;

	// if (!g_Data.getLocalPlayer()->canAttack(currentEntity, false))
	// return;

	if (!g_Data.getLocalPlayer()->isAlive())
		return;

	if (!currentEntity->isAlive())
		return;

	if (currentEntity->getNameTag()->getTextLength() <= 1 && currentEntity->getEntityTypeId() == 63)
		return;
	if (currentEntity->width <= 0.01f || currentEntity->height <= 0.01f) // Don't hit this pesky antibot on 2b2e.org
		return;
	if (currentEntity->getEntityTypeId() == 64) // item
		return;
	if (currentEntity->getEntityTypeId() == 69) // xp_orb
		return;
	if (currentEntity->getEntityTypeId() == 80) // arrow
		return;

	if (!TargetUtil::isValidTarget(currentEntity))
		return;

	// how hard is it to play fair? add back the badman check if its hard

	float dist = (*currentEntity->getPos()).dist(*g_Data.getLocalPlayer()->getPos());

	if (dist < CrystalAuraGKMod->targetRange)
	{
		targetListcc7.push_back(currentEntity);
	}
}

bool wtf2(vec3_t *block, C_Entity *ent)
{
	std::vector<vec3_t *> corners;
	corners.clear();

	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.lower.z));
	corners.push_back(new vec3_t(ent->aabb.lower.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.upper.z));
	corners.push_back(new vec3_t(ent->aabb.upper.x, ent->aabb.lower.y, ent->aabb.lower.z));
	int n = 0;
	if (!corners.empty())
		for (auto corner : corners)
		{
			n++;

			if ((floor(corner->x) == floor(block->x)) && (floor(corner->y) == floor(block->y)) && (floor(corner->z) == floor(block->z)))
			{
				return true;
			}
		}

	return false;
}

bool checkSurrounded222(C_Entity *ent)
{
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;

	std::vector<vec3_ti *> blockChecks;
	blockChecks.clear();

	if (blockChecks.empty())
	{
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks)
	{
		if (!wtf2(&blocks->toVector3(), ent))
		{
			return false;
		}
	}
	return true;
}

std::vector<vec3_t *> wtf3(C_Entity *ent)
{
	vec3_t entPos = ent->getPos()->floor();
	entPos.y -= 1;
	std::vector<vec3_t *> finalBlocks;
	std::vector<vec3_ti *> blockChecks;
	blockChecks.clear();
	finalBlocks.clear();
	if (blockChecks.empty())
	{
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z + 1));
		blockChecks.push_back(new vec3_ti(entPos.x, entPos.y, entPos.z - 1));
		blockChecks.push_back(new vec3_ti(entPos.x + 1, entPos.y, entPos.z));
		blockChecks.push_back(new vec3_ti(entPos.x - 1, entPos.y, entPos.z));
	}

	for (auto blocks : blockChecks)
	{
		auto blkID = g_Data.getLocalPlayer()->region->getBlock(*blocks)->toLegacy()->blockId;
		auto blkIDL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 1, blocks->z))->toLegacy()->blockId;
		auto blkIDLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 2, blocks->z))->toLegacy()->blockId;
		auto blkIDLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 3, blocks->z))->toLegacy()->blockId;
		auto blkIDLLLL = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blocks->x, blocks->y - 4, blocks->z))->toLegacy()->blockId;

		if (!wtf2(&blocks->toVector3(), ent))
		{ // very efficient coding here

			if (blkID == 0 && blkIDL == 0 && (blkIDLL == 49 || blkIDLL == 7))
			{
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y - 1, blocks->z));
			}
			else if (blkID == 0 && (blkIDL == 7 || blkIDL == 49))
			{
				finalBlocks.push_back(new vec3_t(blocks->x, blocks->y, blocks->z));
			}

			static auto GK = moduleMgr->getModule<CrystalAuraGK>();

			if (GK->doMultiple)
			{
				for (int x = entPos.x - GK->rad; x <= entPos.x + GK->rad; x++)
				{
					for (int z = entPos.z - GK->rad; z <= entPos.z + GK->rad; z++)
					{
						int y = entPos.y;
						auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
						auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

						if ((blk == 0 && (lBlk == 49 || lBlk == 7)))
						{
							finalBlocks.push_back(new vec3_t(x, y, z));
						}
					}
				}
			}
			else
			{
				for (int x = entPos.x - GK->maxProximity; x <= entPos.x + GK->maxProximity; x++)
				{
					for (int z = entPos.z - GK->maxProximity; z <= entPos.z + GK->maxProximity; z++)
					{
						for (int y = entPos.y - 4; z <= entPos.y + 4; z++)
						{
							// int y = entPos.y;
							auto blk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y, z))->toLegacy()->blockId;
							auto lBlk = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(x, y - 1, z))->toLegacy()->blockId;

							if ((blk == 0 && (lBlk == 49 || lBlk == 7)))
							{
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
void CrystalAuraGK::onEnable()
{
	crystalDelayidk = 0;
	placeCheck = false;
}

std::vector<vec3_t *> placeRender1;
// std::vector<vec3_t*> placeRender2;

void findCrystalGK()
{
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 637)
			{ // select crystal
				supplies->selectedHotbarSlot = n;
				// return true;
				return;
			}
		}
	}
	// return false;
}
void switchEndCrystal()
{
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory; // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID

	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
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
void CrystalAuraGK::onTick(C_GameMode *gm)
{
	auto antiDesync = moduleMgr->getModule<AntiDesync>();
	if (antiDesync->mode.getSelectedValue() == 0 && antiDesync->isEnabled())
	{
		auto player = g_Data.getLocalPlayer();
		if (player->getSelectedItemId() == 259 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 300 && g_Data.isRightClickDown())
			return;
		if (player->getSelectedItemId() == 426 && g_Data.isRightClickDown())
			return;
	}

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
	if (stopOnTool && g_Data.getLocalPlayer()->getSelectedItemId() == 300 || g_Data.getLocalPlayer()->getSelectedItemId() == 316 || g_Data.getLocalPlayer()->getSelectedItemId() == 318 || g_Data.getLocalPlayer()->getSelectedItemId() == 319 || g_Data.getLocalPlayer()->getSelectedItemId() == 604 || g_Data.getLocalPlayer()->getSelectedItemId() == 607 || g_Data.getLocalPlayer()->getSelectedItemId() == 606)
		return;

	if (g_Data.getLocalPlayer() == nullptr)
		return;
	auto playerHealth = g_Data.getLocalPlayer()->getHealth();
	auto minPlace = moduleMgr->getModule<CrystalAuraGK>()->placeHealth;
	int health = g_Data.getLocalPlayer()->getHealth();
	if (safety && health < minPlace)
		return;

	targetListcc7.clear();

	g_Data.forEachEntity(wtf1);

	if (autoplace)
		if ((crystalDelayidk >= this->placeDelay) && !(targetListcc7.empty()))
		{
			crystalDelayidk = 0;
			if (!checkSurrounded222(targetListcc7[0]))
			{
				std::vector<vec3_t *> gucciPositions = wtf3(targetListcc7[0]);

				auto supplies = g_Data.getLocalPlayer()->getSupplies();
				auto inv = supplies->inventory;
				slotGK = supplies->selectedHotbarSlot;

				// 615 = normal id for crystal || 616 = crystal id for nukkit servers
				if (!gucciPositions.empty())
				{
					if (switchType.getSelectedValue() == 2)
						findCrystalGK();
					if (switchType.getSelectedValue() == 1)
					{
						origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
						switchEndCrystal();
					}
					if (g_Data.getLocalPlayer()->getSelectedItemId() == 637)
					{
						placeRender1.clear();
						for (auto place : gucciPositions)
						{
							if (placeCheck && !doMultiple)
								break;
							if (targetListcc7.empty())
								return;
							// if (silentGK) findCrystalGK();
							gm->buildBlock(&vec3_ti(place->x, place->y - 1, place->z), 1);
							placeRender1.push_back(new vec3_t(place->x, place->y - 1, place->z));
							placeCheck = true;
						}
					}
					if (switchType.getSelectedValue() == 2)
						supplies->selectedHotbarSlot = slotGK;
				}
				gucciPositions.clear();
			}
		}
		else if (!targetListcc7.empty())
		{
			crystalDelayidk++;
		}

	OBdelay++;
	if (shouldBreak && OBdelay > breakDelay)
	{
		g_Data.forEachEntity([](C_Entity *ent, bool b)
							 {
							int id = ent->getEntityTypeId();
							PointingStruct* pstruct = g_Data.getLocalPlayer()->pointingStruct;
							auto playerHealth = g_Data.getLocalPlayer()->getHealth();
							int Brange = moduleMgr->getModule<CrystalAuraGK>()->breakRange;
							auto minBreak = moduleMgr->getModule<CrystalAuraGK>()->breakHealth;
							auto safety = moduleMgr->getModule<CrystalAuraGK>()->safety;
							int health = g_Data.getLocalPlayer()->getHealth();
		if (targetListcc7.empty()) return;
		if (id == 71 && g_Data.getLocalPlayer()->getPos()->dist(*ent->getPos()) <= Brange) {
			if (safety && health > minBreak) {
				g_Data.getCGameMode()->attack(ent);
			}
			if (!safety) {
				g_Data.getCGameMode()->attack(ent);
			}
			placeCheck = false;
				g_Data.getLocalPlayer()->swingArm();
		} });
		OBdelay = 0;
	}
}

void CrystalAuraGK::onDisable()
{
	crystalDelayidk = 0;
	placeCheck = false;
}
void CrystalAuraGK::onPreRender(C_MinecraftUIRenderContext *renderCtx)
{
	C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
	if (localPlayer != nullptr && GameData::canUseMoveKeys())
	{
		if (!targetListcc7.empty())
		{
			if (!placeRender1.empty())
			{
				for (auto postt : placeRender1)
				{
					// Vectors
					vec3_t block = postt->floor().add(0.5, 0.5, 0.5);
					// Variables
					auto interfaceColor = ColorUtil::interfaceColor(1);
					auto render = renderType.getSelectedValue();

					// The render
					// Outline & Fill
					if (render == 0 || render == 2)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox(postt->floor(), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 0.5f);
					}
					if (render == 0)
					{
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
						DrawUtils::drawFilledBox(block);
					}
					// 2D
					if (render == 3)
					{
						// 2D Outline
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, OutOpacity);
						DrawUtils::drawBox(postt->floor().add(0.f, 0.999f, 0.f), vec3_t(floor(postt->x) + 1.f, floor(postt->y) + 1.f, floor(postt->z) + 1.f), 0.5f);

						// 2D Fill
						DrawUtils::setColor(interfaceColor.r, interfaceColor.g, interfaceColor.b, Opacity);
						DrawUtils::drawFilledBox2D(block);
					}
				}
			}
		}
	}
}
