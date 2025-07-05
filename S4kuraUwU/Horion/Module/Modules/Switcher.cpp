#include "Switcher.h"

Switcher::Switcher() : IModule(0, Category::COMBAT, "Switches between hotbar slots") {
	registerBoolSetting("DoubleSwitch", &ss, ss);
	registerBoolSetting("AutoSwitch", &autos, autos);
	registerIntSetting("Slots", &ternary, 1, 0, 8);
	registerFloatSetting("Delay", &delay,0.1f, 0.1f, 20.1f);
}

const char* Switcher::getModuleName() {
	return ("Switcher");
}

void Switcher::onEnable() {
}
void Switcher::onAttack(C_Entity* attackedEnt) {
	Odelay++;
	if (Odelay >= delay) {
		if (g_Data.getLocalPlayer()->velocity.squaredxzlen() < 0.01) {
			C_MovePlayerPacket p(g_Data.getLocalPlayer(), *g_Data.getLocalPlayer()->getPos());
			g_Data.getClientInstance()->loopbackPacketSender->sendToServer(&p);  // make sure to update rotation if player is standing still
		}
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;

		if (!ownage && !ss) {
			ternary1++;
			if (ternary1 >= ternary + 1) {
				ternary1 = 0;
			}
		}
		supplies->selectedHotbarSlot = ternary1;

		if (ownage && !autos) {
			if (ternaryO == false) {
				if (toggler == false) {
					toggler = true;
					supplies->selectedHotbarSlot = 0;
				} else {
					toggler = false;
					supplies->selectedHotbarSlot = 4;
					ternaryO = true;
				}
			} else {
				supplies->selectedHotbarSlot = 5;
				ternaryO = false;
			}
		}

		if (ss && !autos) {
			if (swap == false) {
				swap = true;
				supplies->selectedHotbarSlot = 0;
			} else {
				swap = false;
				supplies->selectedHotbarSlot = 1;
			}
		}
	}
	Odelay=0;
}

void Switcher::onTick(C_GameMode* gm) {
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
			if (autos && !ownage) {
				ternary1++;
				if (ternary1 >= ternary + 1) {
					ternary1 = 0;
				}
				supplies->selectedHotbarSlot = ternary1;
			}
			if (autos && ownage) {
				if (ternaryO == false) {
					if (toggler == false) {
						toggler = true;
						supplies->selectedHotbarSlot = 0;
					} else {
						toggler = false;
						supplies->selectedHotbarSlot = 4;
						ternaryO = true;
					}
				} else {
					supplies->selectedHotbarSlot = 5;
					ternaryO = false;
				}
			}
			if (ss && autos) {
				if (swap == false) {
					swap = true;
					supplies->selectedHotbarSlot = 0;
				} else {
					swap = false;
					supplies->selectedHotbarSlot = 1;
				}
			}
		}

