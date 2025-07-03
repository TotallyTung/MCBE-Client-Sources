#pragma once

#include "../../../../Utils/DrawUtils.h"
#include "../Module.h"

class SlowSwing : public IModule {
public:
	SlowSwing();
	~SlowSwing();
	int swingspeed = 30;
	virtual const char* getRawModuleName() override;
};
