//
// Created by vastrakai on 7/7/2024.
//

#include "BlockESP.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BlockChangedEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerActionPacket.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/Chunk/LevelChunk.hpp>
#include <SDK/Minecraft/World/Chunk/SubChunkBlockStorage.hpp>


static std::mutex blockMutex = {};

constexpr int REDSTONE_ORE = 73;
constexpr int REDSTONE_ORE_LIT = 74;
constexpr int DIAMOND_ORE = 56;
constexpr int EMERALD_ORE = 129;
constexpr int GOLD_ORE = 14;
constexpr int IRON_ORE = 15;
constexpr int LAPIS_ORE = 21;
constexpr int COAL_ORE = 16;
constexpr int BARRIER = 416;
constexpr int DEEPSLATE_REDSTONE_ORE = 658;
constexpr int DEEPSLATE_LIT_REDSTONE_ORE = 659;
constexpr int DEEPSLATE_DIAMOND_ORE = 660;
constexpr int DEEPSLATE_EMERALD_ORE = 662;
constexpr int DEEPSLATE_GOLD_ORE = 657;
constexpr int DEEPSLATE_IRON_ORE = 656;
constexpr int DEEPSLATE_LAPIS_ORE = 655;
constexpr int DEEPSLATE_COAL_ORE = 661;
constexpr int PORTAL = 90;

constexpr int CHEST = 54;
constexpr int ENDER_CHEST = 130;
constexpr int TRAPPED_CHEST = 146;
constexpr int BARREL = 458;

std::unordered_map<int, ImColor> blockColors = {
    { 73, ImColor(1.f, 0.f, 0.f, 1.f) },
    { 74, ImColor(1.f, 0.f, 0.f, 1.f) },
    { 658, ImColor(1.f, 0.f, 0.f, 1.f) },
    { 659, ImColor(1.f, 0.f, 0.f, 1.f) },
    { 56, ImColor(0.f, 1.f, 1.f, 1.f) },
    { 660, ImColor(0.f, 1.f, 1.f, 1.f) },
    { 129, ImColor(0.f, 1.f, 0.f, 1.f) },
    { 662, ImColor(0.f, 1.f, 0.f, 1.f) },
    { 14, ImColor(1.f, 1.f, 0.f, 1.f) },
    { 657, ImColor(1.f, 1.f, 0.f, 1.f) },
    { 15, ImColor(1.f, 0.5f, 0.f, 1.f) },
    { 656, ImColor(1.f, 0.5f, 0.f, 1.f) },
    { 21, ImColor(0.f, 0.f, 1.f, 1.f) },
    { 655, ImColor(0.f, 0.f, 1.f, 1.f) },
    { 16, ImColor(0.f, 0.f, 0.f, 1.f) },
    { 661, ImColor(0.f, 0.f, 0.f, 1.f) },
    // purple portal
    { 90, ImColor(0.5f, 0.f, 0.5f, 1.f) },

    { 130, ImColor(0.5f, 0.f, 0.5f, 1.f) },
    { 54, ImColor(255, 165, 0) },
    { 146, ImColor(255, 165, 0) },
    { 458, ImColor(255, 165, 0) }
};

bool isValidBlock(int id)
{
    return blockColors.contains(id);
}

ImColor getColorFromId(int id)
{
    if (blockColors.contains(id))
    {
        return blockColors[id];
    }

    return ImColor(1.f, 1.f, 1.f, 1.f);
}

void BlockESP::moveToNext()
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return;
    }
    ClientInstance* ci = ClientInstance::get();
    Actor* player = ci->getLocalPlayer();
    if (!player) return;
    BlockSource* blockSource = ci->getBlockSource();

    // da search pattern
    static const std::vector<std::pair<int, int>> directions = {
        { 1, 0 },
        { 0, 1 },
        { -1, 0 },
        { 0, -1 }
    };

    size_t numSubchunks = (blockSource->getBuildHeight() - blockSource->getBuildDepth()) / 16;
    if(numSubchunks-1 > mSubChunkIndex)
    {
        mSubChunkIndex++;
        //spdlog::debug("Moving to next subchunk [scIndex: {}/{}, chunkPos: ({}, {})]", mSubChunkIndex, numSubchunks, mCurrentChunkPos.x, mCurrentChunkPos.y);
        return;
    }

    mCurrentChunkPos.x += directions[mDirectionIndex].first;
    mCurrentChunkPos.y += directions[mDirectionIndex].second;

    mStepsCount++;
    if (mStepsCount >= mSteps) {
        mStepsCount = 0;
        mDirectionIndex = (mDirectionIndex + 1) % directions.size();
        if (mDirectionIndex % 2 == 0) {
            mSteps++;
        }
    }

    mSubChunkIndex = 0;
    //spdlog::debug("Moving to next subchunk [scIndex: {}/{}, chunkPos: ({}, {})]", mSubChunkIndex, numSubchunks, mCurrentChunkPos.x, mCurrentChunkPos.y);
}

