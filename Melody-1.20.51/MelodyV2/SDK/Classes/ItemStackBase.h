// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/MemoryUtils.h"
#include "Item.h"
enum class EnchantID : int
{
    PROTECTION = 0,
    FIRE_PROTECTION = 1,
    FEATHER_FALLING = 2,
    BLAST_PROTECTION = 3,
    PROJECTILE_PROTECTION = 4,
    THORNS = 5,
    RESPIRATION = 6,
    DEPTH_STRIDER = 7,
    AQUA_AFFINITY = 8,
    sharpness = 9,
    SMITE = 10,
    BANE_OF_ARTHROPODS = 11,
    KNOCKBACK = 12,
    fire_aspect = 13,
    LOOTING = 14,
    EFFICIENCY = 15,
    SILK_TOUCH = 16,
    UNBREAKING = 17,
    FORTUNE = 18,
    POWER = 19,
    PUNCH = 20,
    FLAME = 21,
    INFINITYS = 22,
    LUCK_OF_THE_SEA = 23,
    LURE = 24,
    FROST_WALKER = 25,
    MENDING = 26,
    CURSE_OF_BINDING = 27,
    CURSE_OF_VANISHING = 28,
    IMPALING = 29,
    RIPTIDE = 30,
    LOYALTY = 31,
    CHANNELING = 32,
    MULTISHOT = 33,
    PIERCING = 34,
    QUICK_CHARGE = 35,
    SOUL_SPEED = 36,
    SWIFT_SNEAK = 37
};
class ItemStackBase {
public:
	BUILD_ACCESS(this, Item**, item, 0x8);
	//BUILD_ACCESS(this, CompoundTag*, compoundtag, 0x10);
	//BUILD_ACCESS(this, Block*, block, 0x18);
	BUILD_ACCESS(this, int16_t, uses, 0x20);
	BUILD_ACCESS(this, int8_t, stackCount, 0x22);
public:
	inline Item* getItemPtr() {
		if (this->item == NULL) return nullptr;
		return *this->item;
	}

	int getEnchantLevel(EnchantID enchantId) {
		using func_t = int (__thiscall*)(EnchantID, ItemStackBase*);
		static func_t func = (func_t)findSig(Sigs::enchantutils::getenchantlevel);
		return func(enchantId, this);
	}
};
