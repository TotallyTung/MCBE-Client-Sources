#pragma once
#include "../../ModuleBase/Module.h"

class PlayEmote : public Module {
private:

	int EmoteDelay = 2;
public:
	PlayEmote();

	virtual void onNormalTick(LocalPlayer* actor) override;
};