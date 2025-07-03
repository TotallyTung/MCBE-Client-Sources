#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../../Utils/Maths.h"
#include "../EntityID.h"
struct WeakEntityRef {
private:
    uint8_t pad[0x10]{};

public:
    EntityId id;

private:
    uint8_t pad2[0x4]{};
};

enum class HitResultType : int32_t {
    BLOCK,
    ENTITY,
    ENTITY_OUT_OF_RANGE,
    AIR
};

class HitResult {
public:
    Vec3<float> startPos;
    Vec3<float> rayDirection;
    HitResultType type;
    int32_t selectedFace;
    BlockPos blockPos;
    Vec3<float> endPos;
    WeakEntityRef entity;
    bool isHitLiquid;
    unsigned char liquidFacing;
    BlockPos liquid;
    Vec3<float> liquidPos;
    bool indirectHit;
};