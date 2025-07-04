#pragma once
#include "../Module.h"

class Scaffold : public Module {
private:
	Vec3<float> targetBlock = {};
	float targetY = 0.f;
	bool rot = true;

	int placeticks;
	bool canPlace(Vec3<float> pos);
	bool predictBlock(Vec3<float> pos, GameMode* gm);
	bool buildBlock(Vec3<float> pos, GameMode* gm);
	Vec2<float> rotAngle;
public:
	Scaffold();
	virtual void onEnable() override;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};