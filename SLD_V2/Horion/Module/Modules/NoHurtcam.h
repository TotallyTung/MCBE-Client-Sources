#pragma once
#include "Module.h"

class NoHurtcam : public IModule{
public:
	NoHurtcam();
	~NoHurtcam();
	const char* getModuleName() override;
	virtual void onWorldTick(C_GameMode* gm) override;
};
