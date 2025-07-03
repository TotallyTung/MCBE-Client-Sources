#pragma once
#include "Module.h"
class PacketGapple : public IModule {
public:
	int backSlot = 4;
	int health = 6;

	PacketGapple::PacketGapple() : IModule(0, Category::PLAYER, "PacketGapple") {
		registerIntSetting("BackSlot", &backSlot, backSlot, 1, 9);
		registerIntSetting("Health", &health, health, 1, 20);

	}
	PacketGapple::~PacketGapple(){}
	static int getGapple(LocalPlayer* localPlayer) {
		PlayerInventoryProxy* supplies = localPlayer->getSupplies();
		Inventory* inv = supplies->inventory;
		for (int n = 0; n < 36; n++) {
			ItemStack* stack = inv->getItemStack(n);
			if (stack->isValid()) {
				if (stack->getItem()->itemId == 259) {
					return n;
				}
			}
		}
		return -1;
	}

	virtual void onTick(GameMode* gm) override;
	virtual const char* getModuleName() {
		return ("PacketGapple");
	}

};