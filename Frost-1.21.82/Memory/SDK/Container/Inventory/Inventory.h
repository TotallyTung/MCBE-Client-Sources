#pragma once

class Inventory
{
public:
	void dropSlot(int slot, bool clearContainer, bool dropAll, bool randomize)
	{
		Memory::CallFunc<void, Inventory*, int, bool, bool, bool>(
			SigManager::FillingContainer_dropSlot, this, slot, clearContainer, dropAll, randomize);
	}

	ItemStack* getItemStack(int slot)
	{
		return Memory::CallVFunc<7, ItemStack*, int>(this, slot);
	}

	void setItem(int slot, ItemStack* item) 
	{
		Memory::CallVFunc<7 + 5, ItemStack*, int, ItemStack*>(this, slot, item);
	}
};