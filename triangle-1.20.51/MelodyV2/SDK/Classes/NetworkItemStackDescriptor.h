#pragma once

#include "ItemStack.h"

class NetworkItemStackDescriptor {
public:
	NetworkItemStackDescriptor() = default;
	NetworkItemStackDescriptor(const ItemStack& item) {
		memset(this, 0x0, sizeof(NetworkItemStackDescriptor));
		using constructor_t = void(__fastcall*)(NetworkItemStackDescriptor*, const ItemStack&);
		static constructor_t  constructor = (constructor_t)(findSig(Sigs::networkitemstackdescriptor::constructor));
		constructor(this, item);
		//48 89 5C 24 ? 55 56 57 41 56 41 57 48 83 EC ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 44 24 ? 48 8B FA 48 8B F1 48 89 4C 24 ? ? ? 5A 22
	}
	void fromStack(ItemStack* item) {
		using constructor_t = void(__fastcall*)(NetworkItemStackDescriptor*, const ItemStack*);
		static constructor_t  constructor = (constructor_t)(findSig(Sigs::networkitemstackdescriptor::fromStack));
		constructor(this, item);


	}
};