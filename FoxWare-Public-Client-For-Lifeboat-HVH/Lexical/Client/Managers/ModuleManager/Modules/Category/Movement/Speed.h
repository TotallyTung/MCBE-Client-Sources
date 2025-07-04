#pragma once
#include "../../ModuleBase/Module.h"

class Speed : public Module {
protected:
	float speed = 1.f;
	bool jump = false;
	int sprintType = 0;
	int mode = 0;
public:
	Speed();
	virtual void onNormalTick(LocalPlayer* actor);
};