#pragma once
#include "Actor.h"

class Mob : public Actor {
public:
	void setSprintingActorComponent(bool shouldSprint) {
		MemoryUtil::CallVFunc<153, void, bool>(this, shouldSprint);
	}
};
