#pragma once
#include "../../Utils/Utils.h"

struct AABBShapeComponent {
public:
	AABB aabb;
	float width; //0x0018
	float height; //0x001C
}; //Size: 0x0020