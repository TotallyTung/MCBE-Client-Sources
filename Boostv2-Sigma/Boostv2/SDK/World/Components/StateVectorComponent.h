#pragma once
#include "../../../Utils/Maths.h"

struct StateVectorComponent {
    Vec3<float> pos;
    Vec3<float> oldPos;
    Vec3<float> velocity;
};
