#include "FastEat.h"
FastEat::FastEat() : IModule(0, Category::PLAYER, "AutoStop") {
}

FastEat::~FastEat() {
}

const char* FastEat::getModuleName() {
	return "FastEat";
}

void FastEat::onTick(GameMode* gm) {
	auto timermod = moduleMgr->getModule<Timer>();
	auto timer2mod = moduleMgr->getModule<Timer1>();
	auto timer3mod = moduleMgr->getModule<Timer2>();
	auto _2espeedmod = moduleMgr->getModule<NukeSpeed>();
	auto cbspeedmod = moduleMgr->getModule<CubeSpeed>();
	auto speedmod = moduleMgr->getModule<Speed>();
	auto bhopmod = moduleMgr->getModule<Bhop>();
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	int dbrr2 = Game.getLocalPlayer()->getSelectedItemId();
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		// if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 32) {
		if (stack->item != NULL && (*stack->item)->itemId == 259) {
			(*stack->item)->setMaxUseDuration(32);
		}
		// dbrr == 259 || dbrr == 426
		// }
	}
	if ((dbrr2 == 259 || dbrr2 == 426) && Game.isRightClickDown()) {
		if (timermod->isEnabled()) TimerWasEnabled = true;
		if (timer2mod->isEnabled()) Timer2we = true;
		if (timer3mod->isEnabled()) Timer3we = true;
		if (_2espeedmod->isEnabled()) _2espeedWasEnabled = true;
		if (cbspeedmod->isEnabled()) cbspeedwe = true;
		if (bhopmod->isEnabled()) bhopwe = true;
		if (speedmod->isEnabled()) spwe = true;
		if (TimerWasEnabled) timermod->setEnabled(false);
		if (Timer2we) timer2mod->setEnabled(false);
		if (Timer3we) timer3mod->setEnabled(false);
		if (_2espeedWasEnabled) _2espeedmod->setEnabled(false);
		if (spwe) speedmod->setEnabled(false);
		if (cbspeedwe) cbspeedmod->setEnabled(false);
		if (bhopwe) bhopmod->setEnabled(false);
		Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	} else {
		if (TimerWasEnabled) {
			timermod->setEnabled(true);
			TimerWasEnabled = false;
		}
		if (Timer2we) {
			timer2mod->setEnabled(true);
			Timer2we = false;
		}
		if (Timer3we) {
			timer3mod->setEnabled(true);
			Timer3we = false;
		}
		if (_2espeedWasEnabled) {
			_2espeedmod->setEnabled(true);
			_2espeedWasEnabled = false;
		}
		if (spwe) {
			speedmod->setEnabled(true);
			spwe = false;
		}
		if (bhopwe) {
			bhopmod->setEnabled(true);
			bhopwe = false;
		}
		if (cbspeedwe) {
			cbspeedmod->setEnabled(true);
			cbspeedwe = false;
		}
		if (!TimerWasEnabled) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!Timer2we) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!Timer3we) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!_2espeedWasEnabled) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!spwe) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!cbspeedwe) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
		if (!bhopwe) Game.getClientInstance()->minecraft->setTimerSpeed(20.f);
	}
}

void FastEat::onDisable() {
	if (Game.getLocalPlayer() == nullptr)
		return;
	PlayerInventoryProxy* supplies = Game.getLocalPlayer()->getSupplies();
	Inventory* inv = supplies->inventory;
	for (int i = 0; i < 36; i++) {
		ItemStack* stack = inv->getItemStack(i);
		// if (stack->item != NULL && (*stack->item)->itemId != 261 && (*stack->item)->duration == 5) {
		if (stack->item != NULL && (*stack->item)->itemId == 259) {
			(*stack->item)->setMaxUseDuration(32);
		}
		//}
	}
}
