#pragma once

#include "ItemStackBase.h"

class ItemStack : public ItemStackBase
{
public:
	uint8_t mStackNetId;          // this+0x0088
	char    pad_0x0089[0x8];     // this+0x0089

	bool isBlockType() {
		return mBlock != nullptr;
	}

	std::string getHoverName() {
		std::string str;
		Memory::CallFastcall<void, ItemStack*, std::string*>(SigManager::ItemStack_getCustomName, this, &str);
		return str;
	}

	bool customNameContains(const std::string& itemName) {
		return getHoverName().find(itemName) != std::string::npos;
	}

	int32_t getEnchantValue(EnchantType enchantType) {
		return Memory::CallFastcall<int, EnchantType, ItemStack*>(SigManager::ItemStack_getEnchantValue, enchantType, this);
	}
};
//static_assert(sizeof(ItemStack) == 0x98, "ItemStack size invalid");