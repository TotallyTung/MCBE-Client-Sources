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

	int getItemUseDuration() {
		return MemoryUtil::CallVFunc<162, int>(this);
	}
};
