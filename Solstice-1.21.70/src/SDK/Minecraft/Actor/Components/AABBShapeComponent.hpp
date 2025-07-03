//
// Created by vastrakai on 6/28/2024.
//

#pragma once


#include <Utils/Structs.hpp>

struct AABBShapeComponent {
    glm::vec3 mMin;
    glm::vec3 mMax;
    float mWidth;
    float mHeight;

    AABB getAABB() const {
        return AABB(mMin, mMax, true);
    }
};

static_assert(sizeof(AABBShapeComponent) == 0x20);