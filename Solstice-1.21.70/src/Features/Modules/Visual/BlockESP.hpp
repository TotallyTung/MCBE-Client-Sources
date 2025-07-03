#pragma once
//
// Created by vastrakai on 7/7/2024.
//

#include <Features/Modules/Module.hpp>


class BlockESP : public ModuleBase<BlockESP>
{
public:
    enum class BlockRenderMode {
        Filled,
        Outline,
        Both
    };

    EnumSettingT<BlockRenderMode> mRenderMode = EnumSettingT("Render Mode", "The mode to render block", BlockRenderMode::Outline, "Filled", "Outline", "Both");
    NumberSetting mRadius = NumberSetting("Radius", "The radius of the block esp", 20.f, 1.f, 100.f, 0.01f);
    NumberSetting mChunkRadius = NumberSetting("Chunk Radius", "The max chunk radius to search for blocks", 4.f, 1.f, 32.f, 1.f);
    NumberSetting mUpdateFrequency = NumberSetting("Update Frequency", "The frequency of the block update (in ticks)", 1.f, 1.f, 40.f, 0.01f);
    NumberSetting mChunkUpdatesPerTick = NumberSetting("Chunk Updates Per Tick", "The number of subchunks to update per tick", 5.f, 1.f, 24.f, 1.f);
    BoolSetting mRenderCurrentChunk = BoolSetting("Render Current Chunk", "Renders the current chunk", false);
    BoolSetting mEmerald = BoolSetting("Emerald", "Draws around emerald ore", true);
    BoolSetting mDiamond = BoolSetting("Diamond", "Draws around diamond ore", true);
    BoolSetting mGold = BoolSetting("Gold", "Draws around gold ore", true);
    BoolSetting mIron = BoolSetting("Iron", "Draws around iron ore", true);
    BoolSetting mCoal = BoolSetting("Coal", "Draws around coal ore", true);
    BoolSetting mRedstone = BoolSetting("Redstone", "Draws around redstone ore", true);
    BoolSetting mLapis = BoolSetting("Lapis", "Draws around lapis ore", true);
    BoolSetting mPortal = BoolSetting("Portal", "Draws around portal blocks", true);
    BoolSetting mChests = BoolSetting("Chests", "Draws around chests", false);
    BoolSetting mOnlyExposedOres = BoolSetting("Only Exposed Ores", "Show only ores that are exposed to air", false);

    BlockESP() : ModuleBase("BlockESP", "Draws a box around selected blocks", ModuleCategory::Visual, 0, false) {
        addSettings(
                &mRenderMode,
            &mRadius,
            &mChunkRadius,
            &mUpdateFrequency,
            &mChunkUpdatesPerTick,
            &mRenderCurrentChunk,
            &mDiamond,
            &mEmerald,
            &mGold,
            &mIron,
            &mCoal,
            &mRedstone,
            &mLapis,
            &mPortal,
            &mChests,
            &mOnlyExposedOres
        );

        mNames = {
            {Lowercase, "blockesp"},
            {LowercaseSpaced, "block esp"},
            {Normal, "BlockESP"},
            {NormalSpaced, "Block ESP"}
        };
    }

    ChunkPos mSearchCenter;
    ChunkPos mCurrentChunkPos;
    int mSubChunkIndex = 0;
    int mDirectionIndex = 0;
    int mSteps = 1;
    int mStepsCount = 0;
    int64_t mSearchStart = 0;

    struct FoundBlock
    {
        const Block* block;
        AABB aabb;
        ImColor color;
    };

    std::unordered_map<BlockPos, FoundBlock> mFoundBlocks = {};

    void moveToNext();
    void tryProcessSub(bool& processed, ChunkPos currentChunkPos, int subChunkIndex);
    bool processSub(ChunkPos processChunk, int subChunk);
    void reset();

    void onEnable() override;
    void onDisable() override;
    std::vector<int> getEnabledBlocks();
    void onBlockChangedEvent(class BlockChangedEvent& event);
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
    void onRenderEvent(class RenderEvent& event);
};