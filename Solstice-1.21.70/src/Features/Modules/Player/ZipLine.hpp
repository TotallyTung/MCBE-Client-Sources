#pragma once
//
// 3/01/2025.
//

#include <Features/Modules/Module.hpp>

class ZipLine : public ModuleBase<ZipLine> {
public:
    NumberSetting mPlaces = NumberSetting("Places", "The amount of blocks to place per tick", 1, 0, 20, 0.01);
    NumberSetting mRange = NumberSetting("Range", "The range at which to place blocks", 5, 0, 10, 0.01);
    NumberSetting mExtend = NumberSetting("Extend", "The distance to extend the placement", 3, 0, 10, 1);
    BoolSetting mLockY = BoolSetting("Lock Y", "Whether or not to lock the Y position", false);
    BoolSetting mHotbarOnly = BoolSetting("Hotbar Only", "Only switch to blocks in the hotbar", true);
    ZipLine() : ModuleBase("ZipLine", "Automatically places blocks above you", ModuleCategory::Player, 0, false)
    {
        addSettings(&mPlaces, &mRange, &mExtend, &mLockY, &mHotbarOnly);

        mNames = {
              {Lowercase, "zipline"},
                {LowercaseSpaced, "zip line"},
                {Normal, "ZipLine"},
                {NormalSpaced, "Zip Line"}
        };
    };

    int mLastSlot = 0;
    float mStartY = 0;

    void onEnable() override;
    void onDisable() override;
    std::vector<glm::ivec3> getCollidingBlocks(Actor* target);
    glm::ivec3 getClosestPlacePos(glm::ivec3 pos, float distance, std::vector<glm::ivec3> collidingBlocks);
    glm::vec3 getRotBasedPos(float extend, float yPos);
    glm::vec3 getPlacePos(float extend);
    bool tickPlace();
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};