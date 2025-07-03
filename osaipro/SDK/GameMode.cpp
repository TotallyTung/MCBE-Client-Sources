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
void GameMode::baseUseItem(ItemStack& a1) {
	using baseUseItem = void (*)(GameMode*, ItemStack&);
	static baseUseItem baseUseItemFunc = reinterpret_cast<baseUseItem>(FindSignature("48 89 5C 24 ? 48 89 74 24 ? 55 57 41 56 48 8D 6C 24 ? 48 81 EC ? ? ? ? 48 8B 05 ? ? ? ? 48 33 C4 48 89 45 ? 48 8B F2 48 8B F9 45 33 F6 4C 89 75 ? E8 ? ? ? ? 4C 8B 00 BA ? ? ? ? 48 8B C8 41 FF 50 ? 48 8B D8 48 89 44 24 ? 48 85 C0 74 ? 33 D2 41 B8 ? ? ? ? 48 8B C8 E8 ? ? ? ? 48 8B CB E8 ? ? ? ? EB ? 49 8B C6 48 89 45 ? 48 8B 5F"));

	return baseUseItemFunc(this, a1);
}
