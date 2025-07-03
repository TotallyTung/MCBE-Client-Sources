#pragma once
#include "../../ModuleBase/Module.h"

class NoRender : public Module {
public:
	bool noFire = false;
	bool noWeather = false;
	bool noEntities = false;
	bool noBlockEntities = false;
	bool noParticles = false;
public:
	bool* noWeatherBoolPtr = nullptr;
	bool* noEntitiesBoolPtr = nullptr;
	bool* noBlockEntitiesBoolPtr = nullptr;
	bool* noParticlesBoolPtr = nullptr;

	NoRender();

	void onDisable() override;
	void onClientTick() override;
};