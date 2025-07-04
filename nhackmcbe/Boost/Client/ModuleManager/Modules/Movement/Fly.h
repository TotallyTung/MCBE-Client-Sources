#pragma once
#include "../Module.h"

class Fly : public Module {
private:
	float speed = 0.325f;
	float upanddown = 0.6f;
	float glideMod = -0.00f;
	float glideModEffective = 0;
	Vec3<float> oldPos;
	bool smborder;
	bool NoPacket = false;

public:
	Fly();
	~Fly();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onEnable() override;
	virtual void onDisable() override;
};
