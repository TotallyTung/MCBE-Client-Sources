#pragma once
#include "../../ModuleBase/Module.h"

class ChunkBorders : public Module {
public:
	bool* showChunkBorderBoolPtr = nullptr;
	ChunkBorders();

	void onDisable() override;
	void onClientTick() override;
};