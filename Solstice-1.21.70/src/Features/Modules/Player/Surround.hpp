#pragma once
//
// 2/21/2025.
//

#include <Features/Modules/Module.hpp>

class Surround : public ModuleBase<Surround> {
public:
    NumberSetting mRange = NumberSetting("Range", "The range at which to surround entities", 5, 3, 8, 0.1);
    NumberSetting mDelay = NumberSetting("Delay", "The delay in ms to place block", 500, 0, 3000, 50);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Only switch to blocks in the hotbar", true);
    BoolSetting mDebug = BoolSetting("Debug", "Send debug messages", false);
    Surround() : ModuleBase("Surround", "Surround a player with blocks", ModuleCategory::Player, 0, false)
    {
        addSetting(&mRange);
        addSetting(&mDelay);
        addSetting(&mHotbarOnly);
        addSetting(&mDebug);

        mNames = {
              {Lowercase, "surround"},
                {LowercaseSpaced, "surround"},
                {Normal, "Surround"},
                {NormalSpaced, "Surround"}
        };
    };

    uint64_t mLastBlockPlaced = 0;
    int mLastSlot = 0;

    void onEnable();
    void onDisable() override;
    std::vector<glm::ivec3> getCollidingBlocks(Actor* target);
    std::vector<glm::ivec3> getPlacePositions(std::vector<glm::ivec3> blockList);
    glm::ivec3 getClosestPlacePos(glm::ivec3 pos, float distance, std::vector<glm::ivec3> collidingBlocks);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};