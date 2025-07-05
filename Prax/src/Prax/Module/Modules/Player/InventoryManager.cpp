//
// Created by vastrakai on 8/28/2023.
//

#include "InventoryManager.h"
#include "ChestStealer.h"
#include <src/Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/Utils/String.h>
#include <Prax/SDK/Game/Network/Other/InventoryTransactionPacket.h>


#define NOW Misc::GetCurrentMs();
static uint64_t lastActionTime = NOW;

int getFireSwordSlot()
{
    // starts with &r&6Sword of &eEmbers
    auto player = Minecraft::ClientInstance->ThePlayer();
    for (int i = 0; i < 36; i++) {
        ItemStack* stack = player->supplies->inventory->getItem(i);
        if (stack && stack->item) {
            if (String::ContainsIgnoreCase(stack->GetCustomName(), "§6Sword of §eEmbers")) {
                return i;
            }
            if (String::ContainsIgnoreCase(stack->GetCustomName(), "§cFire Sword")) {
                return i;
            }
        }
    }
    return -1;
}

void InventoryManager::SwapSlots(int from, int to) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto supplies = player->supplies;
    auto inventory = supplies->inventory;

    inventory->swapSlots(from, to);

    AddDebugMessage("Swapped slots " + std::to_string(from) + " and " + std::to_string(to));

}

void InventoryManager::DropSlot(int slot) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    player->supplies->inventory->dropSlot(slot);
    AddDebugMessage("Dropped item at slot " + std::to_string(slot));
}

void InventoryManager::AddDebugMessage(const std::string& str, int)
{
#ifdef __DEBUG__
    if (Debug.Enabled) {
        Minecraft::ClientInstance->ThePlayer()->addChatMessage("[debug] " + str, 0);
        Logger::Write("InvManager", str, Logger::LogType::Debug);
    }
#endif
}

bool InventoryManager::DoSwapIfNeeded(int bestSlot, int preferredSlot)
{
    if (bestSlot == -1 || preferredSlot == 0 || bestSlot == (preferredSlot - 1)) return false;
    SwapSlots(bestSlot, preferredSlot - 1);
    return true;
}

bool InventoryManager::DoDelay()
{
    if (Delay.Value != 0)
    {
        lastActionTime = NOW;
        //AddDebugMessage("Delaying action: " + std::to_string(lastActionTime), 0);
        return true;
    }

    return false;
}

