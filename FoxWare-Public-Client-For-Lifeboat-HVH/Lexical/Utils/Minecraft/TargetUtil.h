#pragma once
#include "../../SDK/World/LocalPlayer.h"

namespace TargetUtil {
	bool isTargetValid(Actor* target, bool isMob = false, float rangeCheck = 999999.f);
	bool sortByDist(Actor* a1, Actor* a2);
}