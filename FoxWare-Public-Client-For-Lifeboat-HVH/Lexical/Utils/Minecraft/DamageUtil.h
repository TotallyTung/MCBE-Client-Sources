#pragma once
#include "../../SDK/Game.h"

class DamageUtils {
public:
	static float getExplosionDamage(const Vec3<float>& position, Actor* actor, float extrapolation = 0.f, int ignoredBlockId = 0);
};