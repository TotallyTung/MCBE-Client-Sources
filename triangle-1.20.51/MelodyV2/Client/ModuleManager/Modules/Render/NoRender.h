#pragma once
#include "../Module.h"

class NoRender : public Module {
public:
	bool noFire = true;
	bool noWeather = true;
	bool noEntities = false;
	bool noBlockEntities = false;
	bool noParticles = false;
	bool noSky = false;
	bool NoHUD = false;
	bool Noiteminhand = false;
	bool NoTerrain = false;
	bool NoMainMenuPaperdollAnimation = false;
	bool NoMainMenuCubeMap = false;
	NoRender();
};