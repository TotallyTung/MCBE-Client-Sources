#include "GravityAura.h"
GravityAura::GravityAura() : IModule(0, Category::PLAYER, "Places gravity blocks like anvils and sand on people.") {
	registerEnumSetting("Block", &block, 1);
	block.addEntry("Anvil", 1);
	block.addEntry("Sand", 2);
	block.addEntry("Gravel", 3);
	block.addEntry("Powder", 4);
	registerIntSetting("Support Height", &pillarHeight, pillarHeight, 3, 8);
	registerIntSetting("Fall Block Height", &blockHeight, blockHeight, 3, 8);
	registerIntSetting("Place Range", &placeRange, placeRange, 3, 12);
	registerBoolSetting("Spam", &multiplace, multiplace);
	registerBoolSetting("On Click", &onClick, onClick);
	registerBoolSetting("Strict", &Bypass, Bypass);
}
GravityAura::~GravityAura() {}
const char* GravityAura::getModuleName() {return ("GravityAura");}
bool GravityAura::tryAutoFallBlock(Vec3 AutoFallBlock) {
	AutoFallBlock = AutoFallBlock.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(AutoFallBlock));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(AutoFallBlock);

		// Find neighbour
		static std::vector<Vec3i*> checklist;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

		bool foundCandidate = false;
		int i = 0;
		for (auto current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			Game.getGameMode()->buildBlock(&blok, i, false);

			return true;
		}
	}
	return false;
}
void GravityAura::findEntity(Entity* currentEntity, bool isRegularEntity) {
	static GravityAura* gravityAuraMod = moduleMgr->getModule<GravityAura>();
	if (!currentEntity->isAlive()) return;
	if (!Target::isValidTarget(currentEntity)) return;
	if (currentEntity->eyePos0.dist(Game.getLocalPlayer()->eyePos0) > gravityAuraMod->placeRange) return;
	gravityAuraMod->entityList.push_back(currentEntity);
}
void GravityAura::onTick(GameMode* gm) {
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	if (pillarHeight < blockHeight) {
		pillarHeight = blockHeight;
	}
	if (Bypass) {
		if (blockHeight > 7) {
			blockHeight = 6;
		}
		if (pillarHeight > 7) {
			pillarHeight = 6;
		}
	}

	entityList.clear();
	Game.forEachEntity(findEntity);

	int place = 1;

	if (onClick) {
		if (GameData::isRightClickDown()) {
			place = 0;
		} else {
			place = 1;
		}
	}

	if (!onClick) {
		place = 0;
	}

	if (!entityList.empty()) {
		Vec3 blockAbove0 = entityList[0]->eyePos0;
		Vec3 blockAbove1 = entityList[0]->eyePos0;
		Vec3 blockAbove2 = entityList[0]->eyePos0;
		Vec3 blockAbove3 = entityList[0]->eyePos0;
		Vec3 blockAbove4 = entityList[0]->eyePos0;
		Vec3 blockAbove5 = entityList[0]->eyePos0;
		Vec3 blockAboveH0 = entityList[0]->eyePos0;
		Vec3 blockAboveH1 = entityList[0]->eyePos0;
		Vec3 blockAboveH2 = entityList[0]->eyePos0;
		Vec3 blockAboveH3 = entityList[0]->eyePos0;
		Vec3 blockAboveH4 = entityList[0]->eyePos0;
		Vec3 blockAboveH5 = entityList[0]->eyePos0;
		Vec3 blockAboveH6 = entityList[0]->eyePos0;
		Vec3 blockAboveH7 = entityList[0]->eyePos0;
		Vec3 blockAboveH8 = entityList[0]->eyePos0;
		// head 0
		blockAboveH8.y -= entityList[0]->height - 0;
		blockAboveH8.x -= entityList[0]->height - 1;
		blockAboveH8.z -= entityList[0]->height - 2;
		// head 1
		blockAboveH0.y -= entityList[0]->height - 1;
		blockAboveH0.x -= entityList[0]->height - 1;
		blockAboveH0.z -= entityList[0]->height - 2;
		// head 2
		blockAboveH1.y -= entityList[0]->height - 2;
		blockAboveH1.x -= entityList[0]->height - 1;
		blockAboveH1.z -= entityList[0]->height - 2;
		// head 3
		blockAboveH2.y -= entityList[0]->height - 3;
		blockAboveH2.x -= entityList[0]->height - 1;
		blockAboveH2.z -= entityList[0]->height - 2;
		// head 4
		blockAboveH3.y -= entityList[0]->height - 4;
		blockAboveH3.x -= entityList[0]->height - 1;
		blockAboveH3.z -= entityList[0]->height - 2;
		// head 5
		blockAboveH4.y -= entityList[0]->height - 5;
		blockAboveH4.x -= entityList[0]->height - 1;
		blockAboveH4.z -= entityList[0]->height - 2;
		// head 6
		blockAboveH5.y -= entityList[0]->height - 6;
		blockAboveH5.x -= entityList[0]->height - 1;
		blockAboveH5.z -= entityList[0]->height - 2;
		// head 7
		blockAboveH6.y -= entityList[0]->height - 7;
		blockAboveH6.x -= entityList[0]->height - 1;
		blockAboveH6.z -= entityList[0]->height - 2;
		// head 8
		blockAboveH7.y -= entityList[0]->height - 8;
		blockAboveH7.x -= entityList[0]->height - 1;
		blockAboveH7.z -= entityList[0]->height - 2;
		// Anvil 3
		blockAbove0.y -= entityList[0]->height - 3;
		// Anvil 4
		blockAbove1.y -= entityList[0]->height - 4;
		// Anvil 5
		blockAbove2.y -= entityList[0]->height - 5;
		// Anvil 6
		blockAbove3.y -= entityList[0]->height - 6;
		// Anvil 7
		blockAbove4.y -= entityList[0]->height - 7;
		// Anvil 8
		blockAbove5.y -= entityList[0]->height - 8;

		if (place == 0) {
			// Silent Code
			int slotab = 0;
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			Inventory* inv = supplies->inventory;
			slotab = supplies->selectedHotbarSlot;
			for (int n = 0; n < 9; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItem()->itemId == 49) {
						supplies->selectedHotbarSlot = n;
						break;
					}
				}
			}
			if (pillarHeight == 3) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
			}
			if (pillarHeight == 4) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
			}
			if (pillarHeight == 5) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
			}
			if (pillarHeight == 6) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
			}
			if (pillarHeight == 7) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH6)) {
					blockAboveH6.x -= 0;
					blockAboveH6.z -= 0;
				}
			}
			if (pillarHeight == 8) {
				if (!tryAutoFallBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH6)) {
					blockAboveH6.x -= 0;
					blockAboveH6.z -= 0;
				}
				if (!tryAutoFallBlock(blockAboveH7)) {
					blockAboveH7.x -= 0;
					blockAboveH7.z -= 0;
				}
			}
			supplies->selectedHotbarSlot = slotab;
			slotab = 0;
			slotab = supplies->selectedHotbarSlot;
			for (int n = 0; n < 9; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (block.getSelectedValue() == 1) {
						if (stack->getItem()->itemId == 145) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
					if (block.getSelectedValue() == 2) {
						if (stack->getItem()->itemId == 12) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
					if (block.getSelectedValue() == 3) {
						if (stack->getItem()->itemId == 13) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
					if (block.getSelectedValue() == 4) {
						if (stack->getItem()->itemId == 252) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
			}
			if (blockHeight == 3) {
				if (!tryAutoFallBlock(blockAbove0)) {
					blockAbove0.y -= 0;
				}
			}
			if (blockHeight == 4) {
				if (!tryAutoFallBlock(blockAbove1)) {
					blockAbove1.y -= 0;
				}
				if (multiplace) {
					if (!tryAutoFallBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
				}
			}
			if (blockHeight == 5) {
				if (!tryAutoFallBlock(blockAbove2)) {
					blockAbove2.y -= 0;
				}
				if (multiplace) {
					if (!tryAutoFallBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
				}
			}
			if (blockHeight == 6) {
				if (!tryAutoFallBlock(blockAbove3)) {
					blockAbove3.y -= 0;
				}
				if (multiplace) {
					if (!tryAutoFallBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove2)) {
						blockAbove1.y -= 0;
					}
				}
			}
			if (blockHeight == 7) {
				if (!tryAutoFallBlock(blockAbove4)) {
					blockAbove4.y -= 0;
				}
				if (multiplace) {
					if (!tryAutoFallBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove2)) {
						blockAbove2.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove3)) {
						blockAbove2.y -= 0;
					}
				}
			}
			if (blockHeight == 8) {
				if (!tryAutoFallBlock(blockAbove5)) {
					blockAbove5.y -= 0;
				}
				if (multiplace) {
					if (!tryAutoFallBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove2)) {
						blockAbove2.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove3)) {
						blockAbove3.y -= 0;
					}
					if (!tryAutoFallBlock(blockAbove4)) {
						blockAbove4.y -= 0;
					}
				}
			}
			supplies->selectedHotbarSlot = slotab;
		}
	}
}