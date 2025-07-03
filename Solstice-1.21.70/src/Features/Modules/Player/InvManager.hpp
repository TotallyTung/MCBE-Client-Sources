#pragma once
//
// Created by vastrakai on 7/6/2024.
//

#include <Features/Modules/Module.hpp>

class InvManager : public ModuleBase<InvManager> {
public:
    enum class Mode {
        Instant,
        Delayed
    };

    std::vector<std::string> mSlots = {
        "None",
        "1",
        "2",
        "3",
        "4",
        "5",
        "6",
        "7",
        "8",
        "9"
    };

    enum class SlotPreference {
        None,
        Slot1,
        Slot2,
        Slot3,
        Slot4,
        Slot5,
        Slot6,
        Slot7,
        Slot8,
        Slot9
    };

    enum class ManagementMode
    {
        Always,
        ContainerOnly,
        InvOnly
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the module", Mode::Instant, "Instant", "Delayed");
    NumberSetting mDelay = NumberSetting("Delay", "The delay, in milliseconds", 50, 0, 500, 1);
    EnumSettingT<ManagementMode> mManagementMode = EnumSettingT<ManagementMode>("Management", "When to manage the inventory", ManagementMode::Always, "Always", "Container Only", "Inventory Only");
    BoolSetting mPreferredSlots = BoolSetting("Preferred Slots", "Use preferred slots", true);
    EnumSettingT<int> mPreferredSwordSlot = EnumSettingT("Sword Slot", "The slot where your sword is", 1, mSlots);
    EnumSettingT<int> mPreferredPickaxeSlot = EnumSettingT("Pickaxe Slot", "The slot where your pickaxe is", 2, mSlots);
    EnumSettingT<int> mPreferredAxeSlot = EnumSettingT("Axe Slot", "The slot where your axe is", 3, mSlots);
    EnumSettingT<int> mPreferredShovelSlot = EnumSettingT("Shovel Slot", "The slot where your shovel is", 4, mSlots);
    EnumSettingT<int> mPreferredFireSwordSlot = EnumSettingT("Fire Sword Slot", "The slot where your fire sword is", 5, mSlots);
    EnumSettingT<int> mPreferredBlocksSlot = EnumSettingT("Blocks Slot", "The slot where your blocks are", 6, mSlots);
    BoolSetting mDropExtraBows = BoolSetting("Drop Extra Bows", "Drop extra bows when you have more than one", false);
    BoolSetting mIgnoreFireSword = BoolSetting("Ignore Fire Sword", "Don't drop the fire sword", false);
    BoolSetting mStealFireProtection = BoolSetting("Steal Fire Protection", "Keep all armor with Fire Protection", false);

    InvManager() : ModuleBase("InvManager", "Manages your inventory", ModuleCategory::Player, 0, false) {
        addSetting(&mMode);
        addSetting(&mDelay);
        addSetting(&mManagementMode);
        addSetting(&mPreferredSlots);
        addSetting(&mPreferredSwordSlot);
        addSetting(&mPreferredPickaxeSlot);
        addSetting(&mPreferredAxeSlot);
        addSetting(&mPreferredShovelSlot);
        addSetting(&mPreferredFireSwordSlot);
        addSetting(&mPreferredBlocksSlot);
        addSetting(&mDropExtraBows);
        addSetting(&mIgnoreFireSword);
#ifdef __PRIVATE_BUILD__
        addSetting(&mStealFireProtection);
#endif

        VISIBILITY_CONDITION(mPreferredSwordSlot, mPreferredSlots.mValue);
        VISIBILITY_CONDITION(mPreferredPickaxeSlot, mPreferredSlots.mValue);
        VISIBILITY_CONDITION(mPreferredAxeSlot, mPreferredSlots.mValue);
        VISIBILITY_CONDITION(mPreferredShovelSlot, mPreferredSlots.mValue);
        VISIBILITY_CONDITION(mDelay, mMode.mValue == Mode::Delayed);


        mNames = {
            {Lowercase, "invmanager"},
            {LowercaseSpaced, "inv manager"},
            {Normal, "InvManager"},
            {NormalSpaced, "Inv Manager"}
        };
    }

    int64_t mLastAction = NOW;
    int64_t mLastPing = 0;
    bool mHasOpenContainer = false;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPingUpdateEvent(class PingUpdateEvent& event);
    static bool isItemUseless(class ItemStack* item, int slot);

    std::string getSettingDisplay() override {
        return mMode.mValue == Mode::Instant ? "Instant" : std::to_string(static_cast<int>(mDelay.mValue));
    }
};
