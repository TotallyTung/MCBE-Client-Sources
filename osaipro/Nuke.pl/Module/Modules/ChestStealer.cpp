#include "ChestStealer.h"

using namespace std;
ChestStealer::ChestStealer() : IModule(0, Category::PLAYER, "Takes/Dumps items from chests") {
	// registerBoolSetting("Auto Disable", &autoDisable, autoDisable);
	registerBoolSetting("Enhanced", &enhanced, enhanced);
	registerBoolSetting("Dump", &dump, dump);
	registerIntSetting("Delay", &setDelay, setDelay, 0, 30);
}
ChestStealer::~ChestStealer() {
}
const char* ChestStealer::getModuleName() {
	return "ChestStealer";
}

const char* ChestStealer::getModName() {
	return to_string(setDelay).c_str();
}

void ChestStealer::chestScreenController_tick(ChestScreenController* c) {
	if (dump) {
		if (c != nullptr) {
			delay++;
			vector<int> items = {};
			// auto invcleanerMod = moduleMgr->getModule<InvManager>();
			for (int i = 0; i < 54; i++) {
				ItemStack* stack = c->_getItemStack(TextHolder("inventory_items"), i);
				ItemStack* stack2 = c->_getItemStack(TextHolder("hotbar_items"), i);
				if (stack != nullptr && stack->item != NULL || stack2 != nullptr && stack2->item != NULL) {
					//					if (!enhanced || invcleanerMod->stackIsUseful(stack) || invcleanerMod->stackIsUseful(stack))
					if (!enhanced) {
						items.push_back(i);
					}
				}
			}
			if (!items.empty()) {
				for (int i : items) {
					if (delay > setDelay && setDelay >= 1) {
						c->handleAutoPlace("inventory_items", i);
						c->handleAutoPlace("hotbar_items", i);
						delay = 0;
						return;
					}
				}
			} else
				c->tryExit();
		}
	}

	else {
		if (c != nullptr) {
			delay++;
			vector<int> items = {};
			// auto invcleanerMod = moduleMgr->getModule<InvManager>();
			for (int i = 0; i < 54; i++) {
				ItemStack* stack = c->_getItemStack(TextHolder("container_items"), i);
				if (stack != nullptr && stack->item != NULL) {
					// if (!enhanced || invcleanerMod->stackIsUseful(stack))
					if (!enhanced) {
						items.push_back(i);
					}
				}
			}
			if (!items.empty()) {
				for (int i : items) {
					if (delay > setDelay && setDelay >= 1) {
						c->handleAutoPlace("container_items", i);
						delay = 0;
					} else if (setDelay <= 0)
						c->handleAutoPlace("container_items", i);
				}
			} else
				c->tryExit();
		}
	}
}