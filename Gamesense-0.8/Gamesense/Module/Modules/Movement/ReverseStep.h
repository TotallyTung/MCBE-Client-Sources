#pragma once
#include "../Module.h"
class ReverseStep : public IModule {
public:
int speed = 15;
bool jump = false;
	ReverseStep();
	~ReverseStep();
	
	// Inherited via IModule
	virtual const char* getRawModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
