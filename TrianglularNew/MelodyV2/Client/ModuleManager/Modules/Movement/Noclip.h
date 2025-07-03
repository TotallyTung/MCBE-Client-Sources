#pragma once
#include "../Module.h"

class Noclip : public Module {
public:
	float Speed = 4.f;
	Noclip();
	Vec2<float> FlyRot;
	bool Strafe = false;
	Vec3<float> MoveVec = (0, 0, 0);
	Vec2<float> motion;
	virtual void onNormalTick(Actor* actor) override;
};
