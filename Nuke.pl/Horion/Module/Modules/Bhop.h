#pragma once
#include "..\ModuleManager.h"
#include "Module.h"

class Bhop : public IModule {
private:
	float speed = 0.325f;
	float up = 0.10f;
	bool lowhop = true;
	bool step = true;
	bool v2 = false;

public:
	Bhop();
	~Bhop();

	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onMove(MoveInputHandler* input) override;
	virtual void onDisable() override;
};
