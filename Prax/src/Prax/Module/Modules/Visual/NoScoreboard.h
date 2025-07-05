//
// Created by vastrakai on 5/10/2024.
//


#pragma once

#include <Prax/Module/Module.h>

class NoScoreboard : public ModuleBase<NoScoreboard> {
public:
    NoScoreboard() : ModuleBase("NoScoreboard", "Hides the scoreboard", "Visual", 0, false) {}

    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;
};