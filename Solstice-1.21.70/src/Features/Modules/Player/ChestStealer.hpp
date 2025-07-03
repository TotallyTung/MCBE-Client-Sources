#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <Features/Modules/Module.hpp>
#include <SDK/Minecraft/Inventory/ContainerManagerModel.hpp>

class ChestStealer : public ModuleBase<ChestStealer> {
public:
    enum class Mode
    {
        Normal
    };

    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of the chest stealer", Mode::Normal, "Normal");
    BoolSetting mRandomizeDelay = BoolSetting("Randomize Delay", "Randomizes the delay between stealing items", false);
    NumberSetting mDelay = NumberSetting("Delay", "The delay between stealing items (in milliseconds)", 50, 0, 500, 1);
    NumberSetting mRandomizeMin = NumberSetting("Randomize Min", "The minimum delay to randomize", 50, 0, 500, 1);
    NumberSetting mRandomizeMax = NumberSetting("Randomize Max", "The maximum delay to randomize", 100, 0, 500, 1);
    BoolSetting mIgnoreUseless = BoolSetting("Ignore Useless", "Whether or not to ignore useless items", true);


    ChestStealer() : ModuleBase<ChestStealer>("ChestStealer", "Steals items from chests", ModuleCategory::Player, 0, false)
    {
        addSettings(
#ifdef __PRIVATE_BUILD__
            &mMode,
#endif
            &mRandomizeDelay,
            &mDelay,
            &mRandomizeMin,
            &mRandomizeMax,
            &mIgnoreUseless
        );

        VISIBILITY_CONDITION(mRandomizeMin, mRandomizeDelay.mValue == true);
        VISIBILITY_CONDITION(mRandomizeMax, mRandomizeDelay.mValue == true);
        VISIBILITY_CONDITION(mDelay, mRandomizeDelay.mValue == false);

        mNames = {
            {Lowercase, "cheststealer"},
            {LowercaseSpaced, "chest stealer"},
            {Normal, "ChestStealer"},
            {NormalSpaced, "Chest Stealer"}
        };
    }

    bool mIsStealing = false;
    uint64_t mLastItemTaken = 0;
    bool mIsChestOpen = false;
    bool mTotalDirty = false;
    int mTotalItems = 0;
    int mRemainingItems = 0;
    glm::vec3 mLastPos = glm::vec3(0);
    glm::vec3 mHighlightedPos = glm::vec3(0);
    ContainerID mCurrentContainerId = ContainerID::None;
    uint64_t mLastOpen = 0;

    void onContainerScreenTickEvent(class ContainerScreenTickEvent& event) const;
    void reset();
    void onEnable() override;
    void onDisable() override;
    void takeItem(int slot, ItemStack& item);
    void onBaseTickEvent(class BaseTickEvent& event);
    bool doDelay();
    void onPacketOutEvent(class PacketOutEvent& event);
    uint64_t getDelay() const;

    std::string getSettingDisplay() override {
        if (mRandomizeDelay.mValue)
        {
            return std::to_string(static_cast<int>(mRandomizeMin.mValue)) + " " + std::to_string(static_cast<int>(mRandomizeMax.mValue));
        }
        else
        {
            return std::to_string(static_cast<int>(mDelay.mValue)) + "";
        }
    }
};
