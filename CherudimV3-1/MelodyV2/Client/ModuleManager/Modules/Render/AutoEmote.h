#pragma once
#include "../Module.h"

class AutoEmote : public Module {
private:
	
	int EmoteDelay = 2;
public:
	AutoEmote();

	virtual void onNormalTick(Actor* actor) override;
};