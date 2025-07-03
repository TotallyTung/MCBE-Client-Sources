#include "AntiAnvil.h"
bool apb;
bool apbn;
bool ab;
bool abn;
bool ap;
AntiAnvil::AntiAnvil() : IModule(0x0, Category::PLAYER, "No FallBlock!") {
	registerBoolSetting("AutoPlace Button", &apb, false);
	registerBoolSetting("  ->Notif", &apbn, true);
	registerBoolSetting("AutoPhase", &ap, true);
	registerBoolSetting("AutoBreak", &ab, true);
	registerBoolSetting("  ->Notif", &abn, true);
}
std::string blockname = "None";
const char* AntiAnvil::getModName() {
	std::string name = std::string(blockname);
	return name.c_str();
}
const char* AntiAnvil::getModuleName() {
	return ("AntiAnvil");
}
void AntiAnvil::onEnable() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
}
bool isFallingAnvil(Block* block) {int blockID = (int)block->toLegacy()->blockId;return (blockID == 145);}
bool isFallingConcretePowder(Block* block) {int blockID = (int)block->toLegacy()->blockId;return (blockID == 237);}
bool isFallingGravel(Block* block) {int blockID = (int)block->toLegacy()->blockId;return (blockID == 13);}
bool isFallingSand(Block* block) {int blockID = (int)block->toLegacy()->blockId;return (blockID == 12);}
bool height1(Player* plr)  {
	if (plr == nullptr) return false;

	Vec3i blockpos3 = plr->getPos()->floor();
	blockpos3.y = (int)blockpos3.y - 1;
	Vec3 playerFeetPos = Game.getLocalPlayer()->getPos()->floor();
	Vec3 buttonPos = Vec3(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
	// Check for anvils falling above the player's head
	for (int yOffset = 0; yOffset <= (int)1.f; yOffset++) {  // Use the height value from the slider
		Vec3i blockposAbove1 = blockpos3;
		blockposAbove1.y += yOffset;

		Block* blockAbove1 = Game.getLocalPlayer()->region->getBlock(blockposAbove1);
		int blockAboveID1 = (int)blockAbove1->toLegacy()->blockId;

		if (blockAboveID1 == 145 && isFallingAnvil(blockAbove1)) {
			blockname = "Anvil";
			return true;
		}
		if (blockAboveID1 == 237 && isFallingConcretePowder(blockAbove1)) {
			blockname = "Powder";
			return true;
		}
		if (blockAboveID1 == 13 && isFallingGravel(blockAbove1)) {
			blockname = "Gravel";
			return true;
		}
		if (blockAboveID1 == 12 && isFallingSand(blockAbove1)) {
			blockname = "Sand";
			return true;
		}
	}
	return false;
};
bool height3(Player* plr) {
	if (plr == nullptr) return false;

	Vec3i blockpos3 = plr->getPos()->floor();
	blockpos3.y = (int)blockpos3.y - 1;
	Vec3 playerFeetPos = Game.getLocalPlayer()->getPos()->floor();
	Vec3 buttonPos = Vec3(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
	// Check for anvils falling above the player's head
	for (int yOffset = 0; yOffset <= 3; yOffset++) {  // Use the height value from the slider
		Vec3i blockposAbove1 = blockpos3;
		blockposAbove1.y += yOffset;

		Block* blockAbove1 = Game.getLocalPlayer()->region->getBlock(blockposAbove1);
		int blockAboveID1 = (int)blockAbove1->toLegacy()->blockId;

		if (blockAboveID1 == 145 && isFallingAnvil(blockAbove1)) {
			blockname = "Anvil";
			return true;
		}
		if (blockAboveID1 == 237 && isFallingConcretePowder(blockAbove1)) {
			blockname = "Powder";
			return true;
		}
		if (blockAboveID1 == 13 && isFallingGravel(blockAbove1)) {
			blockname = "Gravel";
			return true;
		}
		if (blockAboveID1 == 12 && isFallingSand(blockAbove1)) {
			blockname = "Sand";
			return true;
		}
	}
	return false;
}; 
static std::vector<Entity*> fallingblocklist;

void findFallBlock(Entity* SelectedEntity, bool isRegularEntity) {
	static auto AntiAnvilMod = moduleMgr->getModule<AntiAnvil>();

	if (SelectedEntity == nullptr) return;
	if (SelectedEntity->getEntityTypeId() != 66) return;
	Vec3 Pos = Game.getLocalPlayer()->getPos()->floor();
	Vec3 EnemyPos = SelectedEntity->getPos()->floor();
	if (EnemyPos.y-1.f == Pos.y) {
		if (EnemyPos.x == Pos.x) {
			if (EnemyPos.z == Pos.z) {
				fallingblocklist.push_back(SelectedEntity);
			};
		};
	}
}
float bpav = 0.f;
void findButtons() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int n = 0; n < 9; n++) {
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
int pib() {
	Vec3 pos = Vec3(Game.getLocalPlayer()->getPos()->floor().x, Game.getLocalPlayer()->getPos()->floor().y - 1, Game.getLocalPlayer()->getPos()->floor().z);
	Block* block = Game.getLocalPlayer()->region->getBlock(pos);
	int blockID = (int)block->toLegacy()->blockId; 
	if (blockID == 143) return 1;
	if (blockID == 399) return 1;
	if (blockID == 396) return 1;
	if (blockID == 398) return 1;
	if (blockID == 395) return 1;
	if (blockID == 397) return 1;
	if (blockID == 77) return 1;
	if (blockID == 55) return 1;
	if (blockID != 143 && blockID != 395 && blockID != 396 && blockID != 397 && blockID != 398 && blockID != 399 && blockID != 77 && blockID != 55) return 2;
	return 0;
};
bool tryPlacePB(Vec3 blkPlacement) {
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
			Game.getGameMode()->buildBlock(&blok, i, true);
			return true;
		}
	}

	return false;
}
bool pbbpb(const Vec3& blockPos) {
	Vec3 blockPosBelow1(blockPos.x - 0, blockPos.y - 1, blockPos.z - 0);
	Vec3 blockPosBelow(blockPos.x - 0, blockPos.y - 2, blockPos.z - 0);
	if (tryPlacePB(blockPosBelow1))
		return true;
	if (tryPlacePB(blockPosBelow))
		return true;
	return false;
}

int slotab = 0;
void findPickaxe() {
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	float damage = 0;
	int slot = supplies->selectedHotbarSlot;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			if (stack->getItem()->isPickaxe()) {
				float currentDamage = stack->getAttackingDamageWithEnchants();
				if (currentDamage > damage) {
					damage = currentDamage;
					slot = n;
				}
			}
		}
	}
	supplies->selectedHotbarSlot = slot;
}
bool breaked;
float lastDestroyRate = 0.0f; 
std::pair<int, float> getBestPickaxeSlot(LocalPlayer* lp, GameMode* gm, Block* block) {
	PlayerInventoryProxy* supplies = lp->getSupplies();
	Inventory* inv = supplies->inventory;

	int bestSlot = supplies->selectedHotbarSlot;
	int oldSlot = supplies->selectedHotbarSlot;
	float destroyRate = gm->getDestroyRate(block);
	for (int i = 0; i < 9; i++) {
		supplies->selectedHotbarSlot = i;
		if (gm->getDestroyRate(block) > destroyRate) {
			destroyRate = gm->getDestroyRate(block);
			bestSlot = i;
		}
	}
	supplies->selectedHotbarSlot = oldSlot;
	if (block->blockLegacy->blockId != 0) {
		lastDestroyRate = destroyRate;
	}
	return std::make_pair(bestSlot, destroyRate);
}
bool test = false;
void AntiAnvil::onTick(GameMode* gm) {
	fallingblocklist.clear();
	auto plr = Game.getLocalPlayer();
	if (plr == nullptr) return;
	bool falledblocks = height1(Game.getLocalPlayer());
	auto phaseModule = moduleMgr->getModule<PhaseForAntiAnvil>();
	Game.forEachEntity(findFallBlock);
	if (apb) {
		if (pib() == 0) {
			test = false;
		}

		if (pib() == 1) {
			if (!test) {
				Notifications::addNotifBox("already placed buttons", 10.f);
				test = true;
			}
		}
		if (pib() == 2) {
			if (!fallingblocklist.empty()) {
				if (apbn) {
					Notifications::addNotifBox("AntiAnvil : Block Detected", 0.4f);
				}
			}
			if (!fallingblocklist.empty()) {
				if (apbn) {
					Notifications::addNotifBox("AntiAnvil : Placeing Button", 1.f);
				}
				PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
				Inventory* inv = supplies->inventory;
				slotab = supplies->selectedHotbarSlot;
				findButtons();
				Vec3 playerFeetPos = Game.getLocalPlayer()->getPos()->floor();
				Vec3 buttonPos = Vec3(playerFeetPos.x, playerFeetPos.y - 1, playerFeetPos.z);
				bool a = pbbpb(playerFeetPos);
				if (apbn && a && fallingblocklist.empty()) {
					Notifications::addNotifBox("AntiAnvil : Placed Button", 1.f);
					a = false;
				}
				if (apbn && a && !fallingblocklist.empty()) {
					Notifications::addNotifBox("AntiAnvil : Button Place Failed ,,, Your Testing on World?? Select Buttons ", 5.f);
					a = false;
				}
				supplies->selectedHotbarSlot = slotab;
			}
		}
	}
	if (falledblocks == true) {
		if (ap) {
			phaseModule->setEnabled(true);
		}
		if (ab) {
			float* bpav = (float*)((__int64)gm + 0x24);
			float bpntf = *bpav;
			Block* block = plr->region->getBlock(Game.getLocalPlayer()->getPos()->floor());
			std::pair<int, float> bestSlot = getBestPickaxeSlot(plr, gm, block);
			Vec3 playerFeetPos = Game.getLocalPlayer()->getPos()->floor();
			if (*bpav < 1.f) {
				*bpav += bestSlot.second;
				if (*bpav > 1.f) *bpav = 1.f;
				if (abn) {
					Notifications::addNotifBox("AntiAnvil : bp + " + std::to_string(bestSlot.second) + " | Now bp : " + std::to_string(bpntf), 2.5f);
				}
			}
			else {
				
				if (block->blockLegacy->blockId == 0) return;
				if (plr->getItemUseDuration() > 0) return;
				if (abn) {
					Notifications::addNotifBox("Breaking Anvil", 0.5f);
				}
				MobEquipmentPacket pk(*plr->getUniqueId(), plr->getSupplies()->inventory->getItemStack(bestSlot.first), bestSlot.first, bestSlot.first);
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&pk);
				PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
				Inventory* inv = supplies->inventory;
				slotab = supplies->selectedHotbarSlot;
				findPickaxe();
				breaked = gm->destroyBlock(&Vec3i(playerFeetPos), 0);
				supplies->selectedHotbarSlot = slotab;
				if (abn) {
					if (breaked && !falledblocks) {
						Notifications::addNotifBox("AntiAnvil : AutoBreaked Anvil", 1.f);
						float* bpav = (float*)((__int64)gm + 0x24);
					}
					if (breaked && falledblocks) {
						Notifications::addNotifBox("AntiAnvil : Your Testing on World?? Select Pickaxe | Break", 2.5f);
					}
				}
			}
		}
	}
	else {
		if (ap) {
			phaseModule->setEnabled(false);
		};
		blockname = "None";
	}
};
void AntiAnvil::onLevelRender() {}