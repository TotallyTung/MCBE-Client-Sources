#pragma once
#include "../Module.h"

class Hitbox : public Module {
private:
	float width = 1.f;
	float height = 1.f;
	bool mobs = false;
	std::vector<Actor*> espList;
public:
	Hitbox();

	virtual void onNormalTick(Actor* actor) override;
};