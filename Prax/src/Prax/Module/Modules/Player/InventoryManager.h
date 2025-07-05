#pragma once
//
// Created by vastrakai on 8/28/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class InventoryManager : public ModuleBase<InventoryManager> {
public:
    std::vector<std::string> slots = { "None", "1", "2", "3", "4", "5", "6", "7", "8", "9" };
    NumberSetting Delay = NumberSetting("Delay", "Delay between each action (in milliseconds)", 0, 1000, 50, 1);
    BoolSetting IgnoreFireSword = BoolSetting("Ignore Fire Sword", "Ignores the fire sword in your inventory", true);
    BoolSetting IgnoreExtraPickaxes = BoolSetting("Ignore Extra Pickaxes", "Ignores extra pickaxes in your inventory", true);
    BoolSetting DropExtraTools = BoolSetting("Drop Extra Tools", "Drops extra tools in your inventory", true);
    BoolSetting DropExtraArmor = BoolSetting("Drop Extra Armor", "Drops extra armor in your inventory", true);
    BoolSetting DropBoomboxes = BoolSetting("Drop Boomboxes", "Drops boomboxes in your inventory", false);
    BoolSetting DropBows = BoolSetting("Drop Bows", "Drops bows in your inventory", false);
    BoolSetting DropCrumblingBlocks = BoolSetting("Drop Crumbling Blocks", "Drops crumbling blocks in your inventory", false);
    BoolSetting DropFireSpells = BoolSetting("Drop Fire Spells", "Drops fire spells in your inventory", true);
    BoolSetting MoveBlocksToInventory = BoolSetting("Move Blocks To Inventory", "Moves blocks to your inventory", false);
    ModeSetting PreferredSwordSlot = ModeSetting("Preferred Sword Slot", "The slot where your sword is", slots, 1);
    ModeSetting PreferredPickaxeSlot = ModeSetting("Preferred Pickaxe Slot", "The slot where your pickaxe is", slots, 2);
    ModeSetting PreferredAxeSlot = ModeSetting("Preferred Axe Slot", "The slot where your axe is", slots, 3);
    ModeSetting PreferredShovelSlot = ModeSetting("Preferred Shovel Slot", "The slot where your shovel is", slots, 4);
#ifdef __DEBUG__
    BoolSetting Debug = BoolSetting("Debug", "Prints debug messages", false);
#endif

    InventoryManager() : ModuleBase("InventoryManager", "Automatically equips the best armor in your inventory", "Player") {
        AddSetting(&Delay);
        AddSetting(&IgnoreFireSword);
        AddSetting(&IgnoreExtraPickaxes);
        AddSetting(&DropExtraTools);
        AddSetting(&DropExtraArmor);
        AddSetting(&DropBoomboxes);
        AddSetting(&DropBows);
        AddSetting(&DropCrumblingBlocks);
        AddSetting(&DropFireSpells);
        AddSetting(&MoveBlocksToInventory);
        AddSetting(&PreferredSwordSlot);
        AddSetting(&PreferredPickaxeSlot);
        AddSetting(&PreferredAxeSlot);
        AddSetting(&PreferredShovelSlot);
#ifdef __DEBUG__
        AddSetting(&Debug);
#endif
    }

    void SwapSlots(int slot1, int slot2);
    void DropSlot(int slot);
    void AddDebugMessage(const std::string& str, int a = 0);
    bool DoSwapIfNeeded(int bestSlot, int preferredSlot);
    bool DoDelay();

    void OnTick() override;
    void EquipItem(int slot);
    bool IsUselessItem(ItemStack* itemStack, int slot);
};
