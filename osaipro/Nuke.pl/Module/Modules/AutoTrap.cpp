#include "AutoTrap.h"

AutoTrap::AutoTrap() : IModule(0x0, Category::COMBAT, "Automatically traps the nearest player") {
	registerEnumSetting("Block", &mode, 0);
	mode.addEntry("Obsidian", 0);
	mode.addEntry("tnt", 1);
	registerIntSetting("range", &this->range, this->range, 3, 12);
	registerBoolSetting("onClick", &this->onClick, this->onClick);
}

AutoTrap::~AutoTrap() {
}

const char* AutoTrap::getModuleName() {
	return "AutoTrap";
}

static std::vector<Entity*> targetListat;

void findEntityat(Entity* currentEntity, bool isRegularEntity) {
	static auto AutoTrapMod = moduleMgr->getModule<AutoTrap>();
	if (currentEntity == nullptr) return;
	if (!Game.getLocalPlayer()->canAttack(currentEntity, false)) return;
	if (!Game.getLocalPlayer()->isAlive()) return;
	if (!currentEntity->isAlive()) return;
	if (currentEntity->getEntityTypeId() != 319) return;
	if (currentEntity->getNameTag()->getTextLength() <= 1) return;
	if (currentEntity->isLocalPlayer()) return;
	std::string invalids[6] = {
	"-", ".", "/",
	"+", ",", "#"
	};
	for (auto invalid : invalids) {
		size_t invalidChar = std::string(currentEntity->getNameTag()->getText()).find(invalid);
		if (invalidChar != std::string::npos) return;
	}
	float dist = (*currentEntity->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < AutoTrapMod->range) {
		targetListat.push_back(currentEntity);
	}
}

bool AutoTrap::tryAutoTrap(Vec3 AutoTrap) {
	AutoTrap = AutoTrap.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(AutoTrap));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(AutoTrap);

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

