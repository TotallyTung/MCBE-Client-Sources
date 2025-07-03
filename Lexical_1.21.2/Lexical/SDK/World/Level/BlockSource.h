#pragma once
#include "Block/Block.h"
#include "ChunkPos.h"

class BlockSource {
public:
	Block* getBlock(const BlockPos& pos) {
		return MemoryUtil::CallVFunc<2, Block*, const BlockPos&>(this, pos);
	}

	float getSeenPercent(const Vec3<float>& pos, const AABB& aabb) {
		using func_t = float(__thiscall*)(BlockSource*, const Vec3<float>&, const AABB&);
		static func_t Func = reinterpret_cast<func_t>(Addresses::BlockSource_getSeenPercent);
		return Func(this, pos, aabb);
	}
};