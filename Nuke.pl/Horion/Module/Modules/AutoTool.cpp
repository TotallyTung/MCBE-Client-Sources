#include "AutoTool.h"

AutoTool::AutoTool() : IModule(0, Category::MISC, "Automatically selects the best tool.") {
	registerBoolSetting("Weapons", &weapon, weapon);
	registerBoolSetting("Tools", &tools, tools);
}

const char* AutoTool::getModuleName() {
	return ("AutoTool");
}

void AutoTool::onEnable() {
	hasClicked = false;
	if (Game.getLocalPlayer() == nullptr) {
		setEnabled(false);
		return;
	}
	prevslot = Game.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
}
bool attacknow = false;
int attacktime = 0;
int prevslotWeapon = 0;
void AutoTool::onAttack(Entity* attackedEnt) {
	auto player = Game.getLocalPlayer();
	if (!weapon) return;
	if (player == nullptr) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	int slot = -1;
	int damage = 0;
	for (int n = 0; n < 9; n++) {
		ItemStack* stack = inv->getItemStack(n);
		if (stack->item != nullptr) {
			float currentDamage = stack->getAttackingDamageWithEnchants();
			if (currentDamage > damage && currentDamage >= 1) {
				damage = currentDamage;
				slot = n;
				supplies->selectedHotbarSlot = n;
			}
		}
	}
	if (slot != -1) {
		if (supplies->selectedHotbarSlot != slot) {
			prevslotWeapon = supplies->selectedHotbarSlot;
			supplies->selectedHotbarSlot = slot;
		}
		attacknow = true;
		attacktime = 0;
	}
}
void AutoTool::onTick(GameMode* gm) {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	if (attacknow) attacktime++;
	if (attacktime >= 3) {
		attacktime = 0;
		attacknow = false;
		supplies->selectedHotbarSlot = prevslotWeapon;
	}
	if (!tools) return;
	if (Game.getLocalPlayer()->region->getBlock(player->level->block)->blockLegacy->blockId == 7 || player->level->block == Vec3(0, 0, 0)) {
		if (hasClicked) {
			supplies->selectedHotbarSlot = prevslot;
			hasClicked = false;
		}
		return;
	}
	if (Game.canUseMoveKeys() && !moduleMgr->getModule<ClickGuiMod>()->isEnabled()) {
		float damage = 0;
		if (GameData::isLeftClickDown()) {
			if (!hasClicked) {
				prevslot = supplies->selectedHotbarSlot;
				hasClicked = true;
			}
			else {
				int slot = -1;
				for (int n = 0; n < 9; n++) {
					ItemStack* stack = inv->getItemStack(n);
					if (stack->item != nullptr) {
						float currentDamage = stack->getItem()->getAttackDamage() + stack->getItem()->getDestroySpeed(*stack, *Game.getLocalPlayer()->region->getBlock(player->level->block));
						bool IsUseful = stack->getItem()->getDestroySpeed(*stack, *Game.getLocalPlayer()->region->getBlock(player->level->block)) <= 32767;
						if (currentDamage > damage && IsUseful && currentDamage >= 1) {
							damage = currentDamage;
							slot = n;
						}
					}
				}
				if (slot != -1) supplies->selectedHotbarSlot = slot;
			}
		}
		else if (hasClicked) {
			supplies->selectedHotbarSlot = prevslot;
			hasClicked = false;
		}
	}
}
