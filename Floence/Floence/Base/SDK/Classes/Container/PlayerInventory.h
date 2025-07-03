#pragma once

class PlayerInventory
{
public:
	BUILD_ACCESS(int, hotbarSlot, 0x10); // 1.21.73
	BUILD_ACCESS(Inventory*, inventory, 0xB8); // 1.21.73
};