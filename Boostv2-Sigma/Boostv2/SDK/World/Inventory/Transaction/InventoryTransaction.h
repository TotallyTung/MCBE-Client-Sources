#pragma once
#include "../../../../Utils/MemoryUtil.h"
#include "InventorySource.h"
#include "InventoryAction.h"
#include <vector>
#include <unordered_map>

class InventoryTransaction {
public:
	std::unordered_map<InventorySource, std::vector<InventoryAction>> actions;
	std::vector<__int64> items;
public:
	void addAction(InventoryAction const& action) {
		using func_t = void(__thiscall*)(InventoryTransaction*, InventoryAction const&);
		static func_t Func = reinterpret_cast<func_t>(Addresses::InventoryTransaction_addAction);
		Func(this, action);
	}
};