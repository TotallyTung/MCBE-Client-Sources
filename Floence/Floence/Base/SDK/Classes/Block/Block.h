#pragma once

#include "MaterialType.h"
#include "Material.h"
#include "BlockLegacy.h"

class Block
{
public:
	BlockLegacy* GetBlockLegacy() {
		return *reinterpret_cast<BlockLegacy**>((uintptr_t)this + 0x78); // 1.21.73
	}

	std::string GetTileName()
	{
		//return GetBlockLegacy()->tileName;
		return GetBlockLegacy()->getTileName();
	}

	Material* GetMaterial()
	{
		return GetBlockLegacy()->GetMaterial();
	}

	MaterialType GetMaterialType()
	{
		return GetBlockLegacy()->GetMaterial()->type;
	}
};
