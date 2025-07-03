#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class FullAutoPiston : public IModule {
public:
	FullAutoPiston();
	~FullAutoPiston() {};

	bool Xp = true;
	bool Xm = true;
	bool Zp = true;
	bool Zm = true;

	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm);
	virtual void onJoiningServer();

};
#pragma once
