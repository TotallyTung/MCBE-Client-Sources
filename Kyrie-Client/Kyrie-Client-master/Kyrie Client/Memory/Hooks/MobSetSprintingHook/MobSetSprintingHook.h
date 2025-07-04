#pragma once
#include "../../../SDK/Mob.h"
class MobSetSprintingHook {
private:
	static __int64 setSprinting(Mob* a1, unsigned __int8 a2);
public:
	static void init();
};