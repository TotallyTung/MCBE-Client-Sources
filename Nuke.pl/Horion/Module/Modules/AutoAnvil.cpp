#include "AutoAnvil.h"

AutoAnvil::AutoAnvil() : IModule(0x0, Category::COMBAT, "AutoAnvil (Height Selector)") {
	registerEnumSetting("Block", &BlockID, 1);
	BlockID.addEntry("Anvil", 1);
	BlockID.addEntry("Sand", 2);
	BlockID.addEntry("Gravel", 3);
	BlockID.addEntry("Powder", 4);
	//this->registerIntSetting("Obsidian Height", &this->obsiheight, this->obsiheight, 3, 8);
	//this->registerIntSetting("Fall Block Height", &this->fallblockheight, this->fallblockheight, 3, 8);
	this->registerIntSetting("range", &this->range, this->range, 3, 12);
	//registerBoolSetting("Multi Place", &this->multiplace, this->multiplace);
	//registerBoolSetting("onClick", &this->onClick, this->onClick);
	//registerBoolSetting("2b2e", &this->Bypass, this->Bypass);
}

AutoAnvil::~AutoAnvil() {
}

const char* AutoAnvil::getModuleName() {
	return "AutoAnvil";
}

static std::vector<Entity*> targetListav;

void findEntityav(Entity* currentEntityav, bool isRegularEntity) {
	static auto AutoAnvilMod = moduleMgr->getModule<AutoAnvil>();

	if (currentEntityav == nullptr)
		return;

	if (currentEntityav == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntityav, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntityav->isAlive())
		return;


	if (!Target::isValidTarget(currentEntityav))
		return;

	float dist = (*currentEntityav->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < AutoAnvilMod->range) {
		targetListav.push_back(currentEntityav);

		float dist = (*currentEntityav->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < AutoAnvilMod->range) {
			targetListav.push_back(currentEntityav);
		}
	}
}

bool AutoAnvil::tryAutoAnvil(Vec3 AutoAnvil) {
	AutoAnvil = AutoAnvil.floor();

	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(AutoAnvil));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(AutoAnvil);

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
// void default_posselect() {
// 	if (!targetListav.empty()) {
// 		Vec3 blockAbove0 = targetListav[0]->eyePos0;
// 		Vec3 blockAbove1 = targetListav[0]->eyePos0;
// 		Vec3 blockAbove2 = targetListav[0]->eyePos0;
// 		Vec3 blockAbove3 = targetListav[0]->eyePos0;
// 		Vec3 blockAbove4 = targetListav[0]->eyePos0;
// 		Vec3 blockAbove5 = targetListav[0]->eyePos0;;
// 		Vec3 blockAboveH0 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH1 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH2 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH3 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH4 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH5 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH6 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH7 = targetListav[0]->eyePos0;
//		Vec3 blockAboveH8 = targetListav[0]->eyePos0;
//		//head 0 
//		blockAboveH8.y -= targetListav[0]->height - 0;
//		blockAboveH8.x -= targetListav[0]->height - 1;
//		blockAboveH8.z -= targetListav[0]->height - 2;
//		//head 1
//		blockAboveH0.y -= targetListav[0]->height - 1;
//		blockAboveH0.x -= targetListav[0]->height - 1;
//		blockAboveH0.z -= targetListav[0]->height - 2;
//		//head 2
//		blockAboveH1.y -= targetListav[0]->height - 2;
//		blockAboveH1.x -= targetListav[0]->height - 1;
//		blockAboveH1.z -= targetListav[0]->height - 2;
//		//head 3
//		blockAboveH2.y -= targetListav[0]->height - 3;
//		blockAboveH2.x -= targetListav[0]->height - 1;
//		blockAboveH2.z -= targetListav[0]->height - 2;
//		//head 4
//		blockAboveH3.y -= targetListav[0]->height - 4;
//		blockAboveH3.x -= targetListav[0]->height - 1;
//		blockAboveH3.z -= targetListav[0]->height - 2;
//		//head 5
//		blockAboveH4.y -= targetListav[0]->height - 5;
//		blockAboveH4.x -= targetListav[0]->height - 1;
//		blockAboveH4.z -= targetListav[0]->height - 2;
//		//head 6
//		blockAboveH5.y -= targetListav[0]->height - 6;
//		blockAboveH5.x -= targetListav[0]->height - 1;
//		blockAboveH5.z -= targetListav[0]->height - 2;
//		//head 7
//		blockAboveH6.y -= targetListav[0]->height - 7;
//		blockAboveH6.x -= targetListav[0]->height - 1;
//		blockAboveH6.z -= targetListav[0]->height - 2;
//		//head 8
//		blockAboveH7.y -= targetListav[0]->height - 8;
//		blockAboveH7.x -= targetListav[0]->height - 1;
//		blockAboveH7.z -= targetListav[0]->height - 2;
//		//Anvil 3
//		blockAbove0.y -= targetListav[0]->height - 3;
//		//Anvil 4
//		blockAbove1.y -= targetListav[0]->height - 4;
//		//Anvil 5
//		blockAbove2.y -= targetListav[0]->height - 5;
//		//Anvil 6
//		blockAbove3.y -= targetListav[0]->height - 6;
//		//Anvil 7
//		blockAbove4.y -= targetListav[0]->height - 7;
//		//Anvil 8
//		blockAbove5.y -= targetListav[0]->height - 8;

