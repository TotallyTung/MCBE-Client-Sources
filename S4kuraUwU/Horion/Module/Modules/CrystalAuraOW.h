#pragma once
#include "Module.h"
class CrystalAuraOW : public IModule {
private:
	bool tryCrystalAuraOW(vec3_t CrystalAuraOW);

public:
	CrystalAuraOW();
	~CrystalAuraOW();

	int range = 17;
	bool spoof = false;
	bool onClick = false;
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onTick(C_GameMode* gm) override;
};
#pragma once
