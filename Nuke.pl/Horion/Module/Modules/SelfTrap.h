#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class SelfTrap : public IModule {
private:
	bool trySelfTrap(Vec3 SelfTrap);
	int placed = 0;
public:
	SelfTrap();
	~SelfTrap();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onDisable() override;
};
