#pragma once
#include "Actor.h"

class Mob : public Actor {
public:
	void setSprinting(bool shouldSprint) {
		MemoryUtil::CallVFunc<153, void, bool>(this, shouldSprint);
	}
};
