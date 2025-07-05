#include "AutoTotem.h"

using namespace std;
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems or shield into your offhand") {
	registerEnumSetting("Mode", &mode, 0);
	mode.addEntry(EnumEntry("Nuke", 0))
		.addEntry(EnumEntry("Normal", 1));
}

const char* AutoTotem::getModuleName() {
	return ("AutoTotem");
}

void AutoTotem::onEnable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}

void AutoTotem::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;

	if (mode.selected == 0) {
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
	if (mode.selected == 1) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		C_InventoryTransactionManager* manager = g_Data.getLocalPlayer()->getTransactionManager();
		C_ItemStack* current = g_Data.getLocalPlayer()->getEquippedTotem();
		if (current->item == NULL) {
			for (int i = 0; i < 36; i++) {
				C_ItemStack* totem = inv->getItemStack(i);
				if (totem->item != NULL && (*totem->item)->itemId == 568) {
					C_InventoryAction first(i, totem, nullptr);
					C_InventoryAction second(37, nullptr, totem);
					manager->addInventoryAction(first);
					manager->addInventoryAction(second);
					g_Data.getLocalPlayer()->setOffhandSlot(totem);
				}
			}
		}
	}
}

void AutoTotem::onDisable() {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) return;
}