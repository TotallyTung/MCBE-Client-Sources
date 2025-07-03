#pragma once

class ContainerManagerModel {
public:
	ItemStack* getSlot(int slot) {
		return Memory::CallVFunc<9, ItemStack*, int>(this, slot);
	}
};
