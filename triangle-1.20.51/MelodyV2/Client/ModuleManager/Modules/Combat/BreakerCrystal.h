#pragma once
#include "../Module.h"

class CrystalBreaker1 : public Module {
private:
	int Mode = 0;
	int switchMode = 0;
	bool isInRange = false;
	bool hurttime = false;
	int attackDelay = 2;
	int attackDelayTick = 0;
	int getBestWeaponSlot();
public:
	float TargetDis = 0;
	float ARange = 5.f;
	float MaxSrange = ARange;
	float MinSrange = 0.f;
	float targetRange = 7.f;
	std::vector<Actor*> targetList;
	Vec2<float> rotAngle;
	CrystalBreaker1();
	int rotMode = 1;
	virtual void onNormalTick(Actor* actor) override;
	virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
	virtual void onImGuiRender(ImDrawList* drawlist) override;
};