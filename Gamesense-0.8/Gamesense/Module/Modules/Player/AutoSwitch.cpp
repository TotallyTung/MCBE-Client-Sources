#include "AutoSwitch.h"

AutoSwitch::AutoSwitch() : IModule(0, Category::PLAYER, "Automatically switches between hotbar slots")
{
	// registerIntSetting("MinSlot", &slotMin, slotMin, 2, 9); TODO
	// registerIntSetting("MaxSlot", &slotMax, slotMax, 2, 9); TODO
	registerIntSetting("Slot", &slot, slot, 2, 9);
	// registerBoolSetting("Silent", &silent, silent);
}

const char *AutoSwitch::getRawModuleName()
{
	return ("AutoSwitch");
}

void AutoSwitch::onTick(C_GameMode *gm)
{
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr)
	{
		return;
	}
	if (!silent)
	{
		C_PlayerInventoryProxy *supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory *inv = supplies->inventory;
		supplies->selectedHotbarSlot = switchSlot;
		switchSlot++;
		if (switchSlot >= slot)
		{
			switchSlot = 0;
		}
	}
	if (silent)
	{
		auto supplies = g_Data.getLocalPlayer()->getSupplies();
		auto inv = supplies->inventory;
		silentSlot = supplies->selectedHotbarSlot;
		supplies->selectedHotbarSlot = silentSlot;
		silentSlot++;
		if (silentSlot >= slot)
		{
			switchSlot = 0;
		}
	}
}
