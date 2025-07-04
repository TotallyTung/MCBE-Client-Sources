#include "Switcher2.h"
#include "../pch.h"

Switcher2::Switcher2() : IModule(0, Category::COMBAT, "Still Switcher but made by XxTrongDZxX") {
	registerIntSetting("Slot", &slslot, slslot, 2, 9);
	registerBoolSetting("Fast Mode", &fastmode, fastmode);
}

const char* Switcher2::getModuleName() {
	return ("Switcher2");
}

void Switcher2::onTick(C_GameMode* gm) {
	auto player = g_Data.getLocalPlayer();
	if (player == nullptr) {
		return;
	}
	if (!fastmode) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		supplies->selectedHotbarSlot = ffcc;
		ffcc++;
		if (ffcc >= slslot) {
			ffcc = 0;
		}
	}
}

void Switcher2::onPlayerTick(C_Player* plr) {
	if (plr == nullptr) {
		return;
	}
	if (fastmode) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		supplies->selectedHotbarSlot = ffcc;
		ffcc++;
		if (ffcc >= slslot) {
			ffcc = 0;
		}
	}
}

