#pragma once

// This should be in Actor but using Mob for a clean base since Mob is a child of Actor.
// Mob VTable
class Mob : public Actor { // 1.21.73
public:
	void setSprinting(bool shouldSprint) {
		Memory::CallVFunc<140, void, bool>(this, shouldSprint);
	}
};
