#pragma once
//
// Created by ssi on 10/23/2024.
//

#include <Features/Modules/Module.hpp>
#include <set>

class Anticheat : public ModuleBase<Anticheat> {
public:
    enum class Checks {
        SPEED_A,
        SPEED_B,
        IMMOBILE,
        ROTATION
    };

    class PlayerInfo {
    public:
        std::string playerName;
        glm::vec3 lastPosition;
        std::map<int64_t, float> bpsHistory;
        int64_t lastFlagTime = 0;
        std::map<Checks, int> flagCounts;
        std::pmr::set<Checks> flaggedChecks;

        PlayerInfo() : lastPosition(0.0f), lastFlagTime(0) {
            for (Checks check : { Checks::SPEED_A, Checks::SPEED_B, Checks::IMMOBILE, Checks::ROTATION }) {
                flagCounts[check] = 0;
            }
        }
    };

    BoolSetting mSpeedCheck = BoolSetting("Speed Check", "Detects players that are moving too fast", false);
    BoolSetting mSpeedACheck = BoolSetting("Speed A Check", "Checks for players moving too fast while off the ground", true);
    BoolSetting mSpeedBCheck = BoolSetting("Speed B Check", "Checks for players moving too fast while on the ground", true);

    BoolSetting mImmobileCheck = BoolSetting("Immobile Check", "Detects players teleporting while immobile", false);
    BoolSetting mRotationCheck = BoolSetting("Rotation Check", "Detects players with invalid yaw or pitch", false);

    Anticheat() : ModuleBase("Anticheat", "Detects other players using cheats", ModuleCategory::Misc, 0, false) {
        addSetting(&mSpeedCheck);
        addSetting(&mSpeedACheck);
        addSetting(&mSpeedBCheck);
        addSetting(&mImmobileCheck);
        addSetting(&mRotationCheck);

        VISIBILITY_CONDITION(mSpeedACheck, mSpeedCheck.mValue);
        VISIBILITY_CONDITION(mSpeedBCheck, mSpeedCheck.mValue);

        mNames = {
            {Lowercase, "anticheat"},
            {LowercaseSpaced, "anticheat"},
            {Normal, "Anticheat"},
            {NormalSpaced, "Anticheat"}
        };
    }

    std::unordered_map<Actor*, PlayerInfo> playerMap;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};