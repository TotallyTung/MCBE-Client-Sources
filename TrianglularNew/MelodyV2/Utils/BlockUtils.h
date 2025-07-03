#pragma once
#include "Math.h"

enum class BLOCKTYPE : int { // side project!
	AIR = 0,
	GRASS = 1,
	DIRT = 2,
	PISTON_NORMAL = 33,
	PISTON_STICKY = 29,
	PISTON_ARM_NORMAL = 34,
	PISTON_ARM_STICKY = 472,
	OBSIDIAN = 49,
	BEDROCK = 7,
	BED = 26,
	REDSTONE_BLOCK = 152
};

class BlockUtils {
public:
	static bool isContainer(Vec3<int> pos);
	static int getBlockId(Vec3<int> pos);
	static std::string getBlockName(Vec3<int> pos);
	static bool isReplaceable(Vec3<int> pos);
	static bool buildBlock(Vec3<int> pos, bool airplace = false);
	static bool canBuildBlock(Vec3<int> pos, bool airplace = false);
	static BLOCKTYPE getBlockType(Vec3<int> pos);

	static bool isContainer(Vec3<float> pos);
	static int getBlockId(Vec3<float> pos);
	static bool isReplaceable(Vec3<float> pos);
	static std::string getBlockName(Vec3<float> pos);
	static bool buildBlock(Vec3<float> pos, bool airplace = false);
	static bool canBuildBlock(Vec3<float> pos, bool airplace = false);
	static BLOCKTYPE getBlockType(Vec3<float> pos);
};
