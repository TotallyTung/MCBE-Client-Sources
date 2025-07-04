#pragma once
#include "../Module.h"

class Clip : public Module {
private:
	float Clipvalue = 0.f;
public:

	Clip();
	virtual void onDisable() override;
	virtual void onNormalTick(Actor* actor) override;
};
