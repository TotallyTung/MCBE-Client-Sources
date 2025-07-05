//
// Created by Flash on 9/15/2023.
//

#pragma once

#include <Prax/Module/Module.h>

class Statistics : public ModuleBase<Statistics> {
public:
    Statistics() : ModuleBase("Statistics", "Shows player stats for hive", "Visual") {}

    static int Kills;
    static int Deaths;
    static int Played;

    void OnRender() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel);

    void OnEnable();

    void OnDisable();
};