void InventoryManager::OnTick() {
    if (ChestStealer::IsStealing) return;
    auto player = Minecraft::ClientInstance->ThePlayer();

    uint64_t now = Misc::GetCurrentMs();

    if (now - lastActionTime < Delay.Value)
    {
        //AddDebugMessage("Delaying action (tick): " + std::to_string(now - lastActionTime) + " < " + std::to_string(Delay.Value), 0);
        return;
    }

    int preferredSwordSlot = PreferredSwordSlot.SelectedIndex;
    int preferredPickaxeSlot = PreferredPickaxeSlot.SelectedIndex;
    int preferredAxeSlot = PreferredAxeSlot.SelectedIndex;
    int preferredShovelSlot = PreferredShovelSlot.SelectedIndex;

    PlayerInventoryProxy *supplies = player->supplies;
    Inventory *inventory = supplies->inventory;

    for (int i = 0; i < 36; i++) {
        ItemStack* itemStack = inventory->getItem(i);
        if (itemStack && itemStack->item) {
            if (IsUselessItem(itemStack, i)) {
                DropSlot(i);
                if (DoDelay()) return;
            }
        }
    }

    int bestSwordSlot = -1;
    int bestPickaxeSlot = -1;
    int bestAxeSlot = -1;
    int bestShovelSlot = -1;

    bestSwordSlot = supplies->GetBestSword(false);
    bestPickaxeSlot = supplies->GetBestPickaxe(false);
    bestAxeSlot = supplies->GetBestAxe(false);
    bestShovelSlot = supplies->GetBestShovel(false);

    if (MoveBlocksToInventory.Enabled)
    {
        std::vector<int> emptySlots = {};
        // add empty slots not in hotbar
        for (int i = 9; i < 36; i++) {
            ItemStack* itemStack = inventory->getItem(i);
            if (!itemStack || !itemStack->item) {
                emptySlots.push_back(i);
            }
        }

        // iterate thru hotbar slots
        for (int i = 0; i < 9; i++) {
            ItemStack* itemStack = supplies->GetItemAtSlot(i);
            if (itemStack && itemStack->item) {
                Item* item = *itemStack->item;
                bool isBlock = itemStack->blockPtr != nullptr;
                if (isBlock && String::StartsWith(itemStack->blockPtr->blockLegacy->itemNamespace, "minecraft:"))
                {
                    int firstEmptySlot = emptySlots.size() > 0 ? emptySlots[0] : -1;

                    if (firstEmptySlot != -1) {
                        AddDebugMessage("Performing block swap to empty on slot " + std::to_string(i) + " with name " + itemStack->blockPtr->blockLegacy->itemNamespace, 0);
                        SwapSlots(i, firstEmptySlot);
                        if (DoDelay()) return;
                        emptySlots.erase(emptySlots.begin());
                    } else break;
                }
            }
        }
    }

    // instead of doing the above code, lets use a smaller function to equip the best armor
    if (DoSwapIfNeeded(bestSwordSlot, preferredSwordSlot)) if (DoDelay()) return;
    if (DoSwapIfNeeded(bestPickaxeSlot, preferredPickaxeSlot)) if (DoDelay()) return;
    if (DoSwapIfNeeded(bestAxeSlot, preferredAxeSlot)) if (DoDelay()) return;
    if (DoSwapIfNeeded(bestShovelSlot, preferredShovelSlot)) if (DoDelay()) return;


    // Equip best armor
    ItemStack *helmetItem = player->getArmor(ArmorSlot::Helmet);
    ItemStack *chestItem = player->getArmor(ArmorSlot::Chestplate);
    ItemStack *leggingsItem = player->getArmor(ArmorSlot::Leggings);
    ItemStack *bootsItem = player->getArmor(ArmorSlot::Boots);

    bool hasHelmet = helmetItem->item;
    bool hasChest = chestItem->item;
    bool hasLeggings = leggingsItem->item;
    bool hasBoots = bootsItem->item;

    int bestHelmetSlot = -1;
    int bestChestSlot = -1;
    int bestLeggingsSlot = -1;
    int bestBootsSlot = -1;

    int helmetValue = -1;
    int chestValue = -1;
    int leggingsValue = -1;
    int bootsValue = -1;

    if (hasHelmet) helmetValue = (*helmetItem->item)->getArmorValue() + helmetItem->getEnchantValue(Enchant::PROTECTION);
    if (hasChest) chestValue = (*chestItem->item)->getArmorValue() + chestItem->getEnchantValue(Enchant::PROTECTION);
    if (hasLeggings) leggingsValue = (*leggingsItem->item)->getArmorValue() + leggingsItem->getEnchantValue(Enchant::PROTECTION);
    if (hasBoots) bootsValue = (*bootsItem->item)->getArmorValue() + bootsItem->getEnchantValue(Enchant::PROTECTION);

    for (int i = 0; i < 36; i++) {
        ItemStack* itemStack = inventory->getItem(i);
        if (itemStack && itemStack->item) {
            Item* item = *itemStack->item;
            if (item->IsHelmet()) {
                int armorValue = item->getArmorValue() + itemStack->getEnchantValue(Enchant::PROTECTION);
                if (armorValue > helmetValue) {
                    bestHelmetSlot = i;
                    helmetValue = armorValue;
                }
            } else if (item->IsChestplate()) {
                int armorValue = item->getArmorValue() + itemStack->getEnchantValue(Enchant::PROTECTION);
                if (armorValue > chestValue) {
                    bestChestSlot = i;
                    chestValue = armorValue;
                }
            } else if (item->IsLeggings()) {
                int armorValue = item->getArmorValue() + itemStack->getEnchantValue(Enchant::PROTECTION);
                if (armorValue > leggingsValue) {
                    bestLeggingsSlot = i;
                    leggingsValue = armorValue;
                }
            } else if (item->IsBoots()) {
                int armorValue = item->getArmorValue() + itemStack->getEnchantValue(Enchant::PROTECTION);
                if (armorValue > bootsValue) {
                    bestBootsSlot = i;
                    bootsValue = armorValue;
                }
            }
        }
    }

    if (bestHelmetSlot != -1)
    {
        EquipItem(bestHelmetSlot);
        if (DoDelay()) return;
    }
    if (bestChestSlot != -1)
    {
        EquipItem(bestChestSlot);
        if (DoDelay()) return;
    }
    if (bestLeggingsSlot != -1)
    {
        EquipItem(bestLeggingsSlot);
        if (DoDelay()) return;
    }
    if (bestBootsSlot != -1)
    {
        EquipItem(bestBootsSlot);
        if (DoDelay()) return;
    }
}

void InventoryManager::EquipItem(int slot)
{
    auto player = Minecraft::ClientInstance->ThePlayer();

    AddDebugMessage("Equipping item at slot " + std::to_string(slot), 0);

    player->supplies->inventory->equipArmor(slot);
    // use transaction packet

    AddDebugMessage("Equipped item at slot " + std::to_string(slot), 0);


}