//		if (place == 0) {
//			// Silent Code
//			int slotab = 0;
//			PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
//			Inventory* inv = supplies->inventory;
//			slotab = supplies->selectedHotbarSlot;
//			for (int n = 0; n < 9; n++) {
//		//		ItemStack* stack = inv->getItemStack(n);
//		//		if (stack->item != nullptr) {
//		//			if (stack->getItem()->itemId == 49) {
//		//		//		supplies->selectedHotbarSlot = n;
//		//		//		break;
//		//			}
//		//		}
//			}
//			if (obsiheight == 3) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//			}
//			if (obsiheight == 4) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH3)) {
//		//			blockAboveH3.x -= 0;
//		//			blockAboveH3.z -= 0;
//		//		}
//			}
//			if (obsiheight == 5) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH3)) {
//		//			blockAboveH3.x -= 0;
//		//			blockAboveH3.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH4)) {
//		//			blockAboveH4.x -= 0;
//		//			blockAboveH4.z -= 0;
//		//		}
//			}
//			if (obsiheight == 6) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH3)) {
//		//			blockAboveH3.x -= 0;
//		//			blockAboveH3.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH4)) {
//		//			blockAboveH4.x -= 0;
//		//			blockAboveH4.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH5)) {
//		//			blockAboveH5.x -= 0;
//		//			blockAboveH5.z -= 0;
//		//		}
//			}
//			if (obsiheight == 7) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH3)) {
//		//			blockAboveH3.x -= 0;
//		//			blockAboveH3.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH4)) {
//		//			blockAboveH4.x -= 0;
//		//			blockAboveH4.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH5)) {
//		//			blockAboveH5.x -= 0;
//		//			blockAboveH5.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH6)) {
//		//			blockAboveH6.x -= 0;
//		//			blockAboveH6.z -= 0;
//		//		}
//			}
//			if (obsiheight == 8) {
//		//		if (!tryAutoAnvil(blockAboveH8)) {
//		//			blockAboveH8.x -= 0;
//		//			blockAboveH8.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH0)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH1)) {
//		//			blockAboveH1.x -= 0;
//		//			blockAboveH1.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH2)) {
//		//			blockAboveH2.x -= 0;
//		//			blockAboveH2.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH3)) {
//		//			blockAboveH3.x -= 0;
//		//			blockAboveH3.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH4)) {
//		//			blockAboveH4.x -= 0;
//		//			blockAboveH4.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH5)) {
//		//			blockAboveH5.x -= 0;
//		//			blockAboveH5.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH6)) {
//		//			blockAboveH6.x -= 0;
//		//			blockAboveH6.z -= 0;
//		//		}
//		//		if (!tryAutoAnvil(blockAboveH7)) {
//		//			blockAboveH7.x -= 0;
//		//			blockAboveH7.z -= 0;
//		//		}
//			}
//			supplies->selectedHotbarSlot = slotab;
//			slotab = 0;
//			slotab = supplies->selectedHotbarSlot;
//			for (int n = 0; n < 9; n++) {
//		//		ItemStack* stack = inv->getItemStack(n);
//		//		if (stack->item != nullptr) {
//		//			if (BlockID.getSelectedValue() == 1) {
//		//		//		if (stack->getItem()->itemId == 145) {
//		//		//			supplies->selectedHotbarSlot = n;
//		//		//			break;
//		//		//		}
//		//			}
//		//			if (BlockID.getSelectedValue() == 2) {
//		//		//		if (stack->getItem()->itemId == 12) {
//		//		//			supplies->selectedHotbarSlot = n;
//		//		//			break;
//		//		//		}
//		//			}
//		//			if (BlockID.getSelectedValue() == 3) {
//		//		//		if (stack->getItem()->itemId == 13) {
//		//		//			supplies->selectedHotbarSlot = n;
//		//		//			break;
//		//		//		}
//		//			}
//		//			if (BlockID.getSelectedValue() == 4) {
//		//		//		if (stack->getItem()->itemId == 252) {
//		//		//			supplies->selectedHotbarSlot = n;
//		//		//			break;
//		//		//		}
//		//			}
//		//		}
//			}
//			if (fallblockheight == 3) {
//		//		if (!tryAutoAnvil(blockAbove0)) {
//		//			blockAbove0.y -= 0;
//		//		}
//			}
//			if (fallblockheight == 4) {
//		//		if (!tryAutoAnvil(blockAbove1)) {
//		//			blockAbove1.y -= 0;
//		//		}
//		//		if (multiplace) {
//		//			if (!tryAutoAnvil(blockAbove0)) {
//		//		//		blockAbove0.y -= 0;
//		//			}
//		//		}
//			}
//			if (fallblockheight == 5) {
//		//		if (!tryAutoAnvil(blockAbove2)) {
//		//			blockAbove2.y -= 0;
//		//		}
//		//		if (multiplace) {
//		//			if (!tryAutoAnvil(blockAbove0)) {
//		//		//		blockAbove0.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove1)) {
//		//		//		blockAbove1.y -= 0;
//		//			}
//		//		}
//			}
//			if (fallblockheight == 6) {
//		//		if (!tryAutoAnvil(blockAbove3)) {
//		//			blockAbove3.y -= 0;
//		//		}
//		//		if (multiplace) {
//		//			if (!tryAutoAnvil(blockAbove0)) {
//		//		//		blockAbove0.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove1)) {
//		//		//		blockAbove1.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove2)) {
//		//		//		blockAbove1.y -= 0;
//		//			}
//		//		}
//			}
//			if (fallblockheight == 7) {
//		//		if (!tryAutoAnvil(blockAbove4)) {
//		//			blockAbove4.y -= 0;
//		//		}
//		//		if (multiplace) {
//		//			if (!tryAutoAnvil(blockAbove0)) {
//		//		//		blockAbove0.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove1)) {
//		//		//		blockAbove1.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove2)) {
//		//		//		blockAbove2.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove3)) {
//		//		//		blockAbove2.y -= 0;
//		//			}
//		//		}
//			}
//			if (fallblockheight == 8) {
//		//		if (!tryAutoAnvil(blockAbove5)) {
//		//			blockAbove5.y -= 0;
//		//		}
//		//		if (multiplace) {
//		//			if (!tryAutoAnvil(blockAbove0)) {
//		//		//		blockAbove0.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove1)) {
//		//		//		blockAbove1.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove2)) {
//		//		//		blockAbove2.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove3)) {
//		//		//		blockAbove3.y -= 0;
//		//			}
//		//			if (!tryAutoAnvil(blockAbove4)) {
//		//		//		blockAbove4.y -= 0;
//		//			}
//		//		}
//			}
//			supplies->selectedHotbarSlot = slotab;
//		}
//	}
//}
void AutoAnvil::onTick(GameMode* gm) {
	targetListav.clear();
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (!Game.canUseMoveKeys())
		return;
	Game.forEachEntity(findEntityav);

	if (!targetListav.empty()) {
		Vec3 blockAbove0 = targetListav[0]->eyePos0;
		Vec3 blockAbove1 = targetListav[0]->eyePos0;
		Vec3 blockAbove2 = targetListav[0]->eyePos0;
		Vec3 blockAbove3 = targetListav[0]->eyePos0;
		Vec3 blockAbove4 = targetListav[0]->eyePos0;
		Vec3 blockAbove5 = targetListav[0]->eyePos0;;
		Vec3 blockAboveH0 = targetListav[0]->eyePos0;
		Vec3 blockAboveH1 = targetListav[0]->eyePos0;
		Vec3 blockAboveH2 = targetListav[0]->eyePos0;
		Vec3 blockAboveH3 = targetListav[0]->eyePos0;
		Vec3 blockAboveH4 = targetListav[0]->eyePos0;
		Vec3 blockAboveH5 = targetListav[0]->eyePos0;
		Vec3 blockAboveH6 = targetListav[0]->eyePos0;
		Vec3 blockAboveH7 = targetListav[0]->eyePos0;
		Vec3 blockAboveH8 = targetListav[0]->eyePos0;
		//head 0 
		blockAboveH8.y -= targetListav[0]->height - 0;
		blockAboveH8.x -= targetListav[0]->height - 1;
		blockAboveH8.z -= targetListav[0]->height - 2;
		//head 1
		blockAboveH0.y -= targetListav[0]->height - 1;
		blockAboveH0.x -= targetListav[0]->height - 1;
		blockAboveH0.z -= targetListav[0]->height - 2;
		//head 2
		blockAboveH1.y -= targetListav[0]->height - 2;
		blockAboveH1.x -= targetListav[0]->height - 1;
		blockAboveH1.z -= targetListav[0]->height - 2;
		//head 3
		blockAboveH2.y -= targetListav[0]->height - 3;
		blockAboveH2.x -= targetListav[0]->height - 1;
		blockAboveH2.z -= targetListav[0]->height - 2;
		//head 4
		blockAboveH3.y -= targetListav[0]->height - 4;
		blockAboveH3.x -= targetListav[0]->height - 1;
		blockAboveH3.z -= targetListav[0]->height - 2;
		//head 5
		blockAboveH4.y -= targetListav[0]->height - 5;
		blockAboveH4.x -= targetListav[0]->height - 1;
		blockAboveH4.z -= targetListav[0]->height - 2;
		//head 6
		blockAboveH5.y -= targetListav[0]->height - 6;
		blockAboveH5.x -= targetListav[0]->height - 1;
		blockAboveH5.z -= targetListav[0]->height - 2;
		//head 7
		blockAboveH6.y -= targetListav[0]->height - 7;
		blockAboveH6.x -= targetListav[0]->height - 1;
		blockAboveH6.z -= targetListav[0]->height - 2;
		//head 8
		blockAboveH7.y -= targetListav[0]->height - 8;
		blockAboveH7.x -= targetListav[0]->height - 1;
		blockAboveH7.z -= targetListav[0]->height - 2;
		//Anvil 3
		blockAbove0.y -= targetListav[0]->height - 3;
		//Anvil 4
		blockAbove1.y -= targetListav[0]->height - 4;
		//Anvil 5
		blockAbove2.y -= targetListav[0]->height - 5;
		//Anvil 6
		blockAbove3.y -= targetListav[0]->height - 6;
		//Anvil 7
		blockAbove4.y -= targetListav[0]->height - 7;
		//Anvil 8
		blockAbove5.y -= targetListav[0]->height - 8;
		// Silent Code
		int slotab = 0;
		PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
		Inventory* inv = supplies->inventory;
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
		if (!tryAutoAnvil(blockAboveH8)) {
			blockAboveH8.x -= 0;
			blockAboveH8.z -= 0;
		}
		if (!tryAutoAnvil(blockAboveH0)) {
			blockAboveH1.x -= 0;
			blockAboveH1.z -= 0;
		}
		if (!tryAutoAnvil(blockAboveH1)) {
			blockAboveH1.x -= 0;
			blockAboveH1.z -= 0;
		}
		if (!tryAutoAnvil(blockAboveH2)) {
			blockAboveH2.x -= 0;
			blockAboveH2.z -= 0;
		}
		supplies->selectedHotbarSlot = slotab;
		slotab = 0;
		slotab = supplies->selectedHotbarSlot;
		for (int n = 0; n < 9; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->item != nullptr) {
				if (BlockID.getSelectedValue() == 1) {
					if (stack->getItem()->itemId == 145) {
						supplies->selectedHotbarSlot = n;
						break;
					}
				}
				if (BlockID.getSelectedValue() == 2) {
					if (stack->getItem()->itemId == 12) {
						supplies->selectedHotbarSlot = n;
						break;
					}
				}
				if (BlockID.getSelectedValue() == 3) {
					if (stack->getItem()->itemId == 13) {
						supplies->selectedHotbarSlot = n;
						break;
					}
				}
				if (BlockID.getSelectedValue() == 4) {
					if (stack->getItem()->itemId == 252) {
						supplies->selectedHotbarSlot = n;
						break;
					}
				}
			}
		}
		if (!tryAutoAnvil(blockAbove0)) {
			blockAbove0.y -= 0;
		}
		supplies->selectedHotbarSlot = slotab;
	}
}

void AutoAnvil::onLevelRender() {}