void BlockESP::tryProcessSub(bool& processed, ChunkPos currentChunkPos, int subChunkIndex)
{
    TRY_CALL([&]()
    {
        if (processSub(currentChunkPos, subChunkIndex))
        {
            processed = true;
        }
    });
}

bool BlockESP::processSub(ChunkPos processChunk, int index)
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return false;
    }
    ClientInstance* ci = ClientInstance::get();
    Actor* player = ci->getLocalPlayer();
    if (!player) return false;
    BlockSource* blockSource = ci->getBlockSource();

    size_t numSubchunks = (blockSource->getBuildHeight() - blockSource->getBuildDepth()) / 16;
    if (index < 0 || index >= numSubchunks) return false;

    LevelChunk* chunk = blockSource->getChunk(processChunk);
    if (!chunk) return false;

    // Get the nmber

    auto subChunk = (*chunk->getSubChunks())[index];
    SubChunkBlockStorage* blockReader = subChunk.blockReadPtr;
    if (!blockReader) return false;

    std::vector<int> enabledBlocks = getEnabledBlocks();

    for(uint16_t x = 0; x < 16; x++)
    {
        for(uint16_t z = 0; z < 16; z++)
        {
            for(uint16_t y = 0; y < (blockSource->getBuildHeight() - blockSource->getBuildDepth()) / chunk->getSubChunks()->size(); y++)
            {
                uint16_t elementId = (x * 0x10 + z) * 0x10 + (y & 0xf);
                const Block* found = blockReader->getElement(elementId);
                if (found->mLegacy->getBlockId() == 0)
                {
                    mFoundBlocks.erase(BlockPos((processChunk.x * 16) + x, y + (subChunk.subchunkIndex * 16), (processChunk.y * 16) + z));
                    continue;
                }
                if (std::ranges::find(enabledBlocks, found->mLegacy->getBlockId()) == enabledBlocks.end()) continue;

                BlockPos pos;
                pos.x = (processChunk.x * 16) + x;
                pos.z = (processChunk.y * 16) + z;
                pos.y = y + (subChunk.subchunkIndex * 16);

                int exposedFace = BlockUtils::getExposedFace(pos);
                if (mOnlyExposedOres.mValue && exposedFace == -1) {
                    continue;
                }

                //spdlog::debug("Block at ({}, {}, {}) is {} [{}]", pos.x, pos.y, pos.z, found->mLegacy->mName, found->mLegacy->getBlockId());
                mFoundBlocks[pos] = { found, AABB(pos, glm::vec3(1.f, 1.f, 1.f)), getColorFromId(found->mLegacy->getBlockId()) };
            }
        }
    }


    return true;
}

void BlockESP::reset()
{
    std::lock_guard<std::mutex> lock(blockMutex); // Lock mutex

    ClientInstance* ci = ClientInstance::get();
    Actor* player = ci->getLocalPlayer();
    mSearchStart = NOW;
    mFoundBlocks.clear();
    mStepsCount = 0;
    mSteps = 1;
    mDirectionIndex = 0;
    mSubChunkIndex = 0;
    if (!player) return;
    BlockSource* blockSource = ci->getBlockSource();

    mSearchCenter = ChunkPos(*player->getPos());
    mCurrentChunkPos = mSearchCenter;
}

void BlockESP::onEnable()
{
    gFeatureManager->mDispatcher->listen<RenderEvent, &BlockESP::onRenderEvent, nes::event_priority::VERY_FIRST>(this);
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &BlockESP::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<BlockChangedEvent, &BlockESP::onBlockChangedEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &BlockESP::onPacketInEvent>(this);
    reset();
}

void BlockESP::onDisable()
{
    gFeatureManager->mDispatcher->deafen<RenderEvent, &BlockESP::onRenderEvent>(this);
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &BlockESP::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<BlockChangedEvent, &BlockESP::onBlockChangedEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &BlockESP::onPacketInEvent>(this);
    reset();
}

