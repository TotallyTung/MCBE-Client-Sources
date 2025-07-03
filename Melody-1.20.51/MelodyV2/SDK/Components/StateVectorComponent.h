// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/Math.h"

class StateVectorComponent {
public:
	Vec3<float> pos; //0x0000
	Vec3<float> prevPos; //0x000C
	Vec3<float> velocity; //0x0018
}; //Size: 0x0024