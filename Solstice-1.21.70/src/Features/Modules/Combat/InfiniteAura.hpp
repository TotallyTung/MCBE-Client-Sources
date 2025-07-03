#pragma once
//
// Created by vastrakai on 11/8/2024.
//
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/RenderEvent.hpp>
#include <Features/Events/BaseTickEvent.hpp>

class InfiniteAura : public ModuleBase<InfiniteAura>
{
public:
    enum class Type
    {
        MovePlayerPathing
    };

    enum class Mode {
        Single,
        Switch
    };

    enum class RenderMode {
        Lines,
        Boxes
    };

    EnumSettingT<Type> mType = EnumSettingT("Type", "The type of aura.", Type::MovePlayerPathing, "MovePlayerPathing");
    EnumSettingT<Mode> mMode = EnumSettingT("Mode", "The mode of the aura", Mode::Single, "Single", "Switch");
    EnumSettingT<RenderMode> mRenderMode = EnumSettingT("Render Mode", "The render mode of the aura.", RenderMode::Lines, "Lines", "Boxes");
    NumberSetting mRange = NumberSetting("Range", "The range of the aura.", 70.f, 3.f, 100.f, 0.01f);
    NumberSetting mAPS = NumberSetting("APS", "The attack per second of the aura.", 20.f, 1.f, 20.f, 0.01f);
    NumberSetting mBlocksPerPacket = NumberSetting("Blocks Per Packet", "The amount of blocks to send per packet.", 0.5f, 0.1f, 3.f, 0.01f);
    BoolSetting mSilentAccept = BoolSetting("Silent Accept", "Silently accepts teleport requests.", false);
    BoolSetting mRayTrace = BoolSetting("Ray Trace", "Ray traces to the target.", true);


    InfiniteAura() : ModuleBase("InfiniteAura", "Infinite aura.", ModuleCategory::Combat, 0, false)
    {
        addSettings(&mType, &mMode, &mRenderMode, &mRange, &mAPS, &mBlocksPerPacket, &mSilentAccept, &mRayTrace);

        mNames = {
            {Lowercase, "infiniteaura"},
            {LowercaseSpaced, "infinite aura"},
            {Normal, "InfiniteAura"},
            {NormalSpaced, "Infinite Aura"}
        };
    }

    glm::vec3 mRots;
    bool mHasTarget = false;
    bool mNeedsToPathBack = false;
    uint64_t mLastAttack = 0;
    glm::vec3 mLastPos;
    std::vector<glm::vec3> mPacketPositions;
    uint64_t mLastPathTime = 0;
    std::mutex mMutex;

    void onEnable() override;
    void onDisable() override;
    std::shared_ptr<class MovePlayerPacket> createPacketForPos(glm::vec3 pos);
    std::vector<std::shared_ptr<class MovePlayerPacket>> pathToPos(glm::vec3 from, glm::vec3 to);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onRenderEvent(class RenderEvent& event);
};