std::vector<int> BlockESP::getEnabledBlocks()
{
    std::vector<int> enabledBlocks = {};

    // Insert deepslate ores as well
    if (mEmerald.mValue)
    {
        enabledBlocks.push_back(EMERALD_ORE);
        enabledBlocks.push_back(DEEPSLATE_EMERALD_ORE);
    }
    if (mDiamond.mValue)
    {
        enabledBlocks.push_back(DIAMOND_ORE);
        enabledBlocks.push_back(DEEPSLATE_DIAMOND_ORE);
    }
    if (mGold.mValue)
    {
        enabledBlocks.push_back(GOLD_ORE);
        enabledBlocks.push_back(DEEPSLATE_GOLD_ORE);
    }
    if (mIron.mValue)
    {
        enabledBlocks.push_back(IRON_ORE);
        enabledBlocks.push_back(DEEPSLATE_IRON_ORE);
    }
    if (mCoal.mValue)
    {
        enabledBlocks.push_back(COAL_ORE);
        enabledBlocks.push_back(DEEPSLATE_COAL_ORE);
    }
    if (mRedstone.mValue)
    {
        enabledBlocks.push_back(REDSTONE_ORE);
        enabledBlocks.push_back(REDSTONE_ORE_LIT);
        enabledBlocks.push_back(DEEPSLATE_REDSTONE_ORE);
        enabledBlocks.push_back(DEEPSLATE_LIT_REDSTONE_ORE);
    }
    if (mLapis.mValue)
    {
        enabledBlocks.push_back(LAPIS_ORE);
        enabledBlocks.push_back(DEEPSLATE_LAPIS_ORE);
    }

    if (mPortal.mValue)
    {
        enabledBlocks.push_back(PORTAL);
    }

    if (mChests.mValue)
    {
        enabledBlocks.push_back(CHEST);
        enabledBlocks.push_back(ENDER_CHEST);
        enabledBlocks.push_back(TRAPPED_CHEST);
        enabledBlocks.push_back(BARREL);
    }

    return enabledBlocks;
}

void BlockESP::onBlockChangedEvent(BlockChangedEvent& event)
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return;
    }
    std::lock_guard<std::mutex> lock(blockMutex); // Lock mutex

    auto dabl = BlockInfo(event.mNewBlock, event.mBlockPos);
    if (dabl.getDistance(*ClientInstance::get()->getLocalPlayer()->getPos()) > mRadius.mValue) return;

    ChunkPos chunkPos = ChunkPos(event.mBlockPos);
    int subChunk = (event.mBlockPos.y - ClientInstance::get()->getBlockSource()->getBuildDepth()) >> 4;
    bool result = false;
    tryProcessSub(result, chunkPos, subChunk);

    if (!result) {
        spdlog::critical("Failed to process subchunk [scIndex: {}/{}, chunkPos: ({}, {})]", subChunk, (ClientInstance::get()->getBlockSource()->getBuildHeight() - ClientInstance::get()->getBlockSource()->getBuildDepth()) / 16, chunkPos.x, chunkPos.y);
    }

    auto enabledBlocks = getEnabledBlocks();

    if (isValidBlock(event.mNewBlock->mLegacy->getBlockId()) && std::ranges::find(enabledBlocks, event.mNewBlock->mLegacy->getBlockId()) != enabledBlocks.end())
    {
        const Block* block = event.mNewBlock;
        mFoundBlocks[event.mBlockPos] = { block, AABB(event.mBlockPos, glm::vec3(1.f, 1.f, 1.f)), getColorFromId(block->mLegacy->getBlockId()) };
        spdlog::debug("event.mNewBlock->mLegacy->mName: {} event.mOldBlock->mLegacy->mName: {}", event.mNewBlock->mLegacy->mName, event.mOldBlock->mLegacy->mName);
    }
    else
    {
        mFoundBlocks.erase(event.mBlockPos);
    }
};

