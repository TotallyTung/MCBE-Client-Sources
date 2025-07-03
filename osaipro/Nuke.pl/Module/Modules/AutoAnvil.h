#pragma once
#include "Module.h"
#include "../ModuleManager.h"
#include "../../../Utils/Target.h"
class AutoAnvil : public IModule {
private:
	int obsiheight = 3;
	int fallblockheight = 3;
	bool Bypass = true;
	SettingEnum BlockID = this;
	bool tryAutoAnvil(Vec3 AutoAnvil);

public:
	AutoAnvil();
	~AutoAnvil();

	int range = 10;
	bool multiplace = false;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
	virtual void onJoiningServer() override;
};
