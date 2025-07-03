#pragma once
#pragma once
#pragma once
#include "Module.h"

class PhaseForAntiAnvil : public IModule {
public:
	PhaseForAntiAnvil();
	~PhaseForAntiAnvil();

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
