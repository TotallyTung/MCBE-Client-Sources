#pragma once
#include "InventoryAction.h"

class InventoryTransactionManager {
public:
	void addAction(const InventoryAction& action, bool balanced = false) {
		using func_t = void(__thiscall*)(InventoryTransactionManager*, InventoryAction const&, bool);
		static func_t Func = reinterpret_cast<func_t>(Addresses::InventoryTransactionManager_addAction);
		Func(this, action, balanced);
	}
};