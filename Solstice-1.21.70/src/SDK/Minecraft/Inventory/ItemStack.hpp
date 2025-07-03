//
// Created by vastrakai on 7/5/2024.
//

#pragma once

#include <regex>
#include <SDK/OffsetProvider.hpp>
#include <SDK/SigManager.hpp>
#include <Utils/MemUtils.hpp>

#include "CompoundTag.hpp"


class ItemStackBase {
public:
    uintptr_t** mVfTable;
    //WeakPtr<class Item> mItem;
    class Item** mItem;
    CompoundTag* mCompoundTag;
    class Block* mBlock;
    short mAuxValue;
    int8_t mCount;
    bool valid;
    PAD(0x60);

    Item* getItem() {
        return *mItem;
    }
};

enum class Enchant : int {
    PROTECTION = 0,
    FIRE_PROTECTION = 1,
    FEATHER_FALLING = 2,
    BLAST_PROTECTION = 3,
    PROJECTILE_PROTECTION = 4,
    THORNS = 5,
    RESPIRATION = 6,
    DEPTH_STRIDER = 7,
    AQUA_AFFINITY = 8,
    SHARPNESS = 9,
    SMITE = 10,
    BANE_OF_ARTHROPODS = 11,
    KNOCKBACK = 12,
    FIRE_ASPECT = 13,
    LOOTING = 14,
    EFFICIENCY = 15,
    SILK_TOUCH = 16,
    UNBREAKING = 17,
    FORTUNE = 18,
    POWER = 19,
    PUNCH = 20,
    FLAME = 21,
    BOW_INFINITY = 22,
    LUCK_OF_THE_SEA = 23,
    LURE = 24,
    FROST_WALKER = 25,
    MENDING = 26,
    BINDING = 27,
    VANISHING = 28,
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

class ItemStack : public ItemStackBase
{
public:
    uint8_t mStackNetId; //0x0088
    PAD(0x8);

    void reinit(Item* item, int count, int itemData) {
        mVfTable = reinterpret_cast<uintptr_t**>(SigManager::ItemStack_vTable);
        MemUtils::callVirtualFunc<void>(OffsetProvider::ItemStack_reInit, this, item, count, itemData);
    }

    std::string getCustomName() {
        static auto func = SigManager::ItemStack_getCustomName;
        std::string str;
        MemUtils::callFastcall<void, ItemStack*, std::string*>(func, this, &str);
        return str;
    }

    std::map<int, int> gatherEnchants()
    {
        if (!mCompoundTag) return {};

        std::map<int, int> enchants;
        for (auto& [first, second] : mCompoundTag->data) {
            if (second.type != Tag::Type::List || first != "ench") continue;

            for (const auto list = second.asListTag(); const auto& entry : list->val) {
                if (entry->getId() != Tag::Type::Compound) continue;

                const auto comp = reinterpret_cast<CompoundTag*>(entry);
                int id = comp->get("id")->asShortTag()->val;
                const int lvl = comp->get("lvl")->asShortTag()->val;
                enchants[id] = lvl;
            }
        }

        return enchants;
    }

    int getEnchantValue(int id)
    {
        auto enchants = gatherEnchants();
        return enchants.contains(id) ? enchants[id] : 0;
    }

    int getEnchantValue(Enchant enchant)
    {
        return getEnchantValue(static_cast<int>(enchant));
    }

    ItemStack()
    {
        memset(this, 0, sizeof(ItemStack));
        mVfTable = reinterpret_cast<uintptr_t**>(SigManager::ItemStack_vTable);
    }

    ItemStack(Item* item, int count, int itemData) {
        memset(this, 0x0, sizeof(ItemStack));
        reinit(item, count, itemData);
    }

    std::string getEnchantName(Enchant enchant) const {
        std::string name = std::string(magic_enum::enum_name(enchant));
        name = StringUtils::toLower(name);
        name = StringUtils::replaceAll(name, "_", " ");
        return name;
    }
};

static_assert(sizeof(ItemStack) == 0x98, "ItemStack size invalid");