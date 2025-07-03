#pragma once

#include "../../ModuleBase/Module.h"

class Nuker : public Module {
private:
	int delay = 0;
	int nukerRadius = 4;
	bool veinMiner = false;
	bool autodestroy = true;

public:
	Nuker();
	~Nuker();

	inline bool isAutoMode() { return autodestroy; };
	int getNukerRadius() { return nukerRadius; };
	inline bool isVeinMiner() { return veinMiner; };

	// Inherited via IModule
	virtual void onNormalTick(LocalPlayer* gm) override;
};
