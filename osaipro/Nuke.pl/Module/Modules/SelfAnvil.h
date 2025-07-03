#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class SelfAnvil : public IModule {
private:
	bool trySelfAnvil(Vec3 SelfAnvil);
	int placed = 0;
public:
	SelfAnvil();
	~SelfAnvil();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
