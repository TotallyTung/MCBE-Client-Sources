#pragma once
//
// Created by ssi on 10/25/2024.
//

class AutoVote : public ModuleBase<AutoVote> {
public:
    BoolSetting mPrioritizeVoting = BoolSetting("Prioritize Voting", "Prioritize maps from the autoVoteTemplate list in the templates folder, otherwise vote randomly.", false);

    AutoVote() : ModuleBase("AutoVote", "Automatically votes for maps on The Hive", ModuleCategory::Misc, 0, false) {
        addSetting(&mPrioritizeVoting);

        mNames = {
            {Lowercase, "autovote"},
            {LowercaseSpaced, "auto vote"},
            {Normal, "AutoVote"},
            {NormalSpaced, "Auto Vote"}
        };
    }

    MessageTemplate mAutoVoteTemplate = MessageTemplate("autoVoteTemplate", "Pillars\nIvory\nBaroque\n");

    unsigned int mLastFormId = 0;
    bool mHasFormOpen = false;
    bool mVotedThisDimension = false;
    bool mInteractedThisDimension = false;
    std::string mJson;

    void submitForm(int buttonId);
    void closeForm();
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};