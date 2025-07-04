#pragma once
#include "../../Utils/MemoryUtils.h"
#include "BlockLegacy.h"

class Block {
public:
	char pad_0x0[0x30]; //0x0000
	BlockLegacy* blockLegacy; //0x0030
public:
	uint32_t getRuntimeId() {
		return *(uint32_t*)((__int64)this + 196);
	}
};
