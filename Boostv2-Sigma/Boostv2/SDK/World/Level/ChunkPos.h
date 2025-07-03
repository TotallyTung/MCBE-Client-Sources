#pragma once
#include "../../../Utils/Maths.h"

struct ChunkPos {
public:
	int x;
	int z;
public:
	ChunkPos() {
		x = 0;
		z = 0;
	}

	ChunkPos(const BlockPos& blockPos) {
		x = blockPos.x & ~(15);
		z = blockPos.z & ~(15);
	}
};