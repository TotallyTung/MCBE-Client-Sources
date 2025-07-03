#pragma once

#include "../../Utils/Math.h"

struct AABBShapeComponent
{
public:
	AABB aabb;
	BUILD_ACCESS(this, float, width,0x18);
	BUILD_ACCESS(this, float, height, 0x1C);

}; //Size: 0x0020