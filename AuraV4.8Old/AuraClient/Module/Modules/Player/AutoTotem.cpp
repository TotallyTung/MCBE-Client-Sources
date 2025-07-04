#include "AutoTotem.h"
#include "../pch.h"

using namespace std;
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems or shield into your offhand") {
	registerEnumSetting("Mode", &cc, 0);
	cc.addEntry("Totem", 0);
	cc.addEntry("Shield", 1);
}

const char* AutoTotem::getModuleName() {
	return ("Offhand");
}

void AutoTotem::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void AutoTotem::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
	
	if (cc.getSelectedValue() == 0) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		C_ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();
		if (current->item == NULL) {
			for (int i = 1; i < 36; i++) {
				C_ItemStack* totem = inv->getItemStack(i);
				if (totem->item != NULL && (*totem->item)->itemId == 568) {
					g_Data.getLocalPlayer()->setOffhandSlot(totem);
					inv->removeItem(i, 1);
					break;
				}
			}
		}
	}
	else
	{
		C_PlayerInventoryProxy* suppliesg = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* invg = suppliesg->inventory;
		C_ItemStack* currentg = g_Data.getLocalPlayer()->getEquippedTotem();
		if (currentg->item == NULL) {
			for (int cc = 1; cc < 36; cc++) {
				C_ItemStack* totemcc = invg->getItemStack(cc);
				if (totemcc->item != NULL && (*totemcc->item)->itemId == 355) {
					g_Data.getLocalPlayer()->setOffhandSlot(totemcc);
					invg->removeItem(cc, 1);
					break;
				}
			}
		}
	}
}

void AutoTotem::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}