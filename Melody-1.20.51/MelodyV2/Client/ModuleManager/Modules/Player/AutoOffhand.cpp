// Created by Tony on 2024-10-10 10:01:54

#include "AutoOffhand.h"

AutoOffhand::AutoOffhand() : Module("Offhand", "Auto switch totems/shields to your offhand.", Category::PLAYER) {
	addEnumSetting("Item", "Select item to switch", {"Totem", "Shield"}, &itemMode);
}

void AutoOffhand::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = (LocalPlayer*)actor;
	GameMode* gm = mc.getGameMode();

	std::string ItemID = itemMode == 0 ? "totem" : "shield"; // Best optimize :cool:
	if (localPlayer->getOffhandSlot()->item == nullptr) {
		for (int i = 0; i < 36; i++) {
			ItemStack* itemStack = localPlayer->getPlayerInventory()->inventory->getItemStack(i);
			if (itemStack != nullptr && itemStack->item != nullptr && itemStack->getItemPtr() != nullptr) {
				if (itemStack->getItemPtr()->texture_name == ItemID) {
					
					localPlayer->setOffhandSlot(itemStack);
					
					localPlayer->getPlayerInventory()->inventory->removeItem(i, i);
					break;
				}
			}
		}
	}
}