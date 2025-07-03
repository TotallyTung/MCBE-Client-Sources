// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../Module.h"

class InventoryCleaner : public Module {
private:
	bool sorted = false;

	std::vector<int> findStackableItems();
	std::vector<int> findUselessItems();
	bool isLastItem(Item* item);

	bool keepTools = true;
	bool keepArmor = true;
	bool keepBlocks = true;
	bool keepFood = true;

	bool openInv = true;
	bool autoSort = false;
public:
	bool stackIsUseful(ItemStack* itemStack);
	InventoryCleaner();

	virtual void onNormalTick(Actor* actor) override;
};
