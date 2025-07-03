// Created by Tony on 2024-10-10 10:01:54

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