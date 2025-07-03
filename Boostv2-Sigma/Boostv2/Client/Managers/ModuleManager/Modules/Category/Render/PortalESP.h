#pragma once
#include "../../ModuleBase/Module.h"

class PortalESP : public Module {
public:
	std::unordered_map<BlockPos, UIColor> blockList;
	PortalESP();
public:
	bool trace = true;
	int radius = 64;
	int alpha = 50;
	int Linealpha = 256;
	int tracerAlpha = 255;
public:
	void onLevelRender() override;
};