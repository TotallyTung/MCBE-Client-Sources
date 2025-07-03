// Created by Tony on 2024-10-10 10:01:54

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