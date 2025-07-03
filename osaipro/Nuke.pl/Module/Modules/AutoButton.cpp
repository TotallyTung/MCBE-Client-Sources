#include "AutoButton.h"
#include "../ModuleManager.h"

AutoButton::AutoButton() : IModule(70, Category::PLAYER, "AutoButton By REA, All Button Support") {
	registerBoolSetting("onClick", &onClick, onClick);
}

AutoButton::~AutoButton() {
}
/*/ StartButtonList /*/
/*/		143 /*/
/*/		4294967152 /*/
/*/		4294967155 /*/
/*/		4294967153 /*/
/*/		4294967156 /*/
/*/		4294967154 /*/
/*/		77 /*/
/*/		4294967036 /*/
/*/		4294967035 /*/
/*/		4294967000 /*/
/*/EndButtonList /*/
const char* AutoButton::getModuleName() {
	return ("AutoButton");
}
void AutoButton::findButt() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 36; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->itemId == 143) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967152) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967155) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967153) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967156) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967154) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 77) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967036) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967035) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 4294967000) {
				supplies->selectedHotbarSlot = n;
				break;
			}
			if (stack->getItem()->itemId == 373) {
				supplies->selectedHotbarSlot = n;
				break;
			}

		}
	}
}
bool AutoButton_tryPlace1(Vec3 blkPlacement) {
	blkPlacement = blkPlacement.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blkPlacement));
	BlockLegacy* blockLegacy = block->blockLegacy;
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blkPlacement);
		int i = 0;

		static const std::vector<Vec3i> checklist = {
			//Vec3i(1, -1, 0),
			Vec3i(0, -1, 0),
			Vec3i(0, 1, 0),
			Vec3i(0, 0, -1),
			Vec3i(0, 0, 1),
			Vec3i(-1, 0, 0),
			Vec3i(1, 0, 0) };

		bool foundCandidate = false;

		for (const auto& current : checklist) {
			Vec3i calc = blok.sub(current);
			bool Y = (Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable;
			if (!(Game.getLocalPlayer()->region->getBlock(calc)->blockLegacy)->material->isReplaceable) {
				// Found a solid block to click
				foundCandidate = true;
				blok = calc;
				break;
			}
			i++;
		}

		if (foundCandidate) {
			Game.getGameMode()->buildBlock(&blok, i, true);  // if it breaks, then idk
			return true;
		}
	}

	return false;
}
bool AutoButton_placeBlockBelow1(const Vec3& blockPos) {
	Vec3 blockPosBelow1(blockPos.x - 0, blockPos.y - 1, blockPos.z - 0);
	Vec3 blockPosBelow(blockPos.x - 0, blockPos.y - 2, blockPos.z - 0);
	if (AutoButton_tryPlace1(blockPosBelow1))
		return true;
	if (AutoButton_tryPlace1(blockPosBelow))
		return true;
	return false;
}
void AutoButton::onTick(GameMode* gm) {
	if (Game.getLocalPlayer() != nullptr) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	if (moduleMgr->AutoDesync()) return;
	slotab = supplies->selectedHotbarSlot;
	findButt();
	Vec3 playerFeetPos = Game.getLocalPlayer()->getPos()->floor();
	Vec3 buttonPos = Vec3(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
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
	if (place == 0)
		AutoButton_placeBlockBelow1(playerFeetPos);
	if (silentab) {
		supplies->selectedHotbarSlot = slotab;
	}
}