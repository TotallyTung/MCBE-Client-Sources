#pragma once
#include "../../../Utils/MemoryUtil.h"

class ItemStackBase;
class ItemStack;
class Block;

class Item {
public:
	CLASS_MEMBER(std::string, texture_name, 0x60);
	CLASS_MEMBER(uint16_t, itemId, 0xA2);
public:

	short getDamageValue(__int64 userData) {
		using func_t = short(__thiscall*)(Item*, __int64);
		static func_t Func = reinterpret_cast<func_t>(Addresses::Item_getDamageValue);
		return Func(this, userData);
	}

	bool isFood() {
		return MemoryUtil::CallVFunc<18, bool>(this);
	}

	bool isThrowable() {
		return MemoryUtil::CallVFunc<19, bool>(this);
	}

	bool isUseable() {
		return MemoryUtil::CallVFunc<20, bool>(this);
	}
	short getMaxDamage() {
		return MemoryUtil::CallVFunc<34, short>(this);
	}

	int getAttackDamage() {
		return MemoryUtil::CallVFunc<35, int>(this);
	}

	short setMaxUseDuration(int number) {
		return MemoryUtil::CallVFunc<25, short, int>(this, number);
	}

	bool isGlint(ItemStackBase* itemStackBase) {
		return MemoryUtil::CallVFunc<38, bool, ItemStackBase*>(this, itemStackBase);
	}

	int getArmorValue() {
		return MemoryUtil::CallVFunc<52, int>(this);
	}

	float getDestroySpeed(ItemStackBase* item, Block* block) {
		return MemoryUtil::CallVFunc<77, float, ItemStackBase*, Block*>(this, item, block);
	}
};