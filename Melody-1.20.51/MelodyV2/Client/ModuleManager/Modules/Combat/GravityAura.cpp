// Created by Tony on 2024-10-10 10:01:54

#include "GravityAura.h"
#include <algorithm>
#include <vector>
#include <ranges>
#include <mutex>
#include <omp.h>
/*

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
*/
GravityAura::GravityAura() : Module("GravityAura", "Places gravity blocks like anvils and sand on people.", Category::COMBAT, 0x0) {

	addEnumSetting("Block", "Blockstypes", { "Anvil", "Sand" ,"Gravel","Powder"}, &block);
	addSlider<int>("Support Height", "NULL", ValueType::INT_T, &pillarHeight, 3, 8);
	addSlider<int>("Fall Block Height", "NULL", ValueType::INT_T, &blockHeight, 3, 8);
	addSlider<int>("Place Range", "NULL", ValueType::INT_T, &placeRange, 3, 12);

	addBoolCheck("Spam", "NULL", &multiplace);
	addBoolCheck("Strict", "NULL", &Bypass);
	addBoolCheck("Testing", "NULL", &Mobs);
}

void GravityAura::onNormalTick(Actor* actor) {
	auto* gm = mc.getGameMode();
	auto* localPlayer = mc.getLocalPlayer();
	auto* level = localPlayer->getLevel();
	if (localPlayer == nullptr) return;
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
	for (auto* target : level->getRuntimeActorList()) {
		if (TargetUtils::isTargetValid(target, Mobs) &&
			localPlayer->stateVectorComponent->pos.dist(target->stateVectorComponent->pos) < placeRange) {
			targetList.push_back(target);
		}
	}

	int place = 1;
	if (!targetList.empty()) {
		auto blockAbove0 = targetList[0]->getEyePos().toInt();
		auto blockAbove1 = targetList[0]->getEyePos().toInt();
		auto blockAbove2 = targetList[0]->getEyePos().toInt();
		auto blockAbove3 = targetList[0]->getEyePos().toInt();
		auto blockAbove4 = targetList[0]->getEyePos().toInt();
		auto blockAbove5 = targetList[0]->getEyePos().toInt();
		auto blockAboveH0 = targetList[0]->getEyePos().toInt();
		auto blockAboveH1 = targetList[0]->getEyePos().toInt();
		auto blockAboveH2 = targetList[0]->getEyePos().toInt();
		auto blockAboveH3 = targetList[0]->getEyePos().toInt();
		auto blockAboveH4 = targetList[0]->getEyePos().toInt();
		auto blockAboveH5 = targetList[0]->getEyePos().toInt();
		auto blockAboveH6 = targetList[0]->getEyePos().toInt();
		auto blockAboveH7 = targetList[0]->getEyePos().toInt();
		auto blockAboveH8 = targetList[0]->getEyePos().toInt();
		// head 0
		blockAboveH8.y -= targetList[0]->getHumanPos().y - 0;
		blockAboveH8.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH8.z -= targetList[0]->getHumanPos().y - 2;
		// head 1
		blockAboveH0.y -= targetList[0]->getHumanPos().y - 1;
		blockAboveH0.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH0.z -= targetList[0]->getHumanPos().y - 2;
		// head 2
		blockAboveH1.y -= targetList[0]->getHumanPos().y - 2;
		blockAboveH1.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH1.z -= targetList[0]->getHumanPos().y - 2;
		// head 3
		blockAboveH2.y -= targetList[0]->getHumanPos().y - 3;
		blockAboveH2.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH2.z -= targetList[0]->getHumanPos().y - 2;
		// head 4
		blockAboveH3.y -= targetList[0]->getHumanPos().y - 4;
		blockAboveH3.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH3.z -= targetList[0]->getHumanPos().y - 2;
		// head 5
		blockAboveH4.y -= targetList[0]->getHumanPos().y - 5;
		blockAboveH4.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH4.z -= targetList[0]->getHumanPos().y - 2;
		// head 6
		blockAboveH5.y -= targetList[0]->getHumanPos().y - 6;
		blockAboveH5.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH5.z -= targetList[0]->getHumanPos().y - 2;
		// head 7
		blockAboveH6.y -= targetList[0]->getHumanPos().y - 7;
		blockAboveH6.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH6.z -= targetList[0]->getHumanPos().y - 2;
		// head 8
		blockAboveH7.y -= targetList[0]->getHumanPos().y - 8;
		blockAboveH7.x -= targetList[0]->getHumanPos().y - 1;
		blockAboveH7.z -= targetList[0]->getHumanPos().y - 2;
		// Anvil 3
		blockAbove0.y -= targetList[0]->getHumanPos().y - 3;
		// Anvil 4
		blockAbove1.y -= targetList[0]->getHumanPos().y - 4;
		// Anvil 5
		blockAbove2.y -= targetList[0]->getHumanPos().y - 5;
		// Anvil 6
		blockAbove3.y -= targetList[0]->getHumanPos().y - 6;
		// Anvil 7
		blockAbove4.y -= targetList[0]->getHumanPos().y - 7;
		// Anvil 8
		blockAbove5.y -= targetList[0]->getHumanPos().y - 8;

		if (place == 0) {
			// Silent Code
			int slotab = 0;
			auto* supplies = localPlayer->getPlayerInventory();
			auto* inv = supplies->inventory;
			slotab = supplies->selectedSlot;
			for (int n = 0; n < 9; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (stack->getItemPtr()->itemId == 49) {
						supplies->selectedSlot = n;
						break;
					}
				}
			}
			if (pillarHeight == 3) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
			}
			if (pillarHeight == 4) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
			}
			if (pillarHeight == 5) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
			}
			if (pillarHeight == 6) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
			}
			if (pillarHeight == 7) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH6)) {
					blockAboveH6.x -= 0;
					blockAboveH6.z -= 0;
				}
			}
			if (pillarHeight == 8) {
				if (!gm->tryPlaceBlock(blockAboveH8)) {
					blockAboveH8.x -= 0;
					blockAboveH8.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH0)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH1)) {
					blockAboveH1.x -= 0;
					blockAboveH1.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH2)) {
					blockAboveH2.x -= 0;
					blockAboveH2.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH3)) {
					blockAboveH3.x -= 0;
					blockAboveH3.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH4)) {
					blockAboveH4.x -= 0;
					blockAboveH4.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH5)) {
					blockAboveH5.x -= 0;
					blockAboveH5.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH6)) {
					blockAboveH6.x -= 0;
					blockAboveH6.z -= 0;
				}
				if (!gm->tryPlaceBlock(blockAboveH7)) {
					blockAboveH7.x -= 0;
					blockAboveH7.z -= 0;
				}
			}
			supplies->selectedSlot = slotab;
			slotab = 0;
			slotab = supplies->selectedSlot;
			for (int n = 0; n < 9; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (block == 1) {
						if (stack->getItemPtr()->itemId == 145) {
							supplies->selectedSlot = n;
							break;
						}
					}
					if (block == 2) {
						if (stack->getItemPtr()->itemId == 12) {
							supplies->selectedSlot = n;
							break;
						}
					}
					if (block == 3) {
						if (stack->getItemPtr()->itemId == 13) {
							supplies->selectedSlot = n;
							break;
						}
					}
					if (block == 4) {
						if (stack->getItemPtr()->itemId == 252) {
							supplies->selectedSlot = n;
							break;
						}
					}
				}
			}
			if (blockHeight == 3) {
				if (!gm->tryPlaceBlock(blockAbove0)) {
					blockAbove0.y -= 0;
				}
			}
			if (blockHeight == 4) {
				if (!gm->tryPlaceBlock(blockAbove1)) {
					blockAbove1.y -= 0;
				}
				if (multiplace) {
					if (!gm->tryPlaceBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
				}
			}
			if (blockHeight == 5) {
				if (!gm->tryPlaceBlock(blockAbove2)) {
					blockAbove2.y -= 0;
				}
				if (multiplace) {
					if (!gm->tryPlaceBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
				}
			}
			if (blockHeight == 6) {
				if (!gm->tryPlaceBlock(blockAbove3)) {
					blockAbove3.y -= 0;
				}
				if (multiplace) {
					if (!gm->tryPlaceBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove2)) {
						blockAbove1.y -= 0;
					}
				}
			}
			if (blockHeight == 7) {
				if (!gm->tryPlaceBlock(blockAbove4)) {
					blockAbove4.y -= 0;
				}
				if (multiplace) {
					if (!gm->tryPlaceBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove2)) {
						blockAbove2.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove3)) {
						blockAbove2.y -= 0;
					}
				}
			}
			if (blockHeight == 8) {
				if (!gm->tryPlaceBlock(blockAbove5)) {
					blockAbove5.y -= 0;
				}
				if (multiplace) {
					if (!gm->tryPlaceBlock(blockAbove0)) {
						blockAbove0.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove1)) {
						blockAbove1.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove2)) {
						blockAbove2.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove3)) {
						blockAbove3.y -= 0;
					}
					if (!gm->tryPlaceBlock(blockAbove4)) {
						blockAbove4.y -= 0;
					}
				}
			}
			supplies->selectedSlot = slotab;
		}
	}
}