#include "HoleFiller.h"

HoleFiller::HoleFiller() : IModule(0x0, Category::WORLD, "Places a buttplug in your gf's pussy") {
	this->registerBoolSetting("Obsidian Hole", &this->obsidian, this->obsidian);
	this->registerBoolSetting("Bedrock Hole", &this->bedrock, this->bedrock);
	this->registerBoolSetting("PitchUp", &this->doPitchUp, this->doPitchUp);
	registerBoolSetting("Switch2Obby", &this->holeSwitch, true);
	registerIntSetting("Range", &this->range, this->range, 0, 10);
	registerIntSetting("Blk/sec", &this->blockPerSec, this->blockPerSec, 0, 20);
	registerIntSetting("WallRange", &this->wallRange, this->wallRange, 0, 10);
}

HoleFiller::~HoleFiller() {
}

const char* HoleFiller::getModuleName() {
	return ("HoleFiller");
}

void getObbyHF() {
	auto supplies = g_Data.getLocalPlayer()->getSupplies();
	auto inv = supplies->inventory;  // g_Data.getLocalPlayer()->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies())->getItem()->itemID
	for (int n = 0; n < 9; n++) {
		C_ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {  // select obsid
				supplies->selectedHotbarSlot = n;
				return;
			}
		}
	}
}

bool tryPlaceHF(vec3_t blkPlacement) {
	blkPlacement = blkPlacement.floor();

	C_Block* block = g_Data.getLocalPlayer()->region->getBlock(vec3_ti(blkPlacement));
	C_BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		vec3_ti blok(blkPlacement);
		int i = 0;

		static std::vector<vec3_ti*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new vec3_ti(0, -1, 0));
			checklist.push_back(new vec3_ti(0, 1, 0));

			checklist.push_back(new vec3_ti(0, 0, -1));
			checklist.push_back(new vec3_ti(0, 0, 1));

			checklist.push_back(new vec3_ti(-1, 0, 0));
			checklist.push_back(new vec3_ti(1, 0, 0));
		}

		for (auto current : checklist) {
			vec3_ti calc = blok.sub(*current);
			bool Y = ((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((g_Data.getLocalPlayer()->region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			g_Data.getCGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}
	return false;
}

void HoleFiller::onEnable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	hasStarted = true;
	del0 = 0;

	origSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
	if (holeSwitch) {
		getObbyHF();
		return;
	}
	
}

void HoleFiller::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr) return;
	hasStarted = false;

	if (holeSwitch) {
		g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot = origSlot;
		return;
	}
}

void HoleFiller::onTick(C_GameMode* gm) {

	attemptPitchUp = false;

	if (!(g_Data.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		if (hasStarted) {
			hasStarted = false;
			return;
		}
		int maxT = 20 / blockPerSec;

		if (del0 < maxT) {
			del0++;
			return;
		} else
			del0 = 0;

		if (g_Data.isInGame()) {
			vec3_t* pos = g_Data.getLocalPlayer()->getPos();
			for (int x = (int)pos->x - range; x < pos->x + range; x++) {
				for (int z = (int)pos->z - range; z < pos->z + range; z++) {
					for (int y = (int)pos->y - range; y < pos->y + range; y++) {
						vec4_t underline;
						vec3_t blockPos = vec3_t(x, y, z);
						vec3_t air = vec3_t(x, y + 1, z);
						vec3_t air2 = vec3_t(x, y + 2, z);

						vec3_t blockPos2 = vec3_t(x + 1, y, z);
						vec3_t blockPos3 = vec3_t(x, y, z + 1);
						vec3_t blockPos4 = vec3_t(x, y - 1, z);
						vec3_t blockPos5 = vec3_t(x - 1, y, z);
						vec3_t blockPos6 = vec3_t(x, y, z - 1);

						vec3_t blockPos22 = vec3_t(x + 1, y + 1, z);
						vec3_t blockPos32 = vec3_t(x, y + 1, z + 1);
						vec3_t blockPos52 = vec3_t(x - 1, y + 1, z);
						vec3_t blockPos62 = vec3_t(x, y + 1, z - 1);

						C_Block* block = g_Data.getLocalPlayer()->region->getBlock(blockPos);
						C_Block* airpos = g_Data.getLocalPlayer()->region->getBlock(air);
						C_Block* airpos2 = g_Data.getLocalPlayer()->region->getBlock(air2);
						C_Block* block2 = g_Data.getLocalPlayer()->region->getBlock(blockPos2);
						C_Block* block3 = g_Data.getLocalPlayer()->region->getBlock(blockPos3);
						C_Block* block4 = g_Data.getLocalPlayer()->region->getBlock(blockPos4);
						C_Block* block5 = g_Data.getLocalPlayer()->region->getBlock(blockPos5);
						C_Block* block6 = g_Data.getLocalPlayer()->region->getBlock(blockPos6);

						C_Block* block22 = g_Data.getLocalPlayer()->region->getBlock(blockPos22);
						C_Block* block32 = g_Data.getLocalPlayer()->region->getBlock(blockPos32);
						C_Block* block52 = g_Data.getLocalPlayer()->region->getBlock(blockPos52);
						C_Block* block62 = g_Data.getLocalPlayer()->region->getBlock(blockPos62);

						if (block != nullptr && block2 != nullptr && block3 != nullptr && block4 != nullptr) {
							int blockId = block->toLegacy()->blockId;
							int aircheck = airpos->toLegacy()->blockId;
							int aircheck2 = airpos2->toLegacy()->blockId;
							int blockId2 = block2->toLegacy()->blockId;
							int blockId3 = block3->toLegacy()->blockId;
							int blockId4 = block4->toLegacy()->blockId;
							int blockId5 = block5->toLegacy()->blockId;
							int blockId6 = block6->toLegacy()->blockId;

							int blockId22 = block22->toLegacy()->blockId;
							int blockId32 = block32->toLegacy()->blockId;
							int blockId52 = block52->toLegacy()->blockId;
							int blockId62 = block62->toLegacy()->blockId;

							bool obs = false;
							vec3_ti plugHole = blockPos;
							if (blockId == 0) obs = true;

							if (doPitchUp)
								attemptPitchUp = true;

							if (bedrock) {
								if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)  && (countBlksAlongLine(*pos , plugHole.toVec3t()).blockCount <= wallRange)) {
									//gm->buildBlock(&plugHole, 0);
									tryPlaceHF(plugHole.toVec3t());
								}
							}
							if (obsidian) {
								if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)) && (countBlksAlongLine(*pos, plugHole.toVec3t()).blockCount <= wallRange)) {
									//gm->buildBlock(&plugHole, 0);
									tryPlaceHF(plugHole.toVec3t());
								}
							}
						}
					}
				}
			}
		}
	}
}