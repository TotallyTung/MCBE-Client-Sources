#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../../Utils/Maths.h"

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
};