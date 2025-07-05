#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class The2b2eSpeed : public IModule {
private:
	float speed = 0.325f;
	float up = 0.10f;
	bool lowhop = true;
	bool step = true;
	bool v2 = false;

public:
	The2b2eSpeed();
	~The2b2eSpeed();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(C_MoveInputHandler* input) override;
	virtual void onDisable() override;
};