void BlockESP::onBaseTickEvent(BaseTickEvent& event)
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return;
    }
    std::lock_guard<std::mutex> lock(blockMutex); // Lock mutex

    static uint64_t lastUpdate = 0;
    uint64_t freq = mUpdateFrequency.mValue * 50.f;
    uint64_t now = NOW;

    if (lastUpdate + freq > now) return;

    lastUpdate = now;
    auto ci = ClientInstance::get();
    auto player = ci->getLocalPlayer();
    if (!player) return;
    auto blockSource = ci->getBlockSource();

    if (glm::distance(glm::vec2(mCurrentChunkPos), glm::vec2(mSearchCenter)) > mChunkRadius.mValue)
    {
        //spdlog::debug("Resetting search, found {} block of interest in {}ms", mFoundBlocks.size(), NOW - mSearchStart);
        mSearchStart = NOW;
        mSearchCenter = ChunkPos(*player->getPos());
        mCurrentChunkPos = mSearchCenter;
        mStepsCount = 0;
        mSteps = 1;
        mDirectionIndex = 0;
        mSubChunkIndex = 0;
    }

    for (int i = 0; i < mChunkUpdatesPerTick.mValue; i++)
    {
        bool processed = false;
        tryProcessSub(processed, mCurrentChunkPos, mSubChunkIndex);
        if (!processed)
        {
            spdlog::critical("Failed to process subchunk [scIndex: {}/{}, chunkPos: ({}, {})]", mSubChunkIndex, (blockSource->getBuildHeight() - blockSource->getBuildDepth()) / 16, mCurrentChunkPos.x, mCurrentChunkPos.y);
        }
        moveToNext();
    }

    BlockPos playerPos = *player->getPos();

    int subChunk = (playerPos.y - ClientInstance::get()->getBlockSource()->getBuildDepth()) >> 4;
    bool result = false;
    tryProcessSub(result, ChunkPos(playerPos), subChunk);

    if (!result)
    {
        spdlog::critical("Failed to process subchunk [scIndex: {}/{}, chunkPos: ({}, {})]", subChunk, (blockSource->getBuildHeight() - blockSource->getBuildDepth()) / 16, playerPos.x, playerPos.z);
    }

}

void BlockESP::onPacketInEvent(PacketInEvent& event)
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return;
    }

    if (event.mPacket->getId() == PacketID::ChangeDimension)
    {
        reset(); // Reset the search when changing dimensions
    }

    if (event.mPacket->getId() == PacketID::PlayerAction)
    {
        auto packet = event.getPacket<PlayerActionPacket>();
        if (packet->mAction == PlayerActionType::Respawn) reset();
    }
}

void BlockESP::onRenderEvent(RenderEvent& event)
{
    if (!ClientInstance::get()->getLevelRenderer()) {
        reset();
        return;
    }

    if (ClientInstance::get()->getMouseGrabbed()) return;

    std::lock_guard<std::mutex> lock(blockMutex); // Lock mutex

    auto drawList = ImGui::GetBackgroundDrawList();

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || !ClientInstance::get()->getLevelRenderer())
    {
        reset();
        return;
    }

    glm::ivec3 playerPos = *player->getPos();

    if (mRenderCurrentChunk.mValue)
    {
        ChunkPos currentChunkPos = ChunkPos(mCurrentChunkPos);
        glm::vec3 pos = glm::vec3(currentChunkPos.x * 16, 0, currentChunkPos.y * 16);

        // Render the current chunk
        AABB chunkAABB = AABB(pos, glm::vec3(16.f, 1.f, 16.f));
        std::vector<ImVec2> chunkPoints = MathUtils::getImBoxPoints(chunkAABB);

        if (mRenderMode.mValue == BlockRenderMode::Both || mRenderMode.mValue == BlockRenderMode::Outline) {
            drawList->AddPolyline(chunkPoints.data(), chunkPoints.size(), ImColor(1.f, 1.f, 1.f), 0, 2.0f);
        }
        if (mRenderMode.mValue == BlockRenderMode::Both || mRenderMode.mValue == BlockRenderMode::Filled) {
            drawList->AddConvexPolyFilled(chunkPoints.data(), chunkPoints.size(), ImColor(1.f, 1.f, 1.f, 0.25f));
        }
    }

    auto enabled = getEnabledBlocks();

    for (auto& [pos, block] : mFoundBlocks)
    {
        if (distance(glm::vec3(pos), glm::vec3(playerPos)) > mRadius.mValue) continue;
        if (std::ranges::find(enabled, block.block->mLegacy->getBlockId()) == enabled.end()) continue;

        ImColor& color = block.color;
        AABB& blockAABB = block.aabb;
        std::vector<ImVec2> imPoints = MathUtils::getImBoxPoints(blockAABB);

        if (mRenderMode.mValue == BlockRenderMode::Both || mRenderMode.mValue == BlockRenderMode::Outline) {
            drawList->AddPolyline(imPoints.data(), imPoints.size(), color, 0, 2.0f);
        }
        if (mRenderMode.mValue == BlockRenderMode::Both || mRenderMode.mValue == BlockRenderMode::Filled) {
            drawList->AddConvexPolyFilled(imPoints.data(), imPoints.size(), ImColor(color.Value.x, color.Value.y, color.Value.z, 0.25f));
        }
    }
}