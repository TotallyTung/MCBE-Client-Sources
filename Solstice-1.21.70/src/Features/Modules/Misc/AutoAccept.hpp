#pragma once
//
// Created by vastrakai on 8/23/2024.
//


class AutoAccept : public ModuleBase<AutoAccept> {
public:
    BoolSetting mAcceptPartyInvites = BoolSetting("Accept Party Invites", "Automatically accepts party invites", true);
    BoolSetting mAcceptFriendRequests = BoolSetting("Accept Friend Requests", "Automatically accepts friend requests", true);

    AutoAccept() : ModuleBase("AutoAccept", "Automatically accepts party and friend invites", ModuleCategory::Misc, 0, false)
    {
        addSettings(
            &mAcceptPartyInvites,
            &mAcceptFriendRequests
        );

        mNames = {
            {Lowercase, "autoaccept"},
            {LowercaseSpaced, "auto accept"},
            {Normal, "AutoAccept"},
            {NormalSpaced, "Auto Accept"}
        };
    }

    std::vector<std::pair<uintptr_t, std::string>> mQueuedCommands;
    uint64_t mLastCommandTime = 0;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};