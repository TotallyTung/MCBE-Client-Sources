#pragma once
#include "../Module.h"

class GravityAura : public Module {
private:
	int block = 0;
	int pillarHeight = 3;
	int blockHeight = 3;
	int placeRange = 3;
	bool multiplace = false;
	bool onClick = false;
	bool Bypass = false;
	bool Mobs = false;
	//bool tryAutoFallBlock(Vec3<float> AutoFallBlock);
	std::vector<Actor*> targetList;
public:
	GravityAura();
	virtual void onNormalTick(Actor* actor);
	//virtual void onSendPacket(Packet* packet, bool& shouldCancel) override;
};