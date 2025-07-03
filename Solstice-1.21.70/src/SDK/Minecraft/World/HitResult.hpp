#pragma once
//
// Created by vastrakai on 7/10/2024.
//

#include <SDK/Minecraft/Actor/EntityId.hpp>

enum HitType : int
{
    BLOCK = 0,
    ENTITY = 1,
    NOTHING = 3
};

struct WeakEntityRef {
    PAD(0x10);
    EntityId id;
    PAD(0x4);
};

class HitResult {
public:
    glm::vec3       mStartPos;     // this+0x0
    glm::vec3       mRayDir;       // this+0xC
    HitType         mType;         // this+0x18
    unsigned char   mFacing;       // this+0x1C
    glm::ivec3      mBlockPos;     // this+0x20
    glm::vec3       mPos;          // this+0x2C
    WeakEntityRef   mEntity;       // this+0x38
    bool            mIsHitLiquid;  // this+0x50
    unsigned char   mLiquidFacing; // this+0x51
    glm::ivec3      mLiquid;       // this+0x54
    glm::vec3       mLiquidPos;    // this+0x60
    bool            mIndirectHit;  // this+0x6C

    int getType() {
        return mType;
    }

    std::string getTypeString() {
        return std::string(magic_enum::enum_name(mType));
    }


};
