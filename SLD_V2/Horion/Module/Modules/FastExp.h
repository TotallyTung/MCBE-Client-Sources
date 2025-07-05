#pragma once
#include "../ModuleManager.h"
#include "Module.h"
class FastXP : public IModule {
private:
	int delay = 0;
	int Odel = 0;

public:
	FastXP() : IModule(0x0, Category::PLAYER, "we do the funi super fast"){
		registerIntSetting("Delay (ticks)", &this->delay, this->delay, 0, 25);
	};
	~FastXP(){};

	void onTick(C_GameMode* gm) {
		C_ItemStack* stack = gm->player->getSupplies()->inventory->getItemStack(g_Data.getLocalPlayer()->getSupplies()->selectedHotbarSlot);
		if (GameData::isRightClickDown() && stack != nullptr && (g_Data.getLocalPlayer()->getSelectedItemId() == 508 || g_Data.getLocalPlayer()->getSelectedItemId() == 374 || g_Data.getLocalPlayer()->getSelectedItemId() == 300)) {
			if (Odel >= delay) {
				gm->baseUseItem(*stack);
				Odel = 0;
			}
			Odel++;
		}
		return;
	}

	virtual const char* getModuleName() override {
		return "FastXP";
	}
};