// Based on the above code, let's create a new function that will return a bool value so we can reduce the amount of code in the OnTick function
bool InventoryManager::IsUselessItem(ItemStack* itemStack, int slot)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    PlayerInventoryProxy *supplies = player->supplies;

    if (!itemStack->item) return false;
    Item* item = *itemStack->item;

    if (IgnoreExtraPickaxes.Enabled && item->IsPickaxe()) return false;

    ItemStack *helmetItem = player->getArmor(ArmorSlot::Helmet);
    ItemStack *chestItem = player->getArmor(ArmorSlot::Chestplate);
    ItemStack *leggingsItem = player->getArmor(ArmorSlot::Leggings);
    ItemStack *bootsItem = player->getArmor(ArmorSlot::Boots);

    bool hasHelmet = helmetItem->item;
    bool hasChest = chestItem->item;
    bool hasLeggings = leggingsItem->item;
    bool hasBoots = bootsItem->item;

    int bestHelmetSlot = -1;
    int bestChestSlot = -1;
    int bestLeggingsSlot = -1;
    int bestBootsSlot = -1;

    int helmetValue = -1;
    int chestValue = -1;
    int leggingsValue = -1;
    int bootsValue = -1;

    if (hasHelmet) helmetValue = (*helmetItem->item)->getArmorValue() + helmetItem->getEnchantValue(Enchant::PROTECTION);
    if (hasChest) chestValue = (*chestItem->item)->getArmorValue() + chestItem->getEnchantValue(Enchant::PROTECTION);
    if (hasLeggings) leggingsValue = (*leggingsItem->item)->getArmorValue() + leggingsItem->getEnchantValue(Enchant::PROTECTION);
    if (hasBoots) bootsValue = (*bootsItem->item)->getArmorValue() + bootsItem->getEnchantValue(Enchant::PROTECTION);

    int bestSwordSlot = -1;
    int bestPickaxeSlot = -1;
    int bestAxeSlot = -1;
    int bestShovelSlot = -1;

    int swordValue = -1;
    int pickaxeValue = -1;
    int axeValue = -1;
    int shovelValue = -1;

    bestSwordSlot = supplies->GetBestSword(false);
    bestPickaxeSlot = supplies->GetBestPickaxe(false);
    bestAxeSlot = supplies->GetBestAxe(false);
    bestShovelSlot = supplies->GetBestShovel(false);

    ItemStack* swordItem = nullptr;
    ItemStack* pickaxeItem = nullptr;
    ItemStack* axeItem = nullptr;
    ItemStack* shovelItem = nullptr;

    if (bestSwordSlot != -1) swordItem = supplies->GetItemAtSlot(bestSwordSlot);
    if (bestPickaxeSlot != -1) pickaxeItem = supplies->GetItemAtSlot(bestPickaxeSlot);
    if (bestAxeSlot != -1) axeItem = supplies->GetItemAtSlot(bestAxeSlot);
    if (bestShovelSlot != -1) shovelItem = supplies->GetItemAtSlot(bestShovelSlot);

    if (swordItem && swordItem->item) swordValue = supplies->GetToolItemValue(swordItem) + swordItem->getEnchantValue(Enchant::SHARPNESS);
    if (pickaxeItem && pickaxeItem->item) pickaxeValue = supplies->GetToolItemValue(pickaxeItem) + pickaxeItem->getEnchantValue(Enchant::EFFICIENCY);
    if (axeItem && axeItem->item) axeValue = supplies->GetToolItemValue(axeItem) + axeItem->getEnchantValue(Enchant::EFFICIENCY);
    if (shovelItem && shovelItem->item) shovelValue = supplies->GetToolItemValue(shovelItem) + shovelItem->getEnchantValue(Enchant::EFFICIENCY);

    int fireSwordSlot = getFireSwordSlot();


    if (IgnoreFireSword.Enabled && getFireSwordSlot() == slot) return false;
    if (IgnoreFireSword.Enabled && itemStack->GetCustomName().contains("Sword of §eEmbers")) return false;

    if (!IgnoreFireSword.Enabled) fireSwordSlot = -1;

    auto invItems = supplies->GetInventoryItems();
    int i = 0;

    // Return true if the item passed into this function needs to be dropped
    if (slot == fireSwordSlot && slot != -1)
    {
        AddDebugMessage("Drop fire sword", 0);
        return true;
    }

    int armorValue = item->getArmorValue();

    // Helmet
    if (item->IsHelmet()) {
        armorValue += itemStack->getEnchantValue(Enchant::PROTECTION);
        if (armorValue > helmetValue) {
            bestHelmetSlot = slot;
            helmetValue = armorValue;
        }
    }
    // Chestplate
    else if (item->IsChestplate())
    {
        armorValue += itemStack->getEnchantValue(Enchant::PROTECTION);
        if (armorValue > chestValue) {
            bestChestSlot = slot;
            chestValue = armorValue;
        }
    }
    // Leggings
    else if (item->IsLeggings()) {
        armorValue += itemStack->getEnchantValue(Enchant::PROTECTION);
        if (armorValue > leggingsValue) {
            bestLeggingsSlot = slot;
            leggingsValue = armorValue;
        }
    }
    // Boots
    else if (item->IsBoots())
    {
        armorValue += itemStack->getEnchantValue(Enchant::PROTECTION);
        if (armorValue > bootsValue) {
            bestBootsSlot = slot;
            bootsValue = armorValue;
        }
    } else if (item->IsSword()) {
        if (armorValue > swordValue) {
            bestSwordSlot = slot;
            swordValue = armorValue;
        }
    } else if (item->IsPickaxe()) {
        if (armorValue > pickaxeValue) {
            bestPickaxeSlot = slot;
            pickaxeValue = armorValue;
        }
    } else if (item->IsAxe()) {
        if (armorValue > axeValue) {
            bestAxeSlot = slot;
            axeValue = armorValue;
        }
    } else if (item->IsShovel()) {
        if (armorValue > shovelValue) {
            bestShovelSlot = slot;
            shovelValue = armorValue;
        }
    }

    // If the item is the best, return false
    if (slot == bestHelmetSlot) return false;
    if (slot == bestChestSlot) return false;
    if (slot == bestLeggingsSlot) return false;
    if (slot == bestBootsSlot) return false;
    if (slot == bestSwordSlot) return false;
    if (slot == bestPickaxeSlot) return false;
    if (slot == bestAxeSlot) return false;
    if (slot == bestShovelSlot) return false;
    if (slot == fireSwordSlot) return false;



    // Drop armor pieces that are worse than best armor
    if (DropExtraArmor.Enabled) {
        if (item->IsHelmet()) {
            if (armorValue <= helmetValue) {
                return true;
            }
        } else if (item->IsChestplate()) {
            if (armorValue <= chestValue) {
                return true;
            }
        } else if (item->IsLeggings()) {
            if (armorValue <= leggingsValue) {
                return true;
            }
        } else if (item->IsBoots()) {
            if (armorValue <= bootsValue) {
                return true;
            }
        }
    }

    // Drop all tools that are worse or equal to the value of the best tool
    if (DropExtraTools.Enabled)
    {
        int toolValue = supplies->GetToolItemValue(itemStack);

        if (item->IsSword()) {
            toolValue = supplies->GetToolItemValue(itemStack) + itemStack->getEnchantValue(Enchant::SHARPNESS);
            if (toolValue <= swordValue) {
                return true;
            }
        } else if (item->IsPickaxe()) {
            toolValue = supplies->GetToolItemValue(itemStack) + itemStack->getEnchantValue(Enchant::EFFICIENCY);
            if (toolValue <= pickaxeValue) {
                return true;
            }
        } else if (item->IsAxe()) {
            toolValue = supplies->GetToolItemValue(itemStack) + itemStack->getEnchantValue(Enchant::EFFICIENCY);
            if (toolValue <= axeValue) {
                return true;
            }
        } else if (item->IsShovel()) {
            toolValue = supplies->GetToolItemValue(itemStack) + itemStack->getEnchantValue(Enchant::EFFICIENCY);
            if (toolValue <= shovelValue) {
                return true;
            }
        }
    }


    // Drop extra bows if we have more than one
    if (!DropBows.Enabled && item->IsBow()) {
        int bowCount = 0;
        for (int i = 0; i < 36; i++) {
            ItemStack* stack = supplies->inventory->getItem(i);
            if (stack && stack->item) {
                if ((*stack->item)->IsBow()) {
                    bowCount++;
                }
            }
        }

        if (bowCount > 1) {
            return true;
        } else {
            return false;
        }
    }

    if (DropCrumblingBlocks.Enabled && String::ContainsIgnoreCase(itemStack->GetCustomName(), "Crumbling Cobblestone")) {
        return true;
    }

    if (DropFireSpells.Enabled && String::ContainsIgnoreCase(itemStack->GetCustomName(), "§r§dSpell of Fire Trail§7")) {
        return true;
    }

    if (DropBoomboxes.Enabled && item->IsBoombox()) {
        return true;
    }

    if (DropBows.Enabled && item->IsBowOrArrow()) {
        return true;
    }

    return false;
}


