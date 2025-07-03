#pragma once
//
// Created by vastrakai on 8/23/2024.
//


class AutoCosmetic : public ModuleBase<AutoCosmetic> {
public:
    NumberSetting mUiDelay = NumberSetting("UI Delay", "The delay in milliseconds between each UI interaction", 200, 0, 1000, 100);
    BoolSetting mDebug = BoolSetting("Debug", "Whether or not to display cosmetic application information", false);

    AutoCosmetic() : ModuleBase("AutoCosmetic", "Automatically applies cosmetics on The Hive", ModuleCategory::Misc, 0, false)
    {
        addSetting(&mUiDelay);
        addSetting(&mDebug);

        mNames = {
            {Lowercase, "autocosmetic"},
            {LowercaseSpaced, "auto cosmetic"},
            {Normal, "AutoCosmetic"},
            {NormalSpaced, "Auto Cosmetic"}
        };
    }

    unsigned int mLastFormId = 0;
    bool mHasFormOpen = false;
    bool mIsCosmeticMenu = false;
    std::string mJson = "";
    std::string mLastFormTitle = "";
    bool mFinishedApplying = true;
    int mCosmeticIndex = 0;
    bool mInteractedWithItem = false;
    uint64_t mLastFormTime = 0;

    void onEnable() override;
    void onDisable() override;
    void submitForm(int buttonId);
    void closeForm();
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);

};