#pragma once
#include "../../SDK/Game.h"

class PlayerInventory;
class Inventory;
class ItemStack;
class InventoryTransactionManager;
class InventoryUtils {
public:
	static PlayerInventory* getPlayerInventory() {
		if (Game::ourPlayer() == nullptr) return nullptr;
		return Game::ourPlayer()->getplayerInventory();
	}
	static InventoryTransactionManager* getInventoryManager() {
		return Game::ourPlayer()->getTransactionManager();
	}
	static ItemStack* getBadItemStack(int slot) {
		return getPlayerInventory()->container->getItem(slot);
	}
	static ItemStack* getItemStack(int slot) {
		try {
			ItemStack* stack = getBadItemStack(slot);
			if (stack == nullptr) return nullptr;
			if (!stack->isValid()) return nullptr;
			if (!stack->item) return nullptr;
			return stack;
		}
		catch (const std::exception& e) {
		}
		return nullptr;
	}
public:

	static int getSelectedSlot() {
		return getPlayerInventory()->selectedSlot;
	}

	static int getItem(int itemId) {
		try {
			for (int i = 0; i < 9; i++) {
				ItemStack* item = Game::ourPlayer()->playerInventory->container->getItem(i);
				if (!item) continue;
				if (!item->isValid()) continue;
				if (!item->item) continue;
				if (item->item->itemId != itemId) continue;
				return i;
			}
		}
		catch (const std::exception& e) {
		}
		return getSelectedSlot();
	}

	static int getItem(std::string itemName) {
		for (int i = 0; i < 9; i++) {
			ItemStack* item = Game::ourPlayer()->playerInventory->container->getItem(i);
			if (!item) continue;
			if (!item->isValid()) continue;
			if (!item->item) continue;
			if (item->item->texture_name != itemName) continue;
			return i;
		}
		return getSelectedSlot();
	}

	static bool hasItem(int itemId, bool hotbarOnly = true) {
		for (int i = 0; i < hotbarOnly ? 9 : 36; i++) {
			ItemStack* item = Game::ourPlayer()->playerInventory->container->getItem(i);
			if (!isItemValid(item)) continue;
			if (item->item->itemId != itemId) continue;
			return true;
		}
		for (int i = hotbarOnly ? 9 : 36; i > 0; i--) {
			ItemStack* item = Game::ourPlayer()->playerInventory->container->getItem(i);
			if (!isItemValid(item)) continue;
			if (item->item->itemId != itemId) continue;
			return true;
		}
		return false;
	}

	static void SwitchTo(int hotbarSlot) {
		if (getPlayerInventory() == nullptr) {
			return;
		}
		try {
			getPlayerInventory()->selectedSlot = hotbarSlot;
		}
		catch (const std::exception& e) {
		}
	}
	static ItemStack* getHeldItem() {
		return Game::ourPlayer()->getCarriedItem(); // lol
	}
	static bool isItemValid(ItemStack* stack) {
		if (stack == nullptr) return false;
		if (!stack->isValid()) return false;
		if (!stack->item) return false;
		return true;
	}
	static int getSelectedItemId() {
		if (!getHeldItem()) return -1;
		if (!getHeldItem()->item) return -1;
		return getHeldItem()->item->itemId;
	}
};