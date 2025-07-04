#pragma once
#include "../Module.h"

class AntiAnvil : public Module {
private:
public:

	AntiAnvil();
	virtual void onNormalTick(Actor* actor) override;
};
