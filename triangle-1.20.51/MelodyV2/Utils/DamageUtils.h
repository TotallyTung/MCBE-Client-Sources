#pragma once
#include "../SDK/GameData.h"

class DamageUtils {
public:
	static float getExplosionDamage(const Vec3<float>& position, Actor* actor, float extrapolation = 0.f, int ignoredBlockId = 0);
	static float getPlayerAttackDamage(ItemStack* current);
	static float getPlayerAttackDamage();
};