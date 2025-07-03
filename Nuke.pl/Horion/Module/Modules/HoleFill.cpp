#include "HoleFill.h"

HoleFill::HoleFill() : IModule(0x0, Category::COMBAT, "Locate a good place to holecamp ;)") {
	registerEnumSetting("Block", &mode, 0);
	mode.addEntry("Obsidian", 0);
	mode.addEntry("Cobweb", 1);
	registerBoolSetting("Obsidian Hole", &this->obsidian, this->obsidian);
	registerBoolSetting("Bedrock Hole", &this->bedrock, this->bedrock);
	registerBoolSetting("OnClick", &this->onclick, this->onclick);
	registerIntSetting("Range", &this->range, 1, 3.f, 15.f);
}
int place = 1;
HoleFill::~HoleFill() {
}

const char* HoleFill::getModuleName() {
	return ("HoleFill");
}
bool HoleFill::canpla(Vec3i blockPos) {
	if (Game.getLocalPlayer()->getPos()->dist(blockPos.toVec3t()) >= 7) {
		return false;
	}
	else return true;
}

static std::vector<Entity*> targetListhf;
void findEntityhf(Entity* currentEntityhf, bool isRegularEntity) {
	static auto AutoTrapMod = moduleMgr->getModule<AutoTrap>();

	if (currentEntityhf == nullptr)
		return;

	if (currentEntityhf == Game.getLocalPlayer())  // Skip Local player
		return;

	if (!Game.getLocalPlayer()->canAttack(currentEntityhf, false))
		return;

	if (!Game.getLocalPlayer()->isAlive())
		return;

	if (!currentEntityhf->isAlive())
		return;


	if (!Target::isValidTarget(currentEntityhf))
		return;

	float dist = (*currentEntityhf->getPos()).dist(*Game.getLocalPlayer()->getPos());
	if (dist < AutoTrapMod->range) {
		targetListhf.push_back(currentEntityhf);

		float dist = (*currentEntityhf->getPos()).dist(*Game.getLocalPlayer()->getPos());

		if (dist < AutoTrapMod->range) {
			targetListhf.push_back(currentEntityhf);
		}
	}
}

