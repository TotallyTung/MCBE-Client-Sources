#pragma once
#include "../../Utils/Target.h"
#include "../ModuleManager.h"
#include "Module.h"

class HitBoxes : public IModule {
public:
	float height = 3.f;
	float width = 4.f;
	float range = 8.f;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	HitBoxes();
};