#pragma once

#include "InventoryAction.h"

class InventoryTransactionManager {
public:
	uintptr_t* player;
public:
	void addAction(InventoryAction* action, bool stuff = false) {
		static auto InventoryTransactionManager__addAction = findSig(Sigs::inventorytransactionmanager::addAction);
		reinterpret_cast<void(__fastcall*)(InventoryTransactionManager*, InventoryAction*, bool)>(InventoryTransactionManager__addAction)(this, action, stuff);
	}
};