#include "Surround.h"

Surround::Surround() : IModule(0x0, Category::COMBAT, "Fast") {
	registerBoolSetting("Surround", &surround, surround);
	registerBoolSetting("Surround+", &surroundp, surroundp);
	registerBoolSetting("Surround++", &surroundpp, surroundpp);
	registerBoolSetting("SurroundSide", &surroundside, surroundside);
	registerBoolSetting("SurroundBS", &surroundbs, surroundbs);
	registerBoolSetting("SurroundAC", &surroundac, surroundac);
	registerBoolSetting("SelfAnvil", &selfanvil, selfanvil);
	registerBoolSetting("Center", &centerinf, centerinf);
	registerBoolSetting("Silent", &silent, silent);

}

Surround::~Surround() {
}

const char* Surround::getModuleName() {
	return "FastSurround";
}

bool Surround::trySurround(Vec3 Surround) {
	Surround = Surround.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(Surround));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (/*blockLegacy->material->isReplaceable*/ true == true) {
		Vec3i blok(Surround);

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
			Game.getGameMode()->buildBlock(&blok, i, true);

			return true;
		}
	}
	return false;
}

void getObsidianTS() {
	auto supplies = Game.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(Game.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

void Surround::onTick(GameMode* gm) {
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();

	Vec3 PlayerPos = Vec3(Game.getLocalPlayer()->getPos()->x, Game.getLocalPlayer()->getPos()->y, Game.getLocalPlayer()->getPos()->z);
	Vec3 yR = Vec3(floor(Game.getLocalPlayer()->getPos()->x), Game.getLocalPlayer()->getPos()->y, floor(Game.getLocalPlayer()->getPos()->z));
	yR.x += 0.5f;
	yR.z += 0.5f;
	if (centerinf) {
		Game.getLocalPlayer()->setPos(yR);
	}

	//Surround
	Vec3 blockSideS1 = PlayerPos;
	Vec3 blockSideS2 = PlayerPos;
	Vec3 blockSideS3 = PlayerPos;
	Vec3 blockSideS4 = PlayerPos;

	//SurroundBS
	Vec3 blockSideB1 = PlayerPos;
	Vec3 blockSideB2 = PlayerPos;
	Vec3 blockSideB3 = PlayerPos;
	Vec3 blockSideB4 = PlayerPos;

	//SurroundSide
	Vec3 blockSideSi1 = PlayerPos;
	Vec3 blockSideSi2 = PlayerPos;
	Vec3 blockSideSi3 = PlayerPos;
	Vec3 blockSideSi4 = PlayerPos;

	//SurroundAC
	Vec3 blockSideAC1 = PlayerPos;
	Vec3 blockSideAC2 = PlayerPos;
	Vec3 blockSideAC3 = PlayerPos;
	Vec3 blockSideAC4 = PlayerPos;
	Vec3 blockSideAC5 = PlayerPos;
	Vec3 blockSideAC6 = PlayerPos;
	Vec3 blockSideAC7 = PlayerPos;
	Vec3 blockSideAC8 = PlayerPos;

	//Surround++
	Vec3 blockSideM1 = PlayerPos;
	Vec3 blockSideM2 = PlayerPos;
	Vec3 blockSideM3 = PlayerPos;

	//surround+
	Vec3 blockSideT1 = PlayerPos;
	Vec3 blockSideT2 = PlayerPos;

	//Surround
	blockSideS1.x = PlayerPos.x + 1;
	blockSideS1.z = PlayerPos.z - 0;
	blockSideS1.y = PlayerPos.y - 1;
	auto blockSideS1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideS1))->toLegacy()->blockId;

	blockSideS2.x = PlayerPos.x - 1;
	blockSideS2.z = PlayerPos.z - 0;
	blockSideS2.y = PlayerPos.y - 1;
	auto blockSideS2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideS2))->toLegacy()->blockId;

	blockSideS3.x = PlayerPos.x - 0;
	blockSideS3.z = PlayerPos.z + 1;
	blockSideS3.y = PlayerPos.y - 1;
	auto blockSideS3Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideS3))->toLegacy()->blockId;

	blockSideS4.x = PlayerPos.x - 0;
	blockSideS4.z = PlayerPos.z - 1;
	blockSideS4.y = PlayerPos.y - 1;
	auto blockSideS4Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideS4))->toLegacy()->blockId;

	//SurroundBS
	blockSideB1.x = PlayerPos.x + 1;
	blockSideB1.z = PlayerPos.z + 1;
	blockSideB1.y = PlayerPos.y - 1;
	auto blockSideB1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideB1))->toLegacy()->blockId;

	blockSideB2.x = PlayerPos.x - 1;
	blockSideB2.z = PlayerPos.z - 1;
	blockSideB2.y = PlayerPos.y - 1;
	auto blockSideB2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideB2))->toLegacy()->blockId;

	blockSideB3.x = PlayerPos.x + 1;
	blockSideB3.z = PlayerPos.z - 1;
	blockSideB3.y = PlayerPos.y - 1;
	auto blockSideB3Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideB3))->toLegacy()->blockId;

	blockSideB4.x = PlayerPos.x - 1;
	blockSideB4.z = PlayerPos.z + 1;
	blockSideB4.y = PlayerPos.y - 1;
	auto blockSideB4Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideB4))->toLegacy()->blockId;

	//SurroundSide
	blockSideSi1.x = PlayerPos.x + 2;
	blockSideSi1.z = PlayerPos.z - 0;
	blockSideSi1.y = PlayerPos.y - 1;
	auto blockSideSi1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideSi1))->toLegacy()->blockId;

	blockSideSi2.x = PlayerPos.x - 2;
	blockSideSi2.z = PlayerPos.z - 0;
	blockSideSi2.y = PlayerPos.y - 1;
	auto blockSideSi2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideSi2))->toLegacy()->blockId;

	blockSideSi3.x = PlayerPos.x - 0;
	blockSideSi3.z = PlayerPos.z + 2;
	blockSideSi3.y = PlayerPos.y - 1;
	auto blockSideSi3Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideSi3))->toLegacy()->blockId;

	blockSideSi4.x = PlayerPos.x - 0;
	blockSideSi4.z = PlayerPos.z - 2;
	blockSideSi4.y = PlayerPos.y - 1;
	auto blockSideSi4Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideSi4))->toLegacy()->blockId;

	//SurroundAC
	blockSideAC1.x = PlayerPos.x + 2;
	blockSideAC1.z = PlayerPos.z + 1;
	blockSideAC1.y = PlayerPos.y - 1;
	auto blockSideAC1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC1))->toLegacy()->blockId;

	blockSideAC2.x = PlayerPos.x + 2;
	blockSideAC2.z = PlayerPos.z - 1;
	blockSideAC2.y = PlayerPos.y - 1;
	auto blockSideAC2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC2))->toLegacy()->blockId;

	blockSideAC3.x = PlayerPos.x + 1;
	blockSideAC3.z = PlayerPos.z + 2;
	blockSideAC3.y = PlayerPos.y - 1;
	auto blockSideAC3Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC3))->toLegacy()->blockId;

	blockSideAC4.x = PlayerPos.x - 1;
	blockSideAC4.z = PlayerPos.z + 2;
	blockSideAC4.y = PlayerPos.y - 1;
	auto blockSideAC4Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC4))->toLegacy()->blockId;
	//
	blockSideAC5.x = PlayerPos.x + 1;
	blockSideAC5.z = PlayerPos.z - 2;
	blockSideAC5.y = PlayerPos.y - 1;
	auto blockSideAC5Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC5))->toLegacy()->blockId;

	blockSideAC8.x = PlayerPos.x - 2;
	blockSideAC8.z = PlayerPos.z - 1;
	blockSideAC8.y = PlayerPos.y - 1;
	auto blockSideAC8Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC8))->toLegacy()->blockId;

	blockSideAC7.x = PlayerPos.x - 2;
	blockSideAC7.z = PlayerPos.z + 1;
	blockSideAC7.y = PlayerPos.y - 1;
	auto blockSideAC7Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC7))->toLegacy()->blockId;

	blockSideAC6.x = PlayerPos.x - 1;
	blockSideAC6.z = PlayerPos.z - 2;
	blockSideAC6.y = PlayerPos.y - 1;
	auto blockSideAC6Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideAC6))->toLegacy()->blockId;

	//Surround++
	blockSideM1.x = PlayerPos.x - 0;
	blockSideM1.z = PlayerPos.z - 1;
	blockSideM1.y = PlayerPos.y - 0;
	auto blockSideM1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideM1))->toLegacy()->blockId;

	blockSideM2.x = PlayerPos.x - 0;
	blockSideM2.z = PlayerPos.z - 1;
	blockSideM2.y = PlayerPos.y + 1;
	auto blockSideM2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideM2))->toLegacy()->blockId;

	blockSideM3.x = PlayerPos.x - 0;
	blockSideM3.z = PlayerPos.z - 1;
	blockSideM3.y = PlayerPos.y + 2;
	auto blockSideM3Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideM3))->toLegacy()->blockId;

	//Surround+
	blockSideT1.x = PlayerPos.x - 0;
	blockSideT1.z = PlayerPos.z - 0;
	blockSideT1.y = PlayerPos.y + 2;
	auto blockSideT1Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideT1))->toLegacy()->blockId;

	blockSideT2.x = PlayerPos.x - 0;
	blockSideT2.z = PlayerPos.z - 0;
	blockSideT2.y = PlayerPos.y + 1;
	auto blockSideT2Id = Game.getLocalPlayer()->region->getBlock(Vec3i(blockSideT2))->toLegacy()->blockId;
	int slotab = 0;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	if (silent) {
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
	}
	if (surround) {
		if (blockSideS1Id == 0) {
			getObsidianTS();
			//clientMessageF("S1");
			if (!trySurround(blockSideS1)) {
				blockSideS1.x = 0;
				blockSideS1.y = 0;
				blockSideS1.z = 0;
			}
		}

		if (blockSideS2Id == 0) {
			getObsidianTS();
			//clientMessageF("S2");
			if (!trySurround(blockSideS2)) {
				blockSideS2.x = 0;
				blockSideS2.y = 0;
				blockSideS2.z = 0;
			}
		}

		if (blockSideS3Id == 0) {
			getObsidianTS();
			//clientMessageF("S3");
			if (!trySurround(blockSideS3)) {
				blockSideS3.x = 0;
				blockSideS3.y = 0;
				blockSideS3.z = 0;
			}
		}

		if (blockSideS4Id == 0) {
			getObsidianTS();
			//clientMessageF("S4");
			if (!trySurround(blockSideS4)) {
				blockSideS4.x = 0;
				blockSideS4.y = 0;
				blockSideS4.z = 0;
			}
		}
	}

	if (surroundbs) {
		if (blockSideB1Id == 0) {
			getObsidianTS();
			//clientMessageF("B1");
			if (!trySurround(blockSideB1)) {
				blockSideB1.x = 0;
				blockSideB1.y = 0;
				blockSideB1.z = 0;
			}
		}

		if (blockSideB2Id == 0) {
			getObsidianTS();
			//clientMessageF("B2");
			if (!trySurround(blockSideB2)) {
				blockSideB2.x = 0;
				blockSideB2.y = 0;
				blockSideB2.z = 0;
			}
		}

		if (blockSideB3Id == 0) {
			getObsidianTS();
			//clientMessageF("B3");
			if (blockSideB3Id == 0) {
				if (!trySurround(blockSideB3)) {
					blockSideB3.x = 0;
					blockSideB3.y = 0;
					blockSideB3.z = 0;
				}
			}
		}

		if (blockSideB4Id == 0) {
			getObsidianTS();
			//clientMessageF("B4");
			if (blockSideB4Id == 0) {
				if (!trySurround(blockSideB4)) {
					blockSideB4.x = 0;
					blockSideB4.y = 0;
					blockSideB4.z = 0;
				}
			}
		}
	}

	if (surroundside) {
		if (blockSideSi1Id == 0) {
			getObsidianTS();
			//clientMessageF("Si1");
			if (!trySurround(blockSideSi1)) {
				blockSideSi1.x = 0;
				blockSideSi1.y = 0;
				blockSideSi1.z = 0;
			}
		}

		if (blockSideSi2Id == 0) {
			getObsidianTS();
			//clientMessageF("Si2");
			if (!trySurround(blockSideSi2)) {
				blockSideSi2.x = 0;
				blockSideSi2.y = 0;
				blockSideSi2.z = 0;
			}
		}

		if (blockSideSi3Id == 0) {
			getObsidianTS();
			//clientMessageF("Si3");
			if (!trySurround(blockSideSi3)) {
				blockSideSi3.x = 0;
				blockSideSi3.y = 0;
				blockSideSi3.z = 0;
			}
		}

		if (blockSideSi4Id == 0) {
			getObsidianTS();
			//clientMessageF("Si4");
			if (!trySurround(blockSideSi4)) {
				blockSideSi4.x = 0;
				blockSideSi4.y = 0;
				blockSideSi4.z = 0;
			}
		}
	}

	if (surroundac) {
		if (blockSideAC1Id == 0) {
			getObsidianTS();
			//clientMessageF("AC1");
			if (!trySurround(blockSideAC1)) {
				blockSideAC1.x = 0;
				blockSideAC1.y = 0;
				blockSideAC1.z = 0;
			}
		}

		if (blockSideAC2Id == 0) {
			getObsidianTS();
			//clientMessageF("AC2");
			if (!trySurround(blockSideAC2)) {
				blockSideAC2.x = 0;
				blockSideAC2.y = 0;
				blockSideAC2.z = 0;
			}
		}

		if (blockSideAC3Id == 0) {
			getObsidianTS();
			//clientMessageF("AC3");
			if (!trySurround(blockSideAC3)) {
				blockSideAC3.x = 0;
				blockSideAC3.y = 0;
				blockSideAC3.z = 0;
			}
		}

		if (blockSideAC4Id == 0) {
			getObsidianTS();
			//clientMessageF("AC4");
			if (!trySurround(blockSideAC4)) {
				blockSideAC4.x = 0;
				blockSideAC4.y = 0;
				blockSideAC4.z = 0;
			}
		}

		if (blockSideAC5Id == 0) {
			getObsidianTS();
			//clientMessageF("AC5");
			if (!trySurround(blockSideAC5)) {
				blockSideAC5.x = 0;
				blockSideAC5.y = 0;
				blockSideAC5.z = 0;
			}
		}

		if (blockSideAC6Id == 0) {
			getObsidianTS();
			//clientMessageF("AC6");
			if (!trySurround(blockSideAC6)) {
				blockSideAC6.x = 0;
				blockSideAC6.y = 0;
				blockSideAC6.z = 0;
			}
		}

		if (blockSideAC7Id == 0) {
			getObsidianTS();
			//clientMessageF("AC7");
			if (!trySurround(blockSideAC7)) {
				blockSideAC7.x = 0;
				blockSideAC7.y = 0;
				blockSideAC7.z = 0;
			}
		}

		if (blockSideAC8Id == 0) {
			getObsidianTS();
			//clientMessageF("AC8");
			if (!trySurround(blockSideAC8)) {
				blockSideAC8.x = 0;
				blockSideAC8.y = 0;
				blockSideAC8.z = 0;
			}
		}

	}

	if (surroundpp) {
		if (blockSideM1Id == 0) {
			getObsidianTS();
			//clientMessageF("M1");
			if (!trySurround(blockSideM1)) {
				blockSideM1.x = 0;
				blockSideM1.y = 0;
				blockSideM1.z = 0;
			}
		}

		if (blockSideM2Id == 0) {
			getObsidianTS();
			//clientMessageF("M2");
			if (!trySurround(blockSideM2)) {
				blockSideM2.x = 0;
				blockSideM2.y = 0;
				blockSideM2.z = 0;
			}
		}

		if (blockSideM3Id == 0) {
			getObsidianTS();
			//clientMessageF("M3");
			if (!trySurround(blockSideM3)) {
				blockSideM3.x = 0;
				blockSideM3.y = 0;
				blockSideM3.z = 0;
			}
		}

	}

	if (surroundp) {
		if (blockSideT1Id == 0) {
			getObsidianTS();
			//clientMessageF("T1");
			if (!trySurround(blockSideT1)) {
				blockSideT1.x = 0;
				blockSideT1.y = 0;
				blockSideT1.z = 0;
			}
		}

		if (blockSideT2Id == 0) {
			if (!selfanvil) {
				getObsidianTS();
				//clientMessageF("T2");
				if (!trySurround(blockSideT2)) {
					blockSideT2.x = 0;
					blockSideT2.y = 0;
					blockSideT2.z = 0;
				}
			}
		}

	}
	if (selfanvil) {
		bool place = true;
		Vec3 blockPos = Vec3(Game.getLocalPlayer()->getPos()->x, Game.getLocalPlayer()->getPos()->y - 1, Game.getLocalPlayer()->getPos()->z);
		Block* block = Game.getLocalPlayer()->region->getBlock(blockPos);
		int blockId = block->toLegacy()->blockId;

		Vec3 blockPos2 = Vec3(Game.getLocalPlayer()->getPos()->x, Game.getLocalPlayer()->getPos()->y + 1, Game.getLocalPlayer()->getPos()->z);
		Block* block2 = Game.getLocalPlayer()->region->getBlock(blockPos2);
		int blockId2 = block2->toLegacy()->blockId;

		if (blockSideT2Id == 0) {
			if (blockId == 0 && blockId2 == 0) {
				for (int n = 0; n < 9; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 145) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
				//clientMessageF("T2Anvil");
				if (!trySurround(blockSideT2)) {
					blockSideT2.x = 0;
					blockSideT2.y = 0;
					blockSideT2.z = 0;
				}


			}
			if (blockId != 0) {
				for (int n = 0; n < 9; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						if (stack->getItem()->itemId == 49) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
				//clientMessageF("T2Anvil-Pas");
				if (!trySurround(blockSideT2)) {
					blockSideT2.x = 0;
					blockSideT2.y = 0;
					blockSideT2.z = 0;
				}
			}
		}

	}
	if (silent) {
		supplies->selectedHotbarSlot = slotab;
	};
}