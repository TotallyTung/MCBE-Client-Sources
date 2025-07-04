#pragma once
#include "../../../SDK/Mob.h"

class MobGetCurrentSwingDurationHook {
private:
	static int MobGetCurrentSwingDuration(Mob* a1);
public:
	static void init();
};