bool HoleFill::tryHoleFill(Vec3i blkPlacement) {
	Block* block = Game.getLocalPlayer()->region->getBlock(Vec3i(blkPlacement));
	BlockLegacy* blockLegacy = (block->blockLegacy);
	if (blockLegacy->material->isReplaceable) {
		Vec3i blok(blkPlacement);
		int i = 0;

		static std::vector<Vec3i*> checklist;
		bool foundCandidate = false;
		if (checklist.empty()) {
			checklist.push_back(new Vec3i(0, -1, 0));
			checklist.push_back(new Vec3i(0, 1, 0));

			checklist.push_back(new Vec3i(0, 0, -1));
			checklist.push_back(new Vec3i(0, 0, 1));

			checklist.push_back(new Vec3i(-1, 0, 0));
			checklist.push_back(new Vec3i(1, 0, 0));
		}

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
void HoleFill::onTick(GameMode* gm) {
	if (!(Game.getLocalPlayer() == nullptr || !GameData::canUseMoveKeys())) {
		if (Game.isInGame()) {
			targetListhf.clear();
			Game.forEachEntity(findEntityhf);
			if (!targetListhf.empty()) {
				Vec3* pos = targetListhf[0]->getPos();
				for (int x = (int)pos->x - range; x < pos->x + range; x++) {
					for (int z = (int)pos->z - range; z < pos->z + range; z++) {
						for (int y = (int)pos->y - range; y < pos->y + range; y++) {
							Vec4 underline;
							Vec3 blockPos = Vec3(x, y, z);
							Vec3 air = Vec3(x, y + 1, z);
							Vec3 air2 = Vec3(x, y + 2, z);
							Vec3 blockPos2 = Vec3(x + 1, y, z);
							Vec3 blockPos3 = Vec3(x, y, z + 1);
							Vec3 blockPos4 = Vec3(x, y - 1, z);
							Vec3 blockPos5 = Vec3(x - 1, y, z);
							Vec3 blockPos6 = Vec3(x, y, z - 1);
							Vec3 blockPos22 = Vec3(x + 1, y + 1, z);
							Vec3 blockPos32 = Vec3(x, y + 1, z + 1);
							Vec3 blockPos52 = Vec3(x - 1, y + 1, z);
							Vec3 blockPos62 = Vec3(x, y + 1, z - 1);
							Block* block = Game.getLocalPlayer()->region->getBlock(blockPos);
							Block* airpos = Game.getLocalPlayer()->region->getBlock(air);
							Block* airpos2 = Game.getLocalPlayer()->region->getBlock(air2);
							Block* block2 = Game.getLocalPlayer()->region->getBlock(blockPos2);
							Block* block3 = Game.getLocalPlayer()->region->getBlock(blockPos3);
							Block* block4 = Game.getLocalPlayer()->region->getBlock(blockPos4);
							Block* block5 = Game.getLocalPlayer()->region->getBlock(blockPos5);
							Block* block6 = Game.getLocalPlayer()->region->getBlock(blockPos6);
							Block* block22 = Game.getLocalPlayer()->region->getBlock(blockPos22);
							Block* block32 = Game.getLocalPlayer()->region->getBlock(blockPos32);
							Block* block52 = Game.getLocalPlayer()->region->getBlock(blockPos52);
							Block* block62 = Game.getLocalPlayer()->region->getBlock(blockPos62);
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
								PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
								Inventory* inv = supplies->inventory;
								int slotab = 0;
								if (blockId == 0) obs = true;
								if (bedrock) {
									if ((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0)) {
										slotab = 0;
										slotab = supplies->selectedHotbarSlot;
										for (int n = 0; n < 9; n++) {
											ItemStack* stack = inv->getItemStack(n);
											if (stack->item != nullptr) {
												if (mode.getSelectedValue() == 0) {
													if (stack->getItem()->itemId == 49) {
														supplies->selectedHotbarSlot = n;
														break;
													}
												}
												if (mode.getSelectedValue() == 1) {
													if (stack->getItem()->itemId == 30) {
														supplies->selectedHotbarSlot = n;
														break;
													}
												}
											}
										}
										if (!onclick) {
											place = 0;
										}
										if (onclick) {
											if (GameData::isRightClickDown()) {
												place = 0;
											}
											else {
												place = 1;
											}
										}
										if (place == 0) {
											if (canpla(blockPos)) {
												tryHoleFill(blockPos);
											}
										}
										supplies->selectedHotbarSlot = slotab;
									}
								}
								if (obsidian) {
									if ((blockId2 == 49 || blockId2 == 7) && (blockId3 == 49 || blockId3 == 7) && (blockId4 == 49 || blockId4 == 7) && (blockId5 == 49 || blockId5 == 7) && (blockId6 == 49 || blockId6 == 7) && (blockId == 0) && (aircheck == 0) && !((blockId2 == 7 && blockId3 == 7 && blockId4 == 7 && blockId5 == 7 && blockId6 == 7 && blockId == 0) && (aircheck == 0))) {
										slotab = 0;
										slotab = supplies->selectedHotbarSlot;
										for (int n = 0; n < 9; n++) {
											ItemStack* stack = inv->getItemStack(n);
											if (stack->item != nullptr) {
												if (mode.getSelectedValue() == 0) {
													if (stack->getItem()->itemId == 49) {
														supplies->selectedHotbarSlot = n;
														break;
													}
												}
												if (mode.getSelectedValue() == 1) {
													if (stack->getItem()->itemId == 30) {
														supplies->selectedHotbarSlot = n;
														break;
													}
												}
											}
										}
										if (!onclick) {
											place = 0;
										}
										if (onclick) {
											if (GameData::isRightClickDown()) {
												place = 0;
											}
											else {
												place = 1;
											}
										}
										if (place == 0) {
											if (canpla(blockPos)) {
												tryHoleFill(blockPos);
											}
										}
										supplies->selectedHotbarSlot = slotab;;
									}
								}
							}
						}
					}
				}
			}
		}
	}
}