#pragma once
#include "InventorySource.h"
#include "../../Item/ItemStack.h"
#include "../../Item/NetworkItemStackDescriptor.h"

class InventoryAction {
public:
	InventorySource source;
	uint32_t slot;
	NetworkItemStackDescriptor fromItemDescriptor;
	NetworkItemStackDescriptor toItemDescriptor;
	ItemStack fromItem;
	ItemStack toItem;

	InventoryAction(uint32_t _slot, ItemStack* sourceItem, ItemStack* targetItem, ContainerID containerId = ContainerID::Inventory, InventorySourceType sourceType = InventorySourceType::ContainerInventory, InventorySourceFlags flags = InventorySourceFlags::NoFlag) {
		InventorySource invSource;
		invSource.type = sourceType;
		invSource.containerId = containerId;
		invSource.flags = flags;

		using InvAction_ctt_t = void(__thiscall*)(InventoryAction*, InventorySource*, uint32_t, ItemStack*, ItemStack*);
		static InvAction_ctt_t InvAction_ctt = reinterpret_cast<InvAction_ctt_t>(Addresses::InventoryAction_Constructor);
		InvAction_ctt(this, &invSource, _slot, sourceItem, targetItem);
	}

};