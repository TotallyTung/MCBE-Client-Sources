#include "GameMode.h"
#include "../Memory/GameData.h"

bool GameMode::tryPlaceBlock(const Vec3i& blockPos) {
	BlockSource* region = this->player->region;
	Block* block = region->getBlock(blockPos);
	BlockLegacy* blockLegacy = block->blockLegacy;
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blockPos);

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
		for (Vec3i* current : checklist) {
			Vec3i calc = blok.sub(*current);
			bool Y = ((region->getBlock(calc)->blockLegacy))->material->isReplaceable;
			if (!((region->getBlock(calc)->blockLegacy))->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}
		if (foundCandidate) {
			buildBlock(&blok, i, false);
			return true;
		}
	}
	return false;
}
