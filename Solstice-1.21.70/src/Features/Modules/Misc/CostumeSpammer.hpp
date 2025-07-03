#pragma once
//
// Created by vastrakai on 11/11/2024.
//


class CostumeSpammer : public ModuleBase<CostumeSpammer>
{
public:
    enum class Mode
    {
        Form,
        Command
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode to use.", Mode::Form, "Form", "Command");
    NumberSetting mDelay = NumberSetting("Delay", "The delay between each costume change.", 1.f, 0.01f, 10.f, 0.01f);
    BoolSetting mHideChatMessages = BoolSetting("Hide Chat Messages", "Hides chat messages.", true);

    CostumeSpammer() : ModuleBase("CostumeSpammer", "Spams costumes.", ModuleCategory::Misc, 0, false)
    {
        addSettings(&mMode, &mDelay, &mHideChatMessages);

        mNames = {
            {Lowercase, "costumespammer"},
            {LowercaseSpaced, "costume spammer"},
            {Normal, "CostumeSpammer"},
            {NormalSpaced, "Costume Spammer"}
        };
    }

    std::vector<int> mOpenFormIds;
    std::map<int, std::string> mFormJsons;
    std::map<int, std::string> mFormTitles;
    bool mInteractable = false;
    int64_t mLastInteract = 0;
    int64_t mLastCommand = 0;
    std::map<int64_t, std::string> mCommandQueue;


    void onEnable() override;
    void onDisable() override;
    void submitForm(int buttonId, int formId);
    void submitBoolForm(bool buttonId, int formId);
    void closeForm(int formId);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};