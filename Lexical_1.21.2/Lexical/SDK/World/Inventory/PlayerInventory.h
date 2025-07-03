#pragma once
#include "Container.h"

class PlayerInventory {
public:
	CLASS_MEMBER(int32_t, selectedSlot, 0x10);
	CLASS_MEMBER(Container*, container, 0xC0);
};