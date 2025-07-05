//
// Created by vastrakai on 2/11/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class ChestAura : public ModuleBase<ChestAura> {
public:
    NumberSetting Range = NumberSetting("Range", "The range of the chest aura.", 1, 10, 5, 0.1);
    NumberSetting Delay = NumberSetting("Delay", "The delay between opening chests (measured in milliseconds).", 0, 1000, 0, 1);

    ChestAura() : ModuleBase("ChestAura", "Automatically open chests in a certain range.", "Player", 0, false) {
        AddSetting(&Range);
        AddSetting(&Delay);
    }

    /* Static variables */
    static std::vector<Vector3> OpenedChests;
    static uint64_t LastOpenTime;

    /* Module functions */
    void OnEnable() override;
    void OnDisable() override;
    void OnTick() override;
    void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) override;


};
