#pragma once

#include "../ModuleManager.h"
#include "Module.h"

class CityESP : public IModule {
private:
	bool showExposed = true;
	bool inclSelf = true;

	bool oneDot12 = true;

	int expR = 255;
	int expG = 50;
	int expB = 0;
	float expT = 0.4f;

	int cityR = 0;
	int cityG = 255;
	int cityB = 50;
	float cityT = 0.4f;

public:
	std::vector<vec3_ti> highlightCity;
	std::vector<vec3_t> exposee;

	bool isBlockAGoodCity(vec3_ti* blk, vec3_ti* personPos);

	float range = 10.f;
	CityESP();
	~CityESP();
	// Inherited via IModule
	virtual const char* getModuleName() override;
	virtual void onWorldTick(C_GameMode* gm) override;
	virtual void onPreRender(C_MinecraftUIRenderContext* renderCtx) override;
};