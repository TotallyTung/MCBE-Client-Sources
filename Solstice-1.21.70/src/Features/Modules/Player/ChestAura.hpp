#pragma once
//
// Created by ssi on 10/24/2024.
//

#include <Features/Modules/Module.hpp>

class ChestAura : public ModuleBase<ChestAura> {
public:
    NumberSetting mRange = NumberSetting("Range", "Max distance for chest opening. (For best performance, use 4)", 4, 0, 15, 1);
    NumberSetting mDelay = NumberSetting("Delay", "Time between opening chests (ms)", 100, 0, 150, 1);

    ChestAura() : ModuleBase("ChestAura", "Opens nearby chests automatically", ModuleCategory::Player, 0, false)
    {
        addSettings(&mRange, &mDelay);

        mNames = {
            {Lowercase, "chestaura"},
              {LowercaseSpaced, "chest aura"},
              {Normal, "ChestAura"},
              {NormalSpaced, "Chest Aura"}
        };
    };

    bool mIsChestOpened = false;
    uint64_t mTimeOfLastChestOpen;
    std::vector<glm::vec3> mOpenedChestPositions;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};