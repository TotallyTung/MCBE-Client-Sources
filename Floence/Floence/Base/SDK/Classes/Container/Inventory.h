#pragma once

class ContainerContentChangeListener;
class Player;
class Level;

class Inventory
{
public:
	void dropSlot(int slot, bool clearContainer, bool dropAll, bool randomize)
	{             
		// 85 D2 0F 88 ? ? ? ? 48 89 5C 24 ? 55 56 57 41 56 41 57 48
		// FillingContainer::dropSlot
		using drop_t = void(__fastcall*)(Inventory*, int, bool, bool, bool);
		static drop_t func = reinterpret_cast<drop_t>(Memory::findSig(xorstr_("85 D2 0F 88 ? ? ? ? 48 89 5C 24 ? 55 56 57 41 54 41 55 41 56 41 57 48 8D AC 24")));
		if (func != 0)
			func(this, slot, clearContainer, dropAll, randomize);
	}

	ItemStack* getItem(int slot) {
		return Memory::CallVFunc<7, ItemStack*, int>(this, slot);
	}
};