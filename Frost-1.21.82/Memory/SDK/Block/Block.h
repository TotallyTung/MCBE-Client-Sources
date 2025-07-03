#pragma once

#include "Material/Type/MaterialType.h"
#include "Material/Material.h"
#include "Legacy/BlockLegacy.h"
#include "Palette/BlockPalette.h"

class Block
{
public:
	virtual ~Block();
public:
	BlockLegacy* getLegacyBlock() {
		return hat::member_at<BlockLegacy*>(this, OffsetManager::Block_mLegacy);
	}
};
