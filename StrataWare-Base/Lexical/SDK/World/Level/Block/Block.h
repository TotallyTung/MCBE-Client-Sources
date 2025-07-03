#pragma once
#include "BlockLegacy.h"

class Block {
public:
	CLASS_MEMBER(BlockLegacy*, blockLegacy, 0x30);
public:
	uint32_t* getRuntimeId() {
		return (uint32_t*)((uintptr_t)this + 0xC4);
	}
};