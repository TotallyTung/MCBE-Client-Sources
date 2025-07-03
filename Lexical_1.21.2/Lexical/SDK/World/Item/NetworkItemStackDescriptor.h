#pragma once
#include "../../../Utils/MemoryUtil.h"

class ItemStack;

class NetworkItemStackDescriptor {
private:
	char pad_0x0[0x60];
public:
	NetworkItemStackDescriptor() {
		memset(this, 0x0, sizeof(NetworkItemStackDescriptor));
	}

	NetworkItemStackDescriptor(ItemStack* itemStack) {
		memset(this, 0x0, sizeof(NetworkItemStackDescriptor));
		using NetworkItemStackDescriptor_ctor_t = void(__thiscall*)(NetworkItemStackDescriptor*, ItemStack*);
		static NetworkItemStackDescriptor_ctor_t constructor = reinterpret_cast<NetworkItemStackDescriptor_ctor_t>(Addresses::NetworkItemStackDescriptor_Constructor);
		constructor(this, itemStack);
	}
};