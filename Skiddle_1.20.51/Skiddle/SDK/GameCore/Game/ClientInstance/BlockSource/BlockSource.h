#pragma once

#include "Block.h"

class BlockSource
{
public:
	Block* getBlock(Vector3<int> blockPos)
	{
		return CallVFunc<6, Block*, Vector3<int>&>(this, blockPos);
	}
	Block* getBlock2(Vector3i blockPos)
	{
		return CallVFunc<6, Block*, Vector3i&>(this, blockPos);
	}
};