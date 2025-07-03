#include "GameMode.h"
#include "Player.h"

bool GameMode::tryPlaceBlock(const Vec3<int>& blockPos) {
	BlockSource* region = player->dimension->blockSource;
	Block* block = region->getBlock(blockPos);
	BlockLegacy* blockLegacy = block->blockLegacy;
	if (!blockLegacy->material->isSolidBlocking()) {
		Vec3<int> blok(blockPos);

		static std::vector<Vec3<int>> checklist = {
			Vec3<int>(0, -1, 0),
			Vec3<int>(0, 1, 0),
			Vec3<int>(0, 0, -1),
			Vec3<int>(0, 0, 1),
			Vec3<int>(-1, 0, 0),
			Vec3<int>(1, 0, 0)
		};

		int i = 0;

		for (const Vec3<int>& current : checklist) {
			Vec3<int> calc = blok.sub(current);
			if (region->getBlock(calc)->blockLegacy->material->isSolidBlocking()) {
				buildBlock(calc, i, false);
				return true;
			}
			i++;
		}
	}
	return false;
}