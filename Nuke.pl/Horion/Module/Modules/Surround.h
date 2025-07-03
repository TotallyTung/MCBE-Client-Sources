#pragma once

#include "Module.h"
#include "../ModuleManager.h"
class Surround : public IModule {
private:

	bool trySurround(Vec3 AutoTrap);

public:
	Surround();
	~Surround();

	bool centertick = true;
	bool centerinf = true;
	bool silent = true;

	bool surround = true;
	bool surroundp = true;
	bool surroundpp = true;
	bool surroundside = true;
	bool surroundbs = true;
	bool surroundac = true;
	bool selfanvil = true;
	bool disable = true;

	bool place = true;


	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(GameMode* gm) override;
};
