#pragma once
#include "../Module.h"

class Speed : public Module {
public:
	float speed = 3.f;
	int stage = 0;
	Speed();
	virtual void onNormalTick(Actor* actor);
	virtual void onSendPacket(Packet* packet, bool& shouldCancel);
	//virtual std::string getModName() override;
};
