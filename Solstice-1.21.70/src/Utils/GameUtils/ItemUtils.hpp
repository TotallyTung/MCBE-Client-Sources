#pragma once
#include <complex.h>
#include <unordered_map>
#include <SDK/Minecraft/Inventory/Item.hpp>
#include <SDK/Minecraft/Inventory/ItemStack.hpp>

#include <SDK/Minecraft/World/BlockSource.hpp>
//
// Created by vastrakai on 7/6/2024.
//

class ItemUtils {
public:
    static int getItemValue(ItemStack* item);
    static int getBestItem(SItemType type, bool hotbarOnly = false);
    static bool hasItemType(ItemStack* item);
    static std::unordered_map<SItemType, int> getBestItems();
    static int getAllPlaceables(bool hotbarOnly = true);
    static int getFirstPlaceable(bool hotbarOnly);
    static int getPlaceableItemOnBlock(glm::vec3 blockPos, bool hotbarOnly = true, bool prioHighest = true);
    static bool isUsableBlock(ItemStack* stack);
    static int getSwiftnessSpellbook(bool hotbarOnly = true);
    static int getBoombox(bool hotbarOnly = true, bool tnt = false);
    static int getNonSolidBlock(bool hotbarOnly = true);
    static int getBlocks(bool hotbarOnly = true, bool crumblingCobblestone = false);
    static int getEmptyHotbarSlot();
    static void useItem(int slot);
    static int getHardestBlock(int slot, bool hotbarOnly = false);
    static int getBestBreakingTool(Block* block, bool hotbarOnly = false);
    static float getDestroySpeed(int slot, const Block* block, float divisor = 1.f);
    static bool isFireSword(ItemStack* stack);
    static int getFireSword(bool hotbarOnly = true);
};