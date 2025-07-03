#pragma once
#include "../../../../Utils/MemoryUtil.h"
#include "../../../../Utils/Maths.h"

class BlockSource;
class Block;

enum BlockRenderLayer : int32_t {
	RENDERLAYER_DOUBLE_SIDED = 0x0,
	RENDERLAYER_BLEND = 0x1,
	RENDERLAYER_OPAQUE = 0x2,
	RENDERLAYER_OPTIONAL_ALPHATEST = 0x3,
	RENDERLAYER_ALPHATEST = 0x4,
	RENDERLAYER_SEASONS_OPAQUE = 0x5,
	RENDERLAYER_SEASONS_OPTIONAL_ALPHATEST = 0x6, // red
	RENDERLAYER_ALPHATEST_SINGLE_SIDE = 0x7, // red
	RENDERLAYER_ENDPORTAL = 0x8,
	RENDERLAYER_BARRIER = 0x9,
	RENDERLAYER_STRUCTURE_VOID = 0xA,
	_RENDERLAYER_COUNT = 0xB,
};

class BlockLegacy {
public:
	CLASS_MEMBER(uint16_t, blockId, 0x1A6);
	CLASS_MEMBER(std::string, blockName, 0x28);
	CLASS_MEMBER(BlockRenderLayer, renderLayer, 0x0B4);
public:
	AABB* getOutline(Block* block, BlockSource* blockSource, BlockPos* blockPos, AABB* aabbOut) {
		return MemoryUtil::CallVFunc<9, AABB*>(this, block, blockSource, blockPos, aabbOut);
	}

	bool canBeBuiltOver(BlockSource* region, BlockPos const& blockPos) {
		return MemoryUtil::CallVFunc<104, bool>(this, region, blockPos);
	}

	bool isSolid() {
		return *(bool*)((uintptr_t)this + 0x164);
	}
};
