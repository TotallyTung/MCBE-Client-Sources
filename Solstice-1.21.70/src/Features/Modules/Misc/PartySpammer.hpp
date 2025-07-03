#pragma once
//
// Created by vastrakai on 8/23/2024.
//


class PartySpammer : public ModuleBase<PartySpammer> {
public:
    enum class Mode {
        Form,
        Command,
        Aura
    };
    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the party spammer", Mode::Form, "Form"
#ifdef __PRIVATE_BUILD__
        , "Command", "Aura"
#endif
    );
    BoolSetting mActive = BoolSetting("Active", "Whether the party spammer is active", false);
    NumberSetting mDelay = NumberSetting("Delay", "The delay in milliseconds between each party invite", 1000, 0, 10000, 100);
    BoolSetting mHideInviteMessage = BoolSetting("Hide Invite Message", "Dont show invite message in chat", false);

    PartySpammer() : ModuleBase("PartySpammer", "Spam invites players to your party", ModuleCategory::Misc, 0, false) {
        addSetting(&mMode);
        addSetting(&mActive);
        addSetting(&mDelay);
#ifdef __PRIVATE_BUILD__
        addSetting(&mHideInviteMessage);
#endif

        VISIBILITY_CONDITION(mActive, mMode.mValue == Mode::Form);
        VISIBILITY_CONDITION(mDelay, mMode.mValue == Mode::Form);
        VISIBILITY_CONDITION(mHideInviteMessage, mMode.mValue == Mode::Command || mMode.mValue == Mode::Aura);

        mNames = {
            {Lowercase, "partyspammer"},
            {LowercaseSpaced, "party spammer"},
            {Normal, "PartySpammer"},
            {NormalSpaced, "Party Spammer"}
        };
    }

    std::string mPlayerToSpam = "Sinister4458";
    std::vector<int> mOpenFormIds;
    std::map<int, std::string> mFormJsons;
    std::map<int, std::string> mFormTitles;
    bool mSentInvite = false;
    bool mInteractable = false;
    uint64_t mLastInteract = 0;
    uint64_t mLastCommandRequestPacketSent = 0;
    bool mInvited = false;

    void onEnable() override;
    void onDisable() override;
    void submitForm(int buttonId, int formId);
    void submitBoolForm(bool buttonId, int formId);
    void closeForm(int formId);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};