#include "PlayerUtil.h"

bool PlayerUtil::canPlaceBlock(const BlockPos& blockPos, bool airPlace) {
	LocalPlayer* localPlayer = Game::ourPlayer();
	BlockSource* region = Game::clientInstance->getRegion();
	Block* block = region->getBlock(blockPos);
	BlockLegacy* blockLegacy = block->blockLegacy;

	if (blockLegacy->canBeBuiltOver(region, blockPos)) {

		if (airPlace) {
			return true;
		}

		BlockPos blockToPlace = blockPos;

		static BlockPos checklist[6] = { BlockPos(0, -1, 0),
										 BlockPos(0, 1, 0),
										 BlockPos(0, 0, -1),
										 BlockPos(0, 0, 1),
										 BlockPos(-1, 0, 0),
										 BlockPos(1, 0, 0) };

		for (const BlockPos& current : checklist) {
			BlockPos calc = blockToPlace.sub(current);
			if (!region->getBlock(calc)->blockLegacy->canBeBuiltOver(region, calc)) {
				return true;
			}
		}
	}
	return false;
}

bool PlayerUtil::tryPlaceBlock(const BlockPos& blockPos, bool airPlace, bool packetPlace) {
	LocalPlayer* localPlayer = Game::ourPlayer();
	BlockSource* region = Game::clientInstance->getRegion();
	Block* block = region->getBlock(blockPos);
	BlockLegacy* blockLegacy = block->blockLegacy;

	if (blockLegacy->canBeBuiltOver(region, blockPos)) {

		BlockPos blockToPlace = blockPos;

		static BlockPos checklist[6] = { BlockPos(0, -1, 0),
										 BlockPos(0, 1, 0),
										 BlockPos(0, 0, -1),
										 BlockPos(0, 0, 1),
										 BlockPos(-1, 0, 0),
										 BlockPos(1, 0, 0) };

		bool foundCandidate = false;
		uint8_t i = 0;

		for (const BlockPos& current : checklist) {
			BlockPos calc = blockToPlace.sub(current);
			if (!region->getBlock(calc)->blockLegacy->canBeBuiltOver(region, calc)) {
				foundCandidate = true;
				blockToPlace = calc;
				break;
			}
			i++;
		}

		if (airPlace && !foundCandidate) {
			foundCandidate = true;
			blockToPlace = blockPos;
			i = 0;
		}

		if (foundCandidate) {
			if (!packetPlace) {
				localPlayer->gameMode->buildBlock(blockToPlace, i, false);
			}
			else {
				PlayerInventory* plrInv = localPlayer->playerInventory;
				std::unique_ptr<ItemUseInventoryTransaction> itemUseInvTransac = ItemUseInventoryTransaction::make_unique();
				itemUseInvTransac->actionType = ItemUseInventoryTransaction::ActionType::Place;
				itemUseInvTransac->blockPos = blockToPlace;
				itemUseInvTransac->targetBlockRuntimeId = *region->getBlock(blockToPlace)->getRuntimeId();
				itemUseInvTransac->face = i;
				itemUseInvTransac->slot = plrInv->selectedSlot;
				itemUseInvTransac->itemInHand = NetworkItemStackDescriptor(plrInv->container->getItem(plrInv->selectedSlot));
				itemUseInvTransac->playerPos = localPlayer->getPos();
				itemUseInvTransac->clickPos = Vec3<float>(0.5f, 0.5f, 0.5f);

				InventoryTransactionPacket pk(std::move(itemUseInvTransac));
				localPlayer->level->getPacketSender()->send(&pk);
			}
			return true;
		}
	}
	return false;
}