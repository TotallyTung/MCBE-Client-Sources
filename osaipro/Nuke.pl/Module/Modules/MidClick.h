#pragma once

#include "Module.h"

class MidClick : public IModule {
private:
	bool hasClicked = false;

public:
	MidClick();
	~MidClick();
	bool tell = true;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
