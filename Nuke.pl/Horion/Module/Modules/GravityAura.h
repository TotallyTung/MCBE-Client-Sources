#pragma once

#include "Module.h"
#include "../ModuleManager.h"
class GravityAura : public IModule {
private:
	SettingEnum block = this;
	std::vector<Entity*> entityList;
	static void findEntity(Entity* currentEntity, bool isRegularEntity);

private:
	int placeRange = 5;
	int pillarHeight = 3;
	int blockHeight = 3;
	bool Bypass = false;
	bool multiplace = false;
	bool onClick = false;
	bool tryAutoFallBlock(Vec3 AutoFallBlock);

public:

	GravityAura();
	~GravityAura();

	// Inherited via IModule
	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() override;
};
