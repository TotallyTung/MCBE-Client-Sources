//
// Created by vastrakai on 10/30/2023.
//


#pragma once

#include <Prax/Module/Module.h>


class LeaveNotifs : public ModuleBase<LeaveNotifs> {
public:
    BoolSetting ShowInChat = BoolSetting("Show in chat", "Displays the message in chat", true);
    BoolSetting ShowNotification = BoolSetting("Show notification", "Displays the message as a notification", true);
    BoolSetting ShowJoins = BoolSetting("Show joins", "Displays the message when a player joins", true);
    BoolSetting ShowLeaves = BoolSetting("Show leaves", "Displays the message when a player leaves", true);
    BoolSetting ShowKicks = BoolSetting("Show kicks", "Displays the message when a player is kicked", true);

    LeaveNotifs() : ModuleBase("LeaveNotifs", "Sends a message when a player leaves", "Misc", 0, false) {
        AddSetting(&ShowInChat);
        AddSetting(&ShowNotification);
        AddSetting(&ShowJoins);
        AddSetting(&ShowLeaves);
        AddSetting(&ShowKicks);
    };

    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) override;
};