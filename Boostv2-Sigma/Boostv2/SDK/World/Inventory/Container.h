#pragma once
#include "../Item/ItemStack.h"

class Container {
public:
	ItemStack* getItem(int slot) {
		return MemoryUtil::CallVFunc<7, ItemStack*, int>(this, slot);
	}

	void removeItem(int slot, int count) {
		MemoryUtil::CallVFunc<14, void, int, int>(this, slot, count);
	}
};