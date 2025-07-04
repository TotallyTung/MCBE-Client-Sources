#pragma once
#include "ItemStackBase.h"
#include "ItemStackNetIdVariant.h"

class ItemStack : public ItemStackBase {
public:
	ItemStackNetIdVariant mNetIdVariant;

	ItemStack() {
		memset(this, 0x0, sizeof(ItemStack));
		mValid = true;
		mShowPickUp = true;
		Vtable = (uintptr_t**)Addresses::ItemStackVTable;
		setNull(std::nullopt);
	}

	ItemStack(const ItemStack& copy) {
		memset(this, 0x0, sizeof(ItemStack));
		using ItemStackBaseCopyConstructor_t = void(__thiscall*)(ItemStackBase*, const ItemStackBase&);
		static ItemStackBaseCopyConstructor_t ItemStackBaseCopyConstructor = reinterpret_cast<ItemStackBaseCopyConstructor_t>(Addresses::ItemStackBase_CopyConstructor);
		ItemStackBaseCopyConstructor(this, copy);
		this->mNetIdVariant = copy.mNetIdVariant;
		Vtable = (uintptr_t**)Addresses::ItemStackVTable;
	}

	static ItemStack* getEmptyItem() {
		return reinterpret_cast<ItemStack*>(Addresses::ItemStack_EMPTY_ITEM);
	}

	float getDestroySpeed(Block* block) {
		using func_t = float(__thiscall*)(ItemStack*, Block*);
		static func_t Func = reinterpret_cast<func_t>(Addresses::ItemStack_getDestroySpeed);
		return Func(this, block);
	}
};