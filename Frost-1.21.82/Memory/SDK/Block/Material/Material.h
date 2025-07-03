#pragma once

class Material
{
public:
	MaterialType mType;
	bool         mNeverBuildable;
	bool         mLiquid;
	bool         mBlocksMotion;
	bool         mBlocksPrecipitation;
	bool         mSolid;
	bool         mSuperHot;
};
