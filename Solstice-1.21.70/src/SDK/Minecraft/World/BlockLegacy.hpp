#pragma once
//
// Created by vastrakai on 7/3/2024.
//

#include <optional>
#include <vector>
#include <Utils/MemUtils.hpp>


#include <Utils/Structs.hpp>

class Material
{
public:
    CLASS_FIELD(int, mType, 0x0);
    CLASS_FIELD(bool, mIsFlammable, 0x4);
    CLASS_FIELD(bool, mIsNeverBuildable, 0x5);
    CLASS_FIELD(bool, mIsLiquid, 0x6);
    CLASS_FIELD(bool, mIsBlockingMotion, 0xC);
    CLASS_FIELD(bool, mIsBlockingPrecipitation, 0xD);
    CLASS_FIELD(bool, mIsSolid, 0xE);
    CLASS_FIELD(bool, mIsSuperHot, 0xF);

    bool isTopSolid(bool includeWater, bool includeLeaves)
    {
        if (mType == 7) return includeLeaves;
        if (!includeWater) return mIsBlockingMotion;
        if (mType != 5) return mIsBlockingMotion;
        return true;
    }
};

class BlockLegacy {
public:
    CLASS_FIELD(uintptr_t**, mVfTable, 0x0);
    CLASS_FIELD(std::string, mTileName, 0x28);
    CLASS_FIELD(std::string, mName, 0xA0); // not updated yet
    CLASS_FIELD(Material*, mMaterial, 0x160);
    CLASS_FIELD(bool, mSolid, 0x19C);

    uint16_t getBlockId();
    bool mayPlaceOn(glm::ivec3 pos);
    bool isAir();
    AABB getCollisionShape(Block* block, BlockSource* source, BlockPos pos);
};