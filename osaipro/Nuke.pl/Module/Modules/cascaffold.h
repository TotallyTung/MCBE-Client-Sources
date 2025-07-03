#pragma once
#include "Module.h"
#include "../ModuleManager.h"
class cascaffold : public IModule {
private:
	void findObsi();
public:
	float range = 5;
	bool notarget = false;
	bool tryAutoTrap2(Vec3i blkPlacement);
	bool canpla(Vec3i blockPos);
	cascaffold();
	virtual void onJoiningServer() override;
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};