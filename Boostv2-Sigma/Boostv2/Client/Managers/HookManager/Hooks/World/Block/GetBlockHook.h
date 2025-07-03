#pragma once
#include "../../FuncHook.h"
#include <iostream>
#include <string>

class GetBlockHook : public FuncHook {
private:
	using func_t = Block * (__thiscall*)(BlockSource*, const BlockPos&);
	static inline func_t oFunc;

	static Block* BlockSourceGetBlockCallback(BlockSource* _this, const BlockPos& blockPos) {
		Block* result = oFunc(_this, blockPos);
		static PortalESP* portalEsp = ModuleManager::getModule<PortalESP>();
		static StorageESP* storageEsp = ModuleManager::getModule<StorageESP>();

		if (storageEsp->isEnabled()) {
			int blockId = result->blockLegacy->blockId;
			std::string blockName = result->blockLegacy->blockName;

			if (result->blockLegacy && !result->blockLegacy->blockName.empty()) {
				if (storageEsp->showChests && (blockId == 54 || blockId == 146)) { // Chest
					storageEsp->blockList[blockPos] = UIColor(255, 165, 0);
				}
				else if (storageEsp->showEnderChests && blockId == 130) { // Ender Chest
					storageEsp->blockList[blockPos] = UIColor(255, 0, 255);
				}
				else if (storageEsp->showShulkers && blockName.find("shulker") != std::string::npos) { // Shulker Box
					storageEsp->blockList[blockPos] = UIColor(255, 255, 255);
				}
				else if (storageEsp->showBarrels && blockId == 458) { // Barrel
					storageEsp->blockList[blockPos] = UIColor(155, 75, 0);
				}
				else if (storageEsp->showHoppers && blockId == 154) { // Hopper
					storageEsp->blockList[blockPos] = UIColor(128, 128, 128);
				}
				else if (storageEsp->showFurnaces && (blockId == 61 || blockId == 451 || blockId == 453)) { // Furnace
					storageEsp->blockList[blockPos] = UIColor(70, 70, 70);
				}
				else if (storageEsp->showSpawners && blockId == 52) { // Spawner
					storageEsp->blockList[blockPos] = UIColor(0, 0, 0);
				}
				else if (storageEsp->showBookshelves && blockId == 47) { // Bookshelf
					storageEsp->blockList[blockPos] = UIColor(139, 69, 19);
				}
				else if (storageEsp->showEnchantingTables && blockId == 116) { // Enchanting Table
					storageEsp->blockList[blockPos] = UIColor(0, 0, 255);
				}
				else if (storageEsp->showDecoratedPots && blockName.find("decorated_pot") != std::string::npos) { // Decorated Pot
					storageEsp->blockList[blockPos] = UIColor(255, 0, 0);
				}
				else if (storageEsp->showCobwebs && blockName.find("web") != std::string::npos) { // Cobweb
					storageEsp->blockList[blockPos] = UIColor(128, 128, 128);
				}
				else if (storageEsp->showDroppers && blockName.find("dropper") != std::string::npos) { // Dropper
					storageEsp->blockList[blockPos] = UIColor(192, 192, 192);
				}
				else if (storageEsp->showDispensers && blockId == 23) { // Dispenser
					storageEsp->blockList[blockPos] = UIColor(169, 169, 169);
				}
				else if (storageEsp->showAnvils && blockId == 145) { // Anvil
					storageEsp->blockList[blockPos] = UIColor(105, 105, 105);
				}
			}
		}

		if (portalEsp->isEnabled()) {
			if (result->blockLegacy && !result->blockLegacy->blockName.empty()) {
				if (result->blockLegacy->blockName.find("portal") != std::string::npos) { // Portal
					portalEsp->blockList[blockPos] = UIColor(200, 0, 255);
				}
			}
		}
		return result;
	}
public:
	GetBlockHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&BlockSourceGetBlockCallback;
	}
};
