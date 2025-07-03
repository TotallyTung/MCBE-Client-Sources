//
// Created by vastrakai on 7/7/2024.
//

#include "Item.hpp"


int Item::getArmorSlot()
{
    // Use the name of the item to determine the armor slot
    static constexpr std::array<std::string_view, 4> armorSlots = {
        "_helmet",
        "_chestplate",
        "_leggings",
        "_boots"
    };

    for (int i = 0; i < armorSlots.size(); i++) {
        if (mName.find(armorSlots[i]) != std::string::npos) {
            return i;
        }
    }

    return -1;
}

bool Item::isHelmet()
{
    static constexpr std::string_view helmet = "_helmet";
    return mName.ends_with(helmet);
}

bool Item::isChestplate()
{
    static constexpr std::string_view chestplate = "_chestplate";
    return mName.ends_with(chestplate);
}

bool Item::isLeggings()
{
    static constexpr std::string_view leggings = "_leggings";
    return mName.ends_with(leggings);
}

bool Item::isBoots()
{
    static constexpr std::string_view boots = "_boots";
    return mName.ends_with(boots);
}

bool Item::isSword()
{
    static constexpr std::string_view sword = "_sword";
    return mName.ends_with(sword);
}

bool Item::isPickaxe()
{
    static constexpr std::string_view pickaxe = "_pickaxe";
    return mName.ends_with(pickaxe);
}

bool Item::isAxe()
{
    static constexpr std::string_view axe = "_axe";
    return mName.ends_with(axe);
}

bool Item::isShovel()
{
    static constexpr std::string_view shovel = "_shovel";
    return mName.ends_with(shovel);
}

int Item::getItemTier()
{
    // Use the name of the item to determine the tier
    static constexpr std::array<std::string_view, 7> tiers = {
        "wooden_",
        "chainmail_",
        "stone_",
        "iron_",
        "golden_",
        "diamond_",
        "netherite_"
    };

    for (int i = 0; i < tiers.size(); i++)
    {
        if (mName.starts_with(tiers[i])) {
            return i;
        }
    }

    return 0;
}

int Item::getArmorTier()
{
    // Use the name of the item to determine the tier
    static constexpr std::array<std::string_view, 7> tiers = {
        "leather_",
        "chainmail_",
        "iron_",
        "golden_",
        "diamond_",
        "netherite_"
    };

    for (int i = 0; i < tiers.size(); i++)
    {
        if (mName.starts_with(tiers[i])) {
            return i;
        }
    }

    return 0;
}

SItemType Item::getItemType()
{
    if (isHelmet()) return SItemType::Helmet;
    if (isChestplate()) return SItemType::Chestplate;
    if (isLeggings()) return SItemType::Leggings;
    if (isBoots()) return SItemType::Boots;
    if (isSword()) return SItemType::Sword;
    if (isPickaxe()) return SItemType::Pickaxe;
    if (isAxe()) return SItemType::Axe;
    if (isShovel()) return SItemType::Shovel;

    return SItemType::None;
}

