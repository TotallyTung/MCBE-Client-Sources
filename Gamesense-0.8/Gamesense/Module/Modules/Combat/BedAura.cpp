#include "BedAura.h"
int Bdelay = 0;
int OBAdelay = 0;
BedAura::BedAura() : IModule(0, Category::COMBAT, "Automatically breaks beds")
{
	registerEnumSetting("Swap", &this->swap, 0, "Switch to beds");
	swap.addEntry(EnumEntry("None", 0));
	swap.addEntry(EnumEntry("Regular", 1));
	swap.addEntry(EnumEntry("Geyser", 2));
	swap.addEntry(EnumEntry("Silent", 3));
	swap.addEntry(EnumEntry("NukkitSpoof", 4));
	registerEnumSetting("Rotations", &this->rotations, 0, "Controls the break rotations");
	rotations.addEntry(EnumEntry("None", 0));
	rotations.addEntry(EnumEntry("Normal", 1));
	rotations.addEntry(EnumEntry("Silent", 2));
	registerIntSetting("Range", &range, range, 1, 10, "The range for the client to start breaking beds");
	registerBoolSetting("Suicide", &suicide, suicide, "Ignore minimum health and keep breaking anyways");
	registerBoolSetting("Target", &targetOnly, targetOnly, "Only break beds when theres a player nearby");
	registerBoolSetting("FastPlace", &fastMode, fastMode, "Allows you to place beds very fast");
	registerFloatSetting("Minimum Health", &minHealth, minHealth, 1, 20, 0.1f, "If health is less than this then the client will stop breaking beds(unless suicide is on)");
}

BedAura::~BedAura()
{
}

const char *BedAura::getRawModuleName()
{
	return ("BedAura");
}
static std::vector<C_Entity *> targetList;
bool findTargets(C_Entity *curEnt, bool isRegularEntity)
{
	auto bedAura = moduleMgr->getModule<BedAura>();
	if (bedAura->targetOnly)
	{
		if (curEnt == nullptr)
			return false;
		if (curEnt == g_Data.getLocalPlayer())
			return false; // Skip Local player
		if (!curEnt->isAlive())
			return false;
		if (!g_Data.getLocalPlayer()->isAlive())
			return false;
		if (curEnt->getEntityTypeId() == 71)
			return false; // endcrystal
		if (curEnt->getEntityTypeId() == 66)
			return false; // falling block
		if (curEnt->getEntityTypeId() == 64)
			return false; // item
		if (curEnt->getEntityTypeId() == 69)
			return false; // xp orb
		if (curEnt->width <= 0.01f || curEnt->height <= 0.01f)
			return false; // Don't hit this pesky antibot on 2b2e.org
		if (!TargetUtil::isValidTarget(curEnt))
			return false;

		float dist = (curEnt->getHumanPos()).dist(g_Data.getLocalPlayer()->getHumanPos());
		if (dist <= moduleMgr->getModule<BedAura>()->range)
		{
			targetList.push_back(curEnt);
			return true;
		}
		return false;
	}
}
// Regular Swap
void swapReg()
{
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;

	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 418)
			{
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
// MobEquipment Swap
bool swapMob(int Itemid)
{
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 36; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if ((*stack->item)->itemId != 0)
			{
				if (stack->getItem()->itemId == Itemid)
				{
					C_MobEquipmentPacket a(id, stack, n, n);
					g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
					return true;
				}
			}
		}
	}
	return false;
}
bool swapMobSpoof()
{
	__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if ((*stack->item)->itemId == 418)
			{
				C_MobEquipmentPacket a(id, stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
	C_MobEquipmentPacket a(id, g_Data.getLocalPlayer()->getSelectedItem(), supplies->selectedHotbarSlot, supplies->selectedHotbarSlot);
	g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
	return false;
}
// Nukkit Spoof
void swapNukkit()
{
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if (stack->getItem()->itemId == 418)
			{
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}
bool stopMob()
{
	__int64 id = -1;
	C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory *inv = supplies->inventory;
	for (int n = 0; n < 9; n++)
	{
		C_ItemStack *stack = inv->getItemStack(n);
		if (stack->item != nullptr)
		{
			if ((*stack->item)->itemId == 259)
			{
				C_MobEquipmentPacket a(id, stack, n, n);
				g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&a);
				return true;
			}
		}
	}
}
void BedAura::onTick(C_GameMode *gm)
{
	g_Data.forEachEntity(findTargets);
	auto player = g_Data.getLocalPlayer();
	PointingStruct *pointing = g_Data.getLocalPlayer()->pointingStruct;
	C_Block *block = g_Data.getLocalPlayer()->region->getBlock(pointing->block);
	int blockID = (int)block->toLegacy()->blockId;
	C_LocalPlayer *localPlayer = g_Data.getLocalPlayer();
	auto selectedItemId = localPlayer->getSelectedItemId();

	// Safety
	if (!suicide)
	{
		int playerHealth = g_Data.getLocalPlayer()->getHealth();
		if (playerHealth < minHealth)
			return;
	}
	// StopOnEat
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
	targetList.clear();
	if (targetList.empty() && targetOnly)
		return;

	// Swap
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;
	slotNukkit = supplies->selectedHotbarSlot;
	switch (swap.getSelectedValue())
	{
	case 1:
		origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
		swapReg();
		break;
	case 2:
		swapMob(418);
		break;
	case 3:
		swapMobSpoof();
		break;
	case 4:
		swapNukkit();
		break;
	}

	vec3_t *pos = gm->player->getPos();
	for (int x = (int)pos->x - range; x < pos->x + range; x++)
	{
		for (int z = (int)pos->z - range; z < pos->z + range; z++)
		{
			for (int y = (int)pos->y - range; y < pos->y + range; y++)
			{
				vec3_ti pos = vec3_ti(x, y, z);
				C_Block *block = gm->player->region->getBlock(pos);
				if (block != nullptr && g_Data.canUseMoveKeys())
				{
					auto id = gm->player->region->getBlock(pos)->toLegacy()->blockId;
					bool destroy = false;
					if (id == 26)
						destroy = true;
					if (destroy)
					{
						/*if (rotations.GetSelectedEntry().GetValue() == 1 || rotations.GetSelectedEntry().GetValue() == 2)
						{
							//rotAngleB = g_Data.getLocalPlayer()->getHumanPos().CalcAngle(pos);
						}*/
						// Grr rot no work
						gm->buildBlock(&pos, 0);
						g_Data.getLocalPlayer()->swing();
						bedList.push_back(pos);
						return;
					}
				}
			}
		}
	}
	if (g_Data.isRightClickDown() && g_Data.canUseMoveKeys() && fastMode && selectedItemId == 26) {
		OBAdelay++;
		if (OBAdelay >= Bdelay) {
			g_Data.rightclickCount++;
			if (blockID != 0) gm->buildBlock(new vec3_ti(pointing->block), pointing->blockSide);
		}
		OBAdelay = 0;
	}
	if (swap.getSelectedValue() == 2)
		stopMob();
	if (swap.getSelectedValue() == 4)
		supplies->selectedHotbarSlot = slotNukkit;
}
void BedAura::onDisable()
{
	if (swap.GetSelectedEntry().GetValue() == 1)
	{
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		supplies->selectedHotbarSlot = origSlot;
		return;
	}
	bedList.clear();
}
