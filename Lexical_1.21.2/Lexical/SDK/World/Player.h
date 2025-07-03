#pragma once
#include "Mob.h"
#include "GameMode.h"
#include "Inventory/PlayerInventory.h"
#include "Inventory/Transaction/InventoryTransactionManager.h"
#include "Inventory/Transaction/ComplexInventoryTransaction.h"

class Player : public Mob {
public:
	CLASS_MEMBER(PlayerInventory*, playerInventory, 0x760);
	CLASS_MEMBER(GameMode*, gameMode, 0xEC8);
public:
	InventoryTransactionManager* getTransactionManager() {
		return reinterpret_cast<InventoryTransactionManager*>((uintptr_t)(this) + 0xE98);
	}
	 void playEmote(std::string const& sex, bool t){
		return MemoryUtil::CallVFunc<70, void, std::string const&, bool>(this, sex, t);
	}
	int getItemUseDuration() {
		return MemoryUtil::CallVFunc<162, int>(this);
	}
};
