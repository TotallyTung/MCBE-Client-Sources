// Created by Tony on 2024-10-10 10:01:54

#pragma once

#include "InventorySource.h"
#include "NetworkItemStackDescriptor.h"
namespace std {
	template <>
	struct hash<InventorySource> {
		constexpr uint64_t operator()(InventorySource const& source) const noexcept {
			return (uint64_t)source.type << 16 ^ (uint64_t)source.container;
		}
	};
}  // namespace std

class InventoryAction {
public:
	InventoryAction() = default;

	InventoryAction(uint32_t slot, ItemStack* sourceItem, ItemStack* targetItem, ContainerID containerId = ContainerID::Inventory, InventorySource::Type sourceType = InventorySource::Type::Container, InventorySource::Flags flags = InventorySource::Flags::NoFlag) {
		memset(this, 0x0, sizeof(InventoryAction));

		this->slot = slot;
		if (sourceItem) this->sourceItem = *sourceItem;
		if (targetItem) this->targetItem = *targetItem;
		this->sourceItemDescriptor.fromStack(&this->sourceItem);
		this->targetItemDescriptor.fromStack(&this->targetItem);
		this->source.type = sourceType;
		this->source.container = containerId;
		this->source.flags = flags;
	}

public:
	InventorySource source;								// 0x0
	uint32_t slot;										// 0xC
	NetworkItemStackDescriptor sourceItemDescriptor;	// 0x10
	NetworkItemStackDescriptor targetItemDescriptor;	// 0x68
	ItemStack sourceItem;								// 0xC0
	ItemStack targetItem;								// 0x160
};
