#pragma once
#include "../../ModuleBase/Module.h"

class StorageESP : public Module {
public:
	std::unordered_map<BlockPos, UIColor> blockList;
	StorageESP();

public:
	bool trace = true;
	int radius = 64;
	int alpha = 50;
	int tracerAlpha = 255;

	bool showChests = true;
	bool showEnderChests = true;
	bool showShulkers = true;
	bool showBarrels = true;
	bool showHoppers = true;
	bool showFurnaces = true;
	bool showSpawners = true;
	bool showBookshelves = true;
	bool showEnchantingTables = true;
	bool showDecoratedPots = true;
	bool showCobwebs = true;
	bool showDroppers = true;
	bool showDispensers = true;
	bool showAnvils = true;

public:
	void onLevelRender() override;
};