void AutoTrap::onTick(GameMode* gm) {
	targetListat.clear();
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	Game.forEachEntity(findEntityat);

	int place = 1;

	if (onClick) {
		if (GameData::isRightClickDown()) {
			place = 0;
		}
		else {
			place = 1;
		}
	}


	if (!onClick) {
		place = 0;
	}

	if (!targetListat.empty()) {
		//ground level
		Vec3 blockSideL1 = targetListat[0]->eyePos0;

		Vec3 blockSideR1 = targetListat[0]->eyePos0;

		Vec3 blockSideL2 = targetListat[0]->eyePos0;

		Vec3 blockSideL3 = targetListat[0]->eyePos0;

		Vec3 blockSideL4 = targetListat[0]->eyePos0;

		Vec3 blockSideR2 = targetListat[0]->eyePos0;

		Vec3 blockSideR3 = targetListat[0]->eyePos0;

		Vec3 blockSideR4 = targetListat[0]->eyePos0;

		//middle level
		Vec3 blockSideL1M = targetListat[0]->eyePos0;

		Vec3 blockSideR1M = targetListat[0]->eyePos0;

		Vec3 blockSideL2M = targetListat[0]->eyePos0;

		Vec3 blockSideL3M = targetListat[0]->eyePos0;

		Vec3 blockSideL4M = targetListat[0]->eyePos0;

		Vec3 blockSideR2M = targetListat[0]->eyePos0;

		Vec3 blockSideR3M = targetListat[0]->eyePos0;

		Vec3 blockSideR4M = targetListat[0]->eyePos0;

		//top level
		Vec3 blockSideL1T = targetListat[0]->eyePos0;

		Vec3 blockSideR1T = targetListat[0]->eyePos0;

		Vec3 blockSideL2T = targetListat[0]->eyePos0;

		Vec3 blockSideL3T = targetListat[0]->eyePos0;

		Vec3 blockSideL4T = targetListat[0]->eyePos0;

		Vec3 blockSideR2T = targetListat[0]->eyePos0;

		Vec3 blockSideR3T = targetListat[0]->eyePos0;

		Vec3 blockSideR4T = targetListat[0]->eyePos0;

		//above head level
		Vec3 blockAboveH1 = targetListat[0]->eyePos0;
		Vec3 blockAboveH2 = targetListat[0]->eyePos0;
		Vec3 blockAboveH3 = targetListat[0]->eyePos0;
		Vec3 blockAboveH4 = targetListat[0]->eyePos0;

		//blocks below player
		Vec3 blockBelowB1 = targetListat[0]->eyePos0;
		Vec3 blockBelowB2 = targetListat[0]->eyePos0;
		Vec3 blockBelowB3 = targetListat[0]->eyePos0;
		Vec3 blockBelowB4 = targetListat[0]->eyePos0;

		//ground level
		Vec3 blockSide1 = targetListat[0]->eyePos0;

		Vec3 blockSide2 = targetListat[0]->eyePos0;

		Vec3 blockSide3 = targetListat[0]->eyePos0;

		Vec3 blockSide4 = targetListat[0]->eyePos0;

		//middle level
		Vec3 blockSide5 = targetListat[0]->eyePos0;

		Vec3 blockSide6 = targetListat[0]->eyePos0;

		Vec3 blockSide7 = targetListat[0]->eyePos0;

		Vec3 blockSide8 = targetListat[0]->eyePos0;

		//top level
		Vec3 blockSide9 = targetListat[0]->eyePos0;

		Vec3 blockSide10 = targetListat[0]->eyePos0;

		Vec3 blockSide11 = targetListat[0]->eyePos0;

		Vec3 blockSide12 = targetListat[0]->eyePos0;

		//above players head
		Vec3 blockAbove = targetListat[0]->eyePos0;
		Vec3 blockAbove1 = targetListat[0]->eyePos0;
		Vec3 blockAbove2 = targetListat[0]->eyePos0;
		Vec3 blockAbove3 = targetListat[0]->eyePos0;
		Vec3 blockAbove4 = targetListat[0]->eyePos0;

		//block below player
		Vec3 blockBelow = targetListat[0]->eyePos0;
		Vec3 blockBelow1 = targetListat[0]->eyePos0;
		Vec3 blockBelow2 = targetListat[0]->eyePos0;
		Vec3 blockBelow3 = targetListat[0]->eyePos0;
		Vec3 blockBelow4 = targetListat[0]->eyePos0;

		//ground level
		blockSideL1.x -= targetListat[0]->height + 0;
		blockSideL1.z -= targetListat[0]->height - 1;
		blockSideL1.y -= targetListat[0]->height - 1;

		blockSideR1.x -= targetListat[0]->height + 0;
		blockSideR1.z += targetListat[0]->height - 1;
		blockSideR1.y -= targetListat[0]->height - 1;

		blockSideL2.x += targetListat[0]->height + 0;
		blockSideL2.z -= targetListat[0]->height - 1;
		blockSideL2.y -= targetListat[0]->height - 1;

		blockSideR2.x += targetListat[0]->height + 0;
		blockSideR2.z += targetListat[0]->height - 1;
		blockSideR2.y -= targetListat[0]->height - 1;

		blockSideL3.x -= targetListat[0]->height - 1;
		blockSideL3.z -= targetListat[0]->height + 0;
		blockSideL3.y -= targetListat[0]->height - 1;

		blockSideR3.x -= targetListat[0]->height - 1;
		blockSideR3.z += targetListat[0]->height + 0;
		blockSideR3.y -= targetListat[0]->height - 1;

		blockSideL4.x += targetListat[0]->height - 1;
		blockSideL4.z -= targetListat[0]->height + 0;
		blockSideL4.y -= targetListat[0]->height - 1;

		blockSideR4.x += targetListat[0]->height - 1;
		blockSideR4.z += targetListat[0]->height + 0;
		blockSideR4.y -= targetListat[0]->height - 1;

		//middle level
		blockSideL1M.x -= targetListat[0]->height + 0;
		blockSideL1M.z -= targetListat[0]->height - 1;
		blockSideL1M.y += targetListat[0]->height - 2;

		blockSideR1M.x -= targetListat[0]->height + 0;
		blockSideR1M.z += targetListat[0]->height - 1;
		blockSideR1M.y += targetListat[0]->height - 2;

		blockSideL2M.x += targetListat[0]->height + 0;
		blockSideL2M.z -= targetListat[0]->height - 1;
		blockSideL2M.y += targetListat[0]->height - 2;

		blockSideR2M.x += targetListat[0]->height + 0;
		blockSideR2M.z += targetListat[0]->height - 1;
		blockSideR2M.y += targetListat[0]->height - 2;

		blockSideL3M.x -= targetListat[0]->height - 1;
		blockSideL3M.z -= targetListat[0]->height + 0;
		blockSideL3M.y += targetListat[0]->height - 2;

		blockSideR3M.x -= targetListat[0]->height - 1;
		blockSideR3M.z += targetListat[0]->height + 0;
		blockSideR3M.y += targetListat[0]->height - 2;

		blockSideL4M.x += targetListat[0]->height - 1;
		blockSideL4M.z -= targetListat[0]->height + 0;
		blockSideL4M.y += targetListat[0]->height - 2;

		blockSideR4M.x += targetListat[0]->height - 1;
		blockSideR4M.z += targetListat[0]->height + 0;
		blockSideR4M.y += targetListat[0]->height - 2;

		//top level
		blockSideL1T.x -= targetListat[0]->height + 0;
		blockSideL1T.z -= targetListat[0]->height - 1;
		blockSideL1T.y -= targetListat[0]->height - 3;

		blockSideR1T.x -= targetListat[0]->height + 0;
		blockSideR1T.z += targetListat[0]->height - 1;
		blockSideR1T.y -= targetListat[0]->height - 3;

		blockSideL2T.x += targetListat[0]->height + 0;
		blockSideL2T.z -= targetListat[0]->height - 1;
		blockSideL2T.y -= targetListat[0]->height - 3;

		blockSideR2T.x += targetListat[0]->height + 0;
		blockSideR2T.z += targetListat[0]->height - 1;
		blockSideR2T.y -= targetListat[0]->height - 3;

		blockSideL3T.x -= targetListat[0]->height - 1;
		blockSideL3T.z -= targetListat[0]->height + 0;
		blockSideL3T.y -= targetListat[0]->height - 3;

		blockSideR3T.x -= targetListat[0]->height - 1;
		blockSideR3T.z += targetListat[0]->height + 0;
		blockSideR3T.y -= targetListat[0]->height - 3;

		blockSideL4T.x += targetListat[0]->height - 1;
		blockSideL4T.z -= targetListat[0]->height + 0;
		blockSideL4T.y -= targetListat[0]->height - 3;

		blockSideR4T.x += targetListat[0]->height - 1;
		blockSideR4T.z += targetListat[0]->height + 0;
		blockSideR4T.y -= targetListat[0]->height - 3;

		//H1 x + z above head level
		blockAboveH1.y -= targetListat[0]->height - 3;
		blockAboveH1.x -= targetListat[0]->height - 1;
		blockAboveH1.z -= targetListat[0]->height - 1;

		blockAboveH2.y -= targetListat[0]->height - 3;
		blockAboveH2.x -= targetListat[0]->height - 1;
		blockAboveH2.z += targetListat[0]->height - 1;

		blockAboveH3.y -= targetListat[0]->height - 3;
		blockAboveH3.x += targetListat[0]->height - 1;
		blockAboveH3.z -= targetListat[0]->height - 1;

		blockAboveH4.y -= targetListat[0]->height - 3;
		blockAboveH4.x += targetListat[0]->height - 1;
		blockAboveH4.z += targetListat[0]->height - 1;

		//below Player B1 to B4
		blockBelowB1.y -= targetListat[0]->height + 0;
		blockBelowB1.x -= targetListat[0]->height - 1;
		blockBelowB1.z -= targetListat[0]->height - 1;

		blockBelowB2.y -= targetListat[0]->height + 0;
		blockBelowB2.x -= targetListat[0]->height - 1;
		blockBelowB2.z += targetListat[0]->height - 1;

		blockBelowB3.y -= targetListat[0]->height + 0;
		blockBelowB3.x += targetListat[0]->height - 1;
		blockBelowB3.z -= targetListat[0]->height - 1;

		blockBelowB4.y -= targetListat[0]->height + 0;
		blockBelowB4.x += targetListat[0]->height - 1;
		blockBelowB4.z += targetListat[0]->height - 1;

		//ground level
		blockSide1.x -= targetListat[0]->height + 0;
		blockSide1.y -= targetListat[0]->height - 1;

		blockSide2.x += targetListat[0]->height + 0;
		blockSide2.y -= targetListat[0]->height - 1;

		blockSide3.z -= targetListat[0]->height + 0;
		blockSide3.y -= targetListat[0]->height - 1;

		blockSide4.z += targetListat[0]->height + 0;
		blockSide4.y -= targetListat[0]->height - 1;

		//middle level
		blockSide5.x -= targetListat[0]->height + 0;
		blockSide6.x += targetListat[0]->height + 0;
		blockSide7.z -= targetListat[0]->height + 0;
		blockSide8.z += targetListat[0]->height + 0;

		//top level
		blockSide9.x -= targetListat[0]->height + 0;
		blockSide9.y -= targetListat[0]->height - 3;

		blockSide10.x += targetListat[0]->height + 0;
		blockSide10.y -= targetListat[0]->height - 3;

		blockSide11.z -= targetListat[0]->height + 0;
		blockSide11.y -= targetListat[0]->height - 3;

		blockSide12.z += targetListat[0]->height + 0;
		blockSide12.y -= targetListat[0]->height - 3;

		//above players head
		blockAbove.y -= targetListat[0]->height - 3;

		blockAbove1.y -= targetListat[0]->height - 3;
		blockAbove1.x -= targetListat[0]->height - 1;

		blockAbove2.y -= targetListat[0]->height - 3;
		blockAbove2.x += targetListat[0]->height - 1;

		blockAbove3.y -= targetListat[0]->height - 3;
		blockAbove3.z -= targetListat[0]->height - 1;

		blockAbove4.y -= targetListat[0]->height - 3;
		blockAbove4.z += targetListat[0]->height - 1;

		//block below player
		blockBelow.y -= targetListat[0]->height + 0;

		blockBelow1.y -= targetListat[0]->height + 0;
		blockBelow1.x -= targetListat[0]->height - 1;

		blockBelow2.y -= targetListat[0]->height + 0;
		blockBelow2.x += targetListat[0]->height - 1;

		blockBelow3.y -= targetListat[0]->height + 0;
		blockBelow3.z -= targetListat[0]->height - 1;

		blockBelow4.y -= targetListat[0]->height + 0;
		blockBelow4.z += targetListat[0]->height - 1;
		if (moduleMgr->AutoDesync()) return;
		if (place == 0) {
			int slotab = 0;
			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
			Inventory* inv = supplies->inventory;
			slotab = supplies->selectedHotbarSlot;
			for (int n = 0; n < 36; n++) {
				ItemStack* stack = inv->getItemStack(n);
				if (stack->item != nullptr) {
					if (mode.getSelectedValue() == 0) {
						if (stack->getItem()->itemId == 49) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
					if (mode.getSelectedValue() == 1) {
						if (stack->getItem()->itemId == 46) {
							supplies->selectedHotbarSlot = n;
							break;
						}
					}
				}
			}
			if (!tryAutoTrap(blockSideL1)) {
				blockSideL1.x -= 0;
				blockSideL1.z -= 0;
			}
			if (!tryAutoTrap(blockSideR1)) {
				blockSideR1.x -= 0;
				blockSideR1.z -= 0;
			}
			if (!tryAutoTrap(blockSideL2)) {
				blockSideL2.x -= 0;
				blockSideL2.z -= 0;
			}
			if (!tryAutoTrap(blockSideR2)) {
				blockSideR2.x -= 0;
				blockSideR2.z -= 0;
			}
			if (!tryAutoTrap(blockSideL3)) {
				blockSideL3.x -= 0;
				blockSideL3.z -= 0;
			}
			if (!tryAutoTrap(blockSideR3)) {
				blockSideR3.x -= 0;
				blockSideR3.z -= 0;
			}
			if (!tryAutoTrap(blockSideL4)) {
				blockSideL4.x -= 0;
				blockSideL4.z -= 0;
			}
			if (!tryAutoTrap(blockSideR4)) {
				blockSideR4.x -= 0;
				blockSideR4.z -= 0;
			}







































			//L1M to R4M are x + z sides middle level
			if (!tryAutoTrap(blockSideL1M)) {
				blockSideL1M.x -= 0;
				blockSideL1M.z -= 0;
			}
			if (!tryAutoTrap(blockSideR1M)) {
				blockSideR1M.x -= 0;
				blockSideR1M.z -= 0;
			}
			if (!tryAutoTrap(blockSideL2M)) {
				blockSideL2M.x -= 0;
				blockSideL2M.z -= 0;
			}
			if (!tryAutoTrap(blockSideR2M)) {
				blockSideR2M.x -= 0;
				blockSideR2M.z -= 0;
			}
			if (!tryAutoTrap(blockSideL3M)) {
				blockSideL3M.x -= 0;
				blockSideL3M.z -= 0;
			}
			if (!tryAutoTrap(blockSideR3M)) {
				blockSideR3M.x -= 0;
				blockSideR3M.z -= 0;
			}
			if (!tryAutoTrap(blockSideL4M)) {
				blockSideL4M.x -= 0;
				blockSideL4M.z -= 0;
			}
			if (!tryAutoTrap(blockSideR4M)) {
				blockSideR4M.x -= 0;
				blockSideR4M.z -= 0;
			}
			//L1T to R4T are x + z sides top level
			if (!tryAutoTrap(blockSideL1T)) {
				blockSideL1T.x -= 0;
				blockSideL1T.z -= 0;
			}
			if (!tryAutoTrap(blockSideR1T)) {
				blockSideR1T.x -= 0;
				blockSideR1T.z -= 0;
			}
			if (!tryAutoTrap(blockSideL2T)) {
				blockSideL2T.x -= 0;
				blockSideL2T.z -= 0;
			}
			if (!tryAutoTrap(blockSideR2T)) {
				blockSideR2T.x -= 0;
				blockSideR2T.z -= 0;
			}
			if (!tryAutoTrap(blockSideL3T)) {
				blockSideL3T.x -= 0;
				blockSideL3T.z -= 0;
			}
			if (!tryAutoTrap(blockSideR3T)) {
				blockSideR3T.x -= 0;
				blockSideR3T.z -= 0;
			}
			if (!tryAutoTrap(blockSideL4T)) {
				blockSideL4T.x -= 0;
				blockSideL4T.z -= 0;
			}
			if (!tryAutoTrap(blockSideR4T)) {
				blockSideR4T.x -= 0;
				blockSideR4T.z -= 0;
			}
			//H1 to H4 are above players head
			if (!tryAutoTrap(blockAboveH1)) {
				blockAboveH1.x -= 0;
				blockAboveH1.z -= 0;
			}
			if (!tryAutoTrap(blockAboveH2)) {
				blockAboveH2.x -= 0;
				blockAboveH2.z -= 0;
			}
			if (!tryAutoTrap(blockAboveH3)) {
				blockAboveH3.x -= 0;
				blockAboveH3.z -= 0;
			}
			if (!tryAutoTrap(blockAboveH4)) {
				blockAboveH4.x -= 0;
				blockAboveH4.z -= 0;
			}
			//B1 to B4 below the player
			if (!tryAutoTrap(blockBelowB1)) {
				blockBelowB1.x -= 0;
				blockBelowB1.z -= 0;
			}
			if (!tryAutoTrap(blockBelowB2)) {
				blockBelowB2.x -= 0;
				blockBelowB2.z -= 0;
			}
			if (!tryAutoTrap(blockBelowB3)) {
				blockBelowB3.x -= 0;
				blockBelowB3.z -= 0;
			}
			if (!tryAutoTrap(blockBelowB4)) {
				blockBelowB4.x -= 0;
				blockBelowB4.z -= 0;
			}
			//blockSide 1 to 4 are placing at ground level around the player
			if (!tryAutoTrap(blockSide1)) {
				blockSide1.y -= 0;
				blockSide1.x -= 0;
			}
			if (!tryAutoTrap(blockSide2)) {
				blockSide2.y += 0;
				blockSide2.x += 0;
			}
			if (!tryAutoTrap(blockSide3)) {
				blockSide3.y -= 0;
				blockSide3.z -= 0;
			}
			if (!tryAutoTrap(blockSide4)) {
				blockSide4.y += 0;
				blockSide4.z += 0;
			}
			//blockSide 5 to 8 are placing 1 up around the player
			if (!tryAutoTrap(blockSide5)) {
				blockSide5.x -= 0;
			}
			if (!tryAutoTrap(blockSide6)) {
				blockSide6.x += 0;
			}
			if (!tryAutoTrap(blockSide7)) {
				blockSide7.z -= 0;
			}
			if (!tryAutoTrap(blockSide8)) {
				blockSide8.z += 0;
			}
			//blockSide 9 to 12 at players head and around
			if (!tryAutoTrap(blockSide9)) {
				blockSide9.y -= 0;
				blockSide9.x -= 0;
			}
			if (!tryAutoTrap(blockSide10)) {
				blockSide10.y -= 0;
				blockSide10.x += 0;
			}
			if (!tryAutoTrap(blockSide11)) {
				blockSide11.y -= 0;
				blockSide11.z -= 0;
			}
			if (!tryAutoTrap(blockSide12)) {
				blockSide12.y -= 0;
				blockSide12.z += 0;
			}
			//placing block above players head (3blocks up from the ground)
			if (!tryAutoTrap(blockAbove)) {
				blockAbove.y -= 0;
			}
			if (!tryAutoTrap(blockAbove1)) {
				blockAbove1.y -= 0;
			}
			if (!tryAutoTrap(blockAbove2)) {
				blockAbove2.y -= 0;
			}

			if (!tryAutoTrap(blockAbove3)) {
				blockAbove3.y -= 0;
			}
			if (!tryAutoTrap(blockAbove4)) {
				blockAbove4.y -= 0;
			}
			//below the player
			if (!tryAutoTrap(blockBelow)) {
				blockBelow.x -= 0;
				blockBelow.z -= 0;
			}
			if (!tryAutoTrap(blockBelow1)) {
				blockBelow1.x -= 0;
				blockBelow1.z -= 0;
			}
			if (!tryAutoTrap(blockBelow2)) {
				blockBelow2.x -= 0;
				blockBelow2.z -= 0;
			}
			if (!tryAutoTrap(blockBelow3)) {
				blockBelow3.x -= 0;
				blockBelow3.z -= 0;
			}
			if (!tryAutoTrap(blockBelow4)) {
				blockBelow4.x -= 0;
				blockBelow4.z -= 0;
			}
			supplies->selectedHotbarSlot = slotab;
		}
	}
}

void AutoTrap::onJoiningServer() {
	targetListat.clear();
}