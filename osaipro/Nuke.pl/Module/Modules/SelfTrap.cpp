#include "SelfTrap.h"

SelfTrap::SelfTrap() : IModule(0x0, Category::COMBAT, "SelfTrap") {

}

SelfTrap::~SelfTrap() {
}

const char* SelfTrap::getModuleName() {
	return "SelfTrap";
}
bool SelfTrapPlace(Vec3 SelfTrapPlacement) {
	SelfTrapPlacement = SelfTrapPlacement.floor();
	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(SelfTrapPlacement));
	BlockLegacy* blockLegacy = block->blockLegacy;
	if (blockLegacy->material->isReplaceable) {
		Vec3i blocks(SelfTrapPlacement);
		int i = 0;

		static const std::vector<Vec3i> checklist = {Vec3i(0, -1, 0), Vec3i(0, 1, 0), Vec3i(0, 0, -1), Vec3i(0, 0, 1), Vec3i(-1, 0, 0), Vec3i(1, 0, 0) };

		bool foundCandidate = false;

		for (const auto& current : checklist) {
			Vec3i calculation = blocks.sub(current);
			bool Y = (Game.getLocalPlayer()->region->getBlock(calculation)->blockLegacy)->material->isReplaceable;
			if (!(Game.getLocalPlayer()->region->getBlock(calculation)->blockLegacy)->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blocks = calculation;
				break;
			}
			i++;
		}

		if (foundCandidate) {
			// Check if there is a button below the player's feet


			// Call buildBlock directly to SelfTrapPlace the block silently
			Game.getGameMode()->buildBlock(&blocks, i, true);
			return true;
		}
	}

	return false;
}
void SelfTrapSelfTrapPlaceBlock(const Vec3& blockPos) {
	Vec3 BlockPos1 = blockPos.add(Vec3(0, 2, 0));
	Vec3 BlockPos2 = blockPos.add(Vec3(0, 1, 0));
	SelfTrapPlace(BlockPos1);
	SelfTrapPlace(BlockPos2);
}

void SelfTrap::onTick(GameMode* gm) {
	GameSettingsInput* input = Game.getClientInstance()->getGameSettingsInput();
	if (Game.getLocalPlayer() == nullptr) return;
	if (!Game.canUseMoveKeys()) return;
	if (moduleMgr->AutoDesync()) return;
	Vec3 playerPos = Game.getLocalPlayer()->getPos()->floor();
	Vec3 Obsidian1 = playerPos.add(Vec3(1, -3, 0));
	Vec3 Obsidian2 = playerPos.add(Vec3(1, -2, 0));
	Vec3 Obsidian3 = playerPos.add(Vec3(1, -1, 0));
	Vec3 Anvil     = playerPos.add(Vec3(0, 0, 0));
	// Silent Code
	int slotab = 0;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	slotab = supplies->selectedHotbarSlot;
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 49) {
				supplies->selectedHotbarSlot = n;
				break;
			}
		}
	}
	SelfTrapSelfTrapPlaceBlock(Obsidian1);
	SelfTrapSelfTrapPlaceBlock(Obsidian2);
	SelfTrapSelfTrapPlaceBlock(Obsidian3);
	SelfTrapSelfTrapPlaceBlock(Anvil);
	supplies->selectedHotbarSlot = slotab;
}
void SelfTrap::onDisable() {
}