#pragma once
#include "../../SDK/Game.h"

namespace InventoryUtil {
	void switchSlot(int32_t slot);
	ItemStack* getItem(int32_t slot);
	int32_t findItemSlotInHotbar(uint16_t itemId);
	int32_t findItemSlotInInventory(uint16_t itemId);
}