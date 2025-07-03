#pragma once
//
// Created by vastrakai on 7/19/2024.
//

#include <Features/Modules/Module.hpp>

class AutoMessage : public ModuleBase<AutoMessage> {
public:
    BoolSetting mOnVote = BoolSetting("On Vote", "Whether or not to send a message when you vote for a map", false);
    BoolSetting mOnDimensionChange = BoolSetting("On Dimension Change", "Whether or not to send a message when you change dimensions", false);

    AutoMessage() : ModuleBase("AutoMessage", "Automatically sends a message in chat depending on the triggers you have enabled.", ModuleCategory::Misc, 0, false)
    {
        addSettings(&mOnVote, &mOnDimensionChange);

        mNames = {
            {Lowercase, "automessage"},
            {LowercaseSpaced, "auto message"},
            {Normal, "AutoMessage"},
            {NormalSpaced, "Auto Message"}
        };
    }

    std::map<int64_t, std::string> mQueuedMessages;
    MessageTemplate mVoteMessageTemplate = MessageTemplate("voteMessageTemplate", "Yo @here! Vote for !mapName!!!", std::make_pair("!mapName!", "The name of the map you voted for"));
    MessageTemplate mDimensionChangeMessageTemplate = MessageTemplate("dimensionChangeMessageTemplate", "hey!! #LAWL #GG #XD");

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};