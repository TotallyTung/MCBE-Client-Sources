#pragma once
#include "../Utils/Math.h"
#include "../Utils/Utils.h"

class LevelRendererPlayer {
public:
	BUILD_ACCESS(this, Vec3<float>, origin, 0x5E4); // still cameraPos but can't modify...
	BUILD_ACCESS(this, Vec3<float>, cameraPos, 0x5F0);
};
