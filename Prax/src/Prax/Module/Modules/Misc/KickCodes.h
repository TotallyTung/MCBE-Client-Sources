//
// Created by vastrakai on 11/11/2023.
//


#pragma once
#include <Prax/Module/Module.h>
#include <Prax/SDK/Game/Network/Network.h>

#ifndef __RELEASEFREE__
class KickCodes : public ModuleBase<KickCodes> {
public:
    BoolSetting ShowReason = BoolSetting("Show reason", "Shows the flags in the kick reason. If this is not enabled, they will be logged to the console.", false);
    BoolSetting CopyToClipboard = BoolSetting("Copy to clipboard", "Copies the kick code to the clipboard", true);
    KickCodes() : ModuleBase("KickCodes", "Shows kick codes on The Hive (experimental)", "Misc", 0, false)
    {
        AddSetting(&ShowReason);
    };

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool *canceled) override;
};
#endif
