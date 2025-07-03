// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class Killaura : public Module {
private:
	float targetRange = 7.f;
	float wallRange = 5.f;
	int Mode = 0;
	int rotMode = 1;
	Vec2<float> rotAngle;
	int switchMode = 0;
	bool attackMob = false;
	bool hurttime = false;
	int attackDelay = 2;
	int attackDelayTick = 0;
	bool visualRange = true;
	UIColor vRColor = UIColor(255, 255, 255, 255);
	bool targetVisualize = true;
	UIColor visualizeColor = UIColor(255, 255, 255, 255);

	int getBestWeaponSlot();
public:
	std::vector<Actor*> targetList;
	Killaura();

	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};