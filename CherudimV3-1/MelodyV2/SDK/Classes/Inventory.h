#pragma once
#include "Container.h"
#include "InventoryAction.h"
#include "InventoryTransactionManager.h"
#include "ItemStack.h"
#include "../../Utils/MemoryUtils.h"
#include <vector>
#include "Level.h"
#include "GameMode.h"
#include "Inventory.h"
#include "Dimension.h"
#include "Actor.h"



class Inventory : public Container {
public:

	std::vector<ItemStack> stacks; //0x00E0

	int getEmptySlotFromContainer()
	{
		for (int currentSlot = 0; currentSlot < stacks.size(); currentSlot++)
		{
			const ItemStack& stack = this->stacks[currentSlot];
			if (!stack.item)
			{
				return currentSlot;
			}
		}
		return -1;
	}

	int getEmptySlotFromHotbar()
	{
		for (int inventoryRow = 3; inventoryRow > 0; inventoryRow--)
		{
			for (int currentSlot = 9 * inventoryRow; currentSlot < 9 * (inventoryRow + 1); currentSlot++)
			{
				const ItemStack& stack = this->stacks[currentSlot];
				if (!stack.item)
				{
					return currentSlot;
				}
			}
		}
		return -1;
	}
	/*void dropSlot(int slot) {
		// FillingContainer::dropSlot
		using drop_t = void(__fastcall*)(Inventory*, int, char);
		static drop_t func = reinterpret_cast<drop_t>(findSig(Sigs::Itemstack::Dropslot));
		if (func != 0)
			func( slot, 0);
	}*/
	void dropSlot(int slot) {
		using func_t = void(__thiscall*)(Inventory*,int, char, char);
		static func_t func = (func_t)findSig(Sigs::Itemstack::Dropslot);
		return func(this,slot, 0,0);
	}
	bool isFull() {
		int fullslots = 0;
		for (int i = 0; i < 36; i++) {
			if (this->getItemStack(i)->item)
				fullslots++;
		}
		if (fullslots == 36) return true;
		return false;
	}
	/*void moveItem(int from, int to = -1) {
		InventoryTransactionManager* manager = mc.getLocalPlayer()->getInventoryTransactionManager();

		if (to < 0) {
			to = getFirstEmptySlot();
		}

		ItemStack* item1 = getItemStack(from);
		ItemStack* item2 = getItemStack(to);

		if (!item1->item) return;

		if (!item2->item) {
			InventoryAction first(from, item1, nullptr);
			InventoryAction second(to, nullptr, item1);

			manager->addAction(first);
			manager->addAction(second);

			ItemStack item1Copy{ *item1 };

			*item2 = std::move(item1Copy);
			*item1 = ItemStack::getEmptyItemStack();

			manager->addAction(second);
		}
		else {
			InventoryAction first(from, item1, nullptr);
			InventoryAction second(to, item2, item1);
			InventoryAction third(from, nullptr, item2);

			manager->addAction(first);
			manager->addAction(second);
			manager->addAction(third);

			ItemStack item1Copy{ *item1 }, item2Copy{ *item2 };

			*item2 = std::move(item1Copy);
			*item1 = std::move(item2Copy);

			manager->addAction(third);
		}
	}*/
	void swapSlots(int from, int to, InventoryTransactionManager* manager) {

		ItemStack* i1 = getItemStack(from);
		ItemStack* i2 = getItemStack(to);

		ItemStack i1Copy{ *i1 }, i2Copy{ *i2 };

		InventoryAction first(from, i1, nullptr);
		InventoryAction second(to, i2, i1);
		InventoryAction third(from, nullptr, i2);

		manager->addAction(&first);
		manager->addAction(&second);
		manager->addAction(&third);

		*i2 = std::move(i1Copy);
		*i1 = std::move(i2Copy);

		manager->addAction(&third);
	}

	//TODO#
	//void moveItem(int from, int to);
	//void swapSlots(int from, int to);
};
