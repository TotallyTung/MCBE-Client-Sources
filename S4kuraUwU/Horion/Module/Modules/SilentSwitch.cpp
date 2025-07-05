#include "SilentSwitch.h"

SilentSwitch::SilentSwitch() : IModule(0, Category::COMBAT, "SilentSwitch by hank") {
	mode = SettingEnum(this)
				.addEntry(EnumEntry("Normal", 0))
				.addEntry(EnumEntry("Gap", 1));	
	registerEnumSetting("Mode", &this->mode, 0);
}

SilentSwitch::~SilentSwitch() {
}

const char* SilentSwitch::getModuleName() {
	return ("SilentSwitch");
}

void SilentSwitch::onEnable() {
	prevSlot = g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot;
}


void SilentSwitch::onTick(C_GameMode* gm) {
	if (mode.GetSelectedEntry().GetValue() == 0) {
		gm->player->getSupplies()->selectedHotbarSlot = prevSlot;
	}
	if (mode.GetSelectedEntry().GetValue() == 1) {
		__int64 id = *g_Data.getLocalPlayer()->getUniqueId();
		C_PlayerInventoryProxy* supplies = g_Data.getLocalPlayer()->getSupplies();
		C_Inventory* inv = supplies->inventory;
		int slot = supplies->selectedHotbarSlot;
		for (int n = 0; n < 9; n++) {
			C_ItemStack* stack = inv->getItemStack(n);
			if (stack->item != NULL && (*stack->item)->itemId == 259) {
				slot = n;
			}
			supplies->selectedHotbarSlot = slot;
		}
	}
}