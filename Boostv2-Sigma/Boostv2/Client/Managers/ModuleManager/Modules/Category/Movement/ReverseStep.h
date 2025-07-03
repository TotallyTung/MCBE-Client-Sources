#pragma once
#include "../../ModuleBase/Module.h"

class ReverseStep : public Module {
public:
	ReverseStep();
	virtual void onNormalTick(LocalPlayer* actor) override;
};