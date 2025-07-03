
#include "Offhand.h"
#include "../ModuleManager.h"
#include <chrono>
#include <thread>
using namespace std;
int offDelay = 0;
int offhandDelay = 0;
int offhandTimeout = 0;
AutoTotem::AutoTotem() : IModule(0, Category::PLAYER, "Automatically puts totems in offhand") {
	registerEnumSetting("Mode", &cc, 0);
	cc.addEntry("Totem", 0);
	cc.addEntry("Shield", 1);
	registerEnumSetting("Notify", &mode, 0);
	mode.addEntry("Off", 0);
	mode.addEntry("Chat", 1);
	mode.addEntry("Notification", 2);
}

const char* AutoTotem::getModuleName() {
	return ("Offhand");
}
int countItemaaa(int countitemid)
{
	int mafs = 0;
	for (int i = 0; i < 36; i++)
	{
		ItemStack* itemToCount = Game.getLocalPlayer()->getSupplies()->inventory->getItemStack(i);
		if (itemToCount->item != NULL && (*itemToCount->item)->itemId == countitemid)
		{
			int itemCountC = itemToCount->count;
			mafs += itemCountC;
		}
	}
	return mafs;
};
const char* AutoTotem::getModName() {
	string Shield;
	string Totem;
	string name;
	if (cc.getSelectedValue() == 0) {
		Totem = to_string(countItemaaa(568));
		name = string("Totem ") + Totem;
	}
	if (cc.getSelectedValue() == 1) {
		Shield = to_string(countItemaaa(355));
		name = string("Shield ") + Shield;
	}
	return (name.c_str());
}
void AutoTotem::offhandNotify() {
	string ItemString;
	if (cc.getSelectedValue() == 0) {
		ItemString = "totem";
	} else {
		ItemString = "Shield";
	};
	if (mode.getSelectedValue() == 1) {
		Game.getClientInstance()->getGuiData()->displayClientMessageF("<%sOffhand%s> %sAdding %s to offhand", GREEN, WHITE, DARK_GREEN, ItemString);
	}
	else if (mode.getSelectedValue() == 2) {
		Notifications::addNotifBox("Adding " + ItemString + " to offhand", 2.f);
	};
	return;
}
bool NotificationsStatus = false;
void AutoTotem::runNukkit() {
	auto player = Game.getLocalPlayer();
	if (player == nullptr) return;
	PlayerInventoryProxy* supplies = player->getSupplies();
	Inventory* inv = supplies->inventory;
	ItemStack* current = player->getOffhandItem();
	for (int i = 0; i < 105; i++) {
		int ItemID = 568;
		string ItemString;
		if (cc.getSelectedValue() == 0) {
			ItemString = "totem";
			ItemID = 568;
		}
		else {
			ItemString = "Shield";
			ItemID = 355;
		};
		if (current->item == NULL) {
//			for (int i = 1; i < 36; i++) {
				ItemStack* totem = inv->getItemStack(i);
				if (totem->item != NULL && (*totem->item)->itemId == ItemID) {
					player->setOffhandSlot(totem);
					inv->removeItem(i, 1);
					if (!NotificationsStatus) {
						player->popcount += 1;
						offhandNotify();
						if (moduleMgr->getModule<PopCounter>()->cys && moduleMgr->getModule<PopCounter>()->isEnabled()) {
							clientMessageF("%s%sYou %sPopped %s%i%s %s!", Utils::getCommandClientName(), RED, WHITE, GREEN, player->popcount, WHITE, ItemString);
							Notifications::addNotifBox(std::string("You Popped " + std::to_string(player->popcount) + " " + ItemString + "!"), 4.f);
						}
						NotificationsStatus = true;
					}
				}
//			}
		}
	}
}
void AutoTotem::onTick(GameMode* gm) {
	runNukkit();
}