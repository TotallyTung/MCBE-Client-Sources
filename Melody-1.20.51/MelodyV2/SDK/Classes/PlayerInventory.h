// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/MemoryUtils.h"
#include "Inventory.h"

class PlayerInventory {
public:
	BUILD_ACCESS(this, int, selectedSlot, 0x10);
	BUILD_ACCESS(this, Inventory*, inventory, 0xC0);
};
