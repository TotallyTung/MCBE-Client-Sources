#include "FastEat.h"

FastEat::FastEat() : IModule(0, Category::PLAYER, "Eat food almost instant") {
	registerEnumSetting("Mode", &Mode, 0);
	Mode.addEntry("Duration", 0);
	Mode.addEntry("Timer", 1);
	registerIntSetting("Duration", &duration, duration, 0, 32);
	registerIntSetting("Timer", &timer, timer, 20, 60);
}

FastEat::~FastEat() {
}

const char* FastEat::getRawModuleName() {
	return "FastEat";
}
const char* FastEat::getModuleName() {
	if (Mode.getSelectedValue() == 0) name = std::string("FastEat ") + std::string(GRAY) + std::string("[") + std::string(WHITE) + std::string("Duration") + std::string(GRAY) + std::string("]");
	else name = std::string("FastEat ") + std::string(GRAY) + std::string("[") + std::string(WHITE) + std::string("Timer") + std::string(GRAY) + std::string("]");
	return name.c_str();
}

void FastEat::onTick(C_GameMode* gm) {
	auto timermod = moduleMgr->getModule<Timer>();
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	int dbrr2 = g_Data.getLocalPlayer()->getSelectedItemId();
	if (Mode.getSelectedValue() == 0) {
		for (int i = 0; i < 36; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			//if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 32) {
			if (stack->item != NULL && (*stack->item)->itemId == 259) {
				(*stack->item)->setMaxUseDuration(duration);
			}
			//dbrr == 259 || dbrr == 426
			//}
		}
	}
	else {
		for (int i = 0; i < 36; i++) {
			C_ItemStack* stack = inv->getItemStack(i);
			//if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 32) {
			if (stack->item != NULL && (*stack->item)->itemId == 259) {
				(*stack->item)->setMaxUseDuration(32);
			}
			//dbrr == 259 || dbrr == 426
			//}
		}
		if ((dbrr2 == 259 || dbrr2 == 426) && g_Data.isRightClickDown()) {
			if (timermod->isEnabled()) TimerWasEnabled = true;
			if (TimerWasEnabled) timermod->setEnabled(false);
			g_Data.getClientInstance()->minecraft->setTimerSpeed(timer);
		}
		else {
			if (TimerWasEnabled) {
				timermod->setEnabled(true);
				TimerWasEnabled = false;
			}
			if (!TimerWasEnabled) g_Data.getClientInstance()->minecraft->setTimerSpeed(20.f);
		}
	}
}

void FastEat::onDisable() {
	if (g_Data.getLocalPlayer() == nullptr)
		return;
	C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
	C_Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		C_ItemStack* stack = inv->getItemStack(i);
		//if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 5) {
		if (stack->item != NULL && (*stack->item)->itemId == 259) {
			(*stack->item)->setMaxUseDuration(32);
		}
		//}
	}
}
