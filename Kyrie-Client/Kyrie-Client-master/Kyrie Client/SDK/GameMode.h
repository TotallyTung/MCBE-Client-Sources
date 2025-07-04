#pragma once
#include "../Utils/Math.h"
#include "../Utils/Utils.h"

class GameMode {
public:
	BUILD_ACCESS(this, Vec3<int>, breakPos, 0x10);
	BUILD_ACCESS(this, float, destroyProgress, 0x24);
public:
	virtual ~GameMode();
};
