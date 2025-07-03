//
// Created by vastrakai on 7/7/2024.
//

#include "BlockUtils.hpp"

#include <src/Features/Modules/Player/Regen.hpp>
#include <src/Features/Modules/Player/OreMiner.hpp>

#include <Features/Events/BlockChangedEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Actor/SyncedPlayerMovementSettings.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerActionPacket.hpp>
#include <SDK/Minecraft/Network/Packets/InventoryTransactionPacket.hpp>
#include <SDK/Minecraft/Network/Packets/UpdateBlockPacket.hpp>
#include <SDK/Minecraft/World/Chunk/LevelChunk.hpp>
#include <SDK/Minecraft/World/Chunk/SubChunkBlockStorage.hpp>


bool BlockUtils::isGoodBlock(glm::ivec3 blockPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return false;
    Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);
    int blockId = block->toLegacy()->getBlockId();
    bool isLiquid = 8 <= blockId && blockId <= 11;
    return blockId != 0 && !isLiquid && block->toLegacy()->mSolid;
}


std::vector<BlockInfo> BlockUtils::getBlockList(const glm::ivec3& position, float r)
{
    std::vector<BlockInfo> blocks = {};

    const auto blockSource = ClientInstance::get()->getBlockSource();

    auto newBlocks = std::vector<BlockInfo>();


    const int radius = static_cast<int>(r);
    newBlocks.reserve(radius * radius * radius); // reserve enough space for all blocks

    for (int x = position.x - radius; x <= position.x + radius; x++)
        for (int y = position.y - radius; y <= position.y + radius; y++)
            for (int z = position.z - radius; z <= position.z + radius; z++)
                if (const auto block = blockSource->getBlock({ x, y, z }))
                    newBlocks.push_back({ block, { x, y, z } });


    return newBlocks;
}

bool BlockUtils::isOverVoid(glm::vec3 vec)
{
    vec = glm::floor(vec);
    std::vector<Block*> blocksUntilZero;

    auto player = ClientInstance::get()->getLocalPlayer();
    // get all the blocks from vec.y to 0
    for (int i = vec.y; i >= 0; i--)
    {
        auto block = ClientInstance::get()->getBlockSource()->getBlock({ vec.x, i, vec.z });
        blocksUntilZero.push_back(block);
    }

    // Return false if any of them are not air
    for (auto block : blocksUntilZero)
    {
        if (block->toLegacy()->getBlockId() != 0) return false;
    }

    return true;
}

glm::vec3 BlockUtils::findClosestBlockToPos(glm::vec3 pos)
{
    glm::vec3 closestBlock = glm::vec3(FLT_MAX, FLT_MAX, FLT_MAX);
    float closestDist = FLT_MAX;

    for (int x = pos.x - 5; x < pos.x + 5; x++)
        for (int y = pos.y - 5; y < pos.y + 5; y++)
            for (int z = pos.z - 5; z < pos.z + 5; z++)
            {
                auto blockPos = glm::vec3(x, y, z);
                Block* block = ClientInstance::get()->getBlockSource()->getBlock(blockPos);

                if (isGoodBlock(blockPos)) {
                    if (isGoodBlock(blockPos + glm::vec3(0, 1, 0)) && isGoodBlock(blockPos + glm::vec3(0, 2, 0)))
                    {
                        float distance = glm::distance(pos, blockPos);
                        if (distance < closestDist) {
                            closestDist = distance;
                            closestBlock = blockPos;
                        }
                    }
                }
            }

    if (closestBlock.x != FLT_MAX && closestBlock.y != FLT_MAX && closestBlock.z != FLT_MAX) {
        spdlog::info("Closest block to ({}, {}, {}) is at ({}, {}, {})", pos.x, pos.y, pos.z, closestBlock.x, closestBlock.y, closestBlock.z);
    }
    else {
        spdlog::info("No blocks found near ({}, {}, {})", pos.x, pos.y, pos.z);
    }

    return closestBlock;
}

/*
std::vector<BlockInfo> BlockUtils::getChunkBasedBlockList(const glm::ivec3& position, float r)
{
    std::vector<BlockInfo> blocks = {};

    const auto blockSource = ClientInstance::get()->getBlockSource();

    auto newBlocks = std::vector<BlockInfo>();


    const int radius = static_cast<int>(r);
    newBlocks.reserve(radius * radius * radius); // reserve enough space for all blocks

    glm::ivec3 min = position - glm::ivec3(radius);
    glm::ivec3 max = position + glm::ivec3(radius);
    auto chunks = getChunkList(min, max);

    if (chunks.empty()) return newBlocks;

    for (auto chunkPos : chunks)
    {
        LevelChunk* chunk = blockSource->getChunk(chunkPos);
        if (chunk->isLoading) continue; // Skip if chunk is loading
        if (!chunk) continue;

        int blockFound = 0;

        for (auto subchunk : chunk->subChunks)
        {
            auto readah = subchunk.blockReadPtr;
            if (!readah) continue;
            uint16_t searchRangeXZ = 16;
            uint16_t searchRangeY = (blockSource->mBuildHeight - blockSource->mBuildDepth) / chunk->subChunks.size();
            for(uint16_t x = 0; x < searchRangeXZ; x++)
            {
                for(uint16_t z = 0; z < searchRangeXZ; z++)
                {
                    for(uint16_t y = 0; y < searchRangeY; y++)
                    {
                        uint16_t elementId = (x * 0x10 + z) * 0x10 + (y & 0xf);
                        Block* found = readah->getElement(elementId);
                        BlockPos pos;
                        pos.x = (chunkPos.x * 16) + x;
                        pos.z = (chunkPos.y * 16) + z;
                        pos.y = y + (subchunk.subchunkIndex * 16);
                        //spdlog::debug("Block at ({}, {}, {}) is {}", pos.x, pos.y, pos.z, found->mLegacy->mName);
                        newBlocks.emplace_back(found, pos);
                    }
                }
            }
        }
    }

    return newBlocks;
}
*/



int BlockUtils::getBlockPlaceFace(glm::ivec3 blockPos)
{
    for (auto& [face, offset] : blockFaceOffsets)
        if (!isAirBlock(blockPos + glm::ivec3(offset))) return face;
    return -1;
}

int BlockUtils::getExposedFace(glm::ivec3 blockPos, bool useFilter)
{
    static std::vector<glm::ivec3> offsetList = {
        glm::ivec3(0, -1, 0),
        glm::ivec3(0, 1, 0),
        glm::ivec3(0, 0, -1),
        glm::ivec3(0, 0, 1),
        glm::ivec3(-1, 0, 0),
        glm::ivec3(1, 0, 0),
    };
    for (int i = 0; i < offsetList.size(); i++) {
        glm::ivec3 checkPos = blockPos + offsetList[i];
        if (isAirBlock(checkPos)) return i;
        if (useFilter)
        {
            if (!ClientInstance::get()->getBlockSource()->getBlock(checkPos)->mLegacy->mSolid) return i;
        }
    }
    return -1;
}

bool BlockUtils::isAirBlock(glm::ivec3 blockPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return false;
    int blockId = ClientInstance::get()->getBlockSource()->getBlock(blockPos)->toLegacy()->getBlockId();
    bool isLiquid = 8 <= blockId && blockId <= 11;
    return blockId == 0 || isLiquid;
}

glm::ivec3 BlockUtils::getClosestPlacePos(glm::ivec3 pos, float distance)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return pos;

    for (int i = 2; i < 6; i++) {
        glm::vec3 blockSel = pos + glm::ivec3(blockFaceOffsets[i]);
        if (isValidPlacePos(blockSel) && isAirBlock(blockSel)) return blockSel;
    }

    glm::vec3 playerPos = *player->getPos() - glm::vec3(0, PLAYER_HEIGHT + 1.f, 0);
    playerPos = glm::floor(playerPos);

    auto closestBlock = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
    float closestDist = FLT_MAX;

    for (int x = pos.x - distance; x < pos.x + distance; x++)
        for (int y = pos.y - distance; y < pos.y + distance; y++)
            for (int z = pos.z - distance; z < pos.z + distance; z++)
            {
                auto blockSel = glm::vec3(x, y, z);
                if (isValidPlacePos(blockSel) && isAirBlock(blockSel)) {
                    float distance = glm::distance(playerPos, blockSel);
                    if (distance < closestDist) {
                        closestDist = distance;
                        closestBlock = blockSel;
                    }
                }
            }

    if (closestBlock.x != INT_MAX && closestBlock.y != INT_MAX && closestBlock.z != INT_MAX) {
        return closestBlock;
    }

    for (int i = 0; i < 1; i++) {
        glm::ivec3 blockSel = pos + glm::ivec3(blockFaceOffsets[i]);
        if (isValidPlacePos(blockSel)) return blockSel;
    }

    return { INT_MAX, INT_MAX, INT_MAX };
}

bool BlockUtils::isValidPlacePos(glm::ivec3 blockPos)
{
    return getBlockPlaceFace(blockPos) != -1;
}

void BlockUtils::placeBlock(glm::vec3 pos, int side)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    glm::ivec3 blockPos = pos;
    if (side == -1) side = getBlockPlaceFace(blockPos);

    glm::vec3 vec = blockPos;

    if (side != -1) vec += blockFaceOffsets[side] * 0.5f;

    HitResult* res = player->getLevel()->getHitResult();

    vec += blockFaceOffsets[side] * 0.5f;

    res->mBlockPos = vec;
    res->mFacing = side;

    res->mType = HitType::BLOCK;
    res->mIndirectHit = false;
    res->mRayDir = vec;
    res->mPos = blockPos;

    bool oldSwinging = player->isSwinging();
    int oldSwingProgress = player->getSwingProgress();
    player->getGameMode()->buildBlock(blockPos + glm::ivec3(blockFaceOffsets[side]) , side, true);
    player->setSwinging(oldSwinging);
    player->setSwingProgress(oldSwingProgress);

    vec += blockFaceOffsets[side] * 0.5f;

    res->mBlockPos = vec;
    res->mFacing = side;

    res->mType = HitType::BLOCK;
    res->mIndirectHit = false;
    res->mRayDir = vec;
    res->mPos = blockPos;


    /*auto transac = MinecraftPackets::createPacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ItemUseInventoryTransaction>();
    cit->mActionType = ItemUseInventoryTransaction::ActionType::Place;
    int slot = player->getSupplies()->mSelectedSlot;
    cit->mSlot = slot;
    cit->mItemInHand = *player->getSupplies()->getContainer()->getItem(slot);
    cit->mBlockPos = blockPos + glm::ivec3(blockFaceOffsets[side]);
    cit->mFace = side;
    cit->mTargetBlockRuntimeId = 0;
    cit->mPlayerPos = *player->getPos();

    glm::vec3 pPos = *player->getPos() - glm::vec3(0, PLAYER_HEIGHT, 0);
    glm::vec3 clickPos = glm::vec3(blockPos) - pPos;


    cit->mClickPos = clickPos;

    transac->mTransaction = std::move(cit);
    PacketUtils::queueSend(transac, false);

    ItemStack* item = player->getSupplies()->getContainer()->getItem(slot);
    if (!item->mItem) return;
    if (!item->mBlock) return;

    setBlock(blockPos, item->mBlock);*/

}

void BlockUtils::startDestroyBlock(glm::vec3 pos, int side)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    glm::ivec3 blockPos = pos;
    if (side == -1) side = getExposedFace(blockPos);

    glm::vec3 vec = blockPos;

    if (side != -1) vec += blockFaceOffsets[side] * -0.5f;

    HitResult* res = player->getLevel()->getHitResult();

    vec += blockFaceOffsets[side] * 0.5f;

    res->mBlockPos = vec;
    res->mFacing = side;

    res->mType = HitType::BLOCK;
    res->mIndirectHit = false;
    res->mRayDir = vec;
    res->mPos = blockPos;

    bool isDestroyedOut = false;
    player->getGameMode()->startDestroyBlock(blockPos, side, isDestroyedOut);
    //if (!isDestroyedOut) player->getGameMode()->continueDestroyBlock(blockPos, side, *player->getPos(), isDestroyedOut);

    vec += blockFaceOffsets[side] * -0.5f;

    res->mBlockPos = vec;
    res->mFacing = side;

    res->mType = HitType::BLOCK;
    res->mIndirectHit = false;
    res->mRayDir = vec;
    res->mPos = blockPos;
}

static constexpr int AIR_RUNTIME_ID = 3690217760;

void BlockUtils::clearBlock(const glm::ivec3& pos)
{
    setBlock(pos, AIR_RUNTIME_ID);
}

void BlockUtils::setBlock(const glm::ivec3& pos, Block* block) {
    ClientInstance::get()->getBlockSource()->setBlock(pos, block);
}

void BlockUtils::setBlock(const glm::ivec3& pos, unsigned int runtimeId) {
    Block* oldBlock = ClientInstance::get()->getBlockSource()->getBlock(pos);
    std::shared_ptr<UpdateBlockPacket> p = MinecraftPackets::createPacket<UpdateBlockPacket>();
    p->mPos = pos;
    p->mLayer = UpdateBlockPacket::BlockLayer::Standard;
    p->mUpdateFlags = BlockUpdateFlag::Priority;
    p->mBlockRuntimeId = runtimeId;
    PacketUtils::sendToSelf(p);
    Block* newBlock = ClientInstance::get()->getBlockSource()->getBlock(pos);

    if (oldBlock == newBlock)
    {
        spdlog::warn("setBlock operation failed, block at ({}, {}, {}) is still the same [rtid: {}]", pos.x, pos.y, pos.z, runtimeId);
    }

    // Fire a block update event because sendToSelf doesn't trigger it
    auto holder = nes::make_holder<BlockChangedEvent>(pos, newBlock, oldBlock, 0, ClientInstance::get()->getLocalPlayer());
    gFeatureManager->mDispatcher->trigger(holder);
}

void BlockUtils::destroyBlock(glm::vec3 pos, int side, bool useTransac)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    glm::ivec3 blockPos = pos;
    if (side == -1) side = getBlockPlaceFace(blockPos);

    if (!useTransac)
    {
        bool oldSwinging = player->isSwinging();
        int oldSwingProgress = player->getSwingProgress();

        player->getGameMode()->destroyBlock(blockPos, side);
        player->getGameMode()->stopDestroyBlock(blockPos);

        player->setSwinging(oldSwinging);
        player->setSwingProgress(oldSwingProgress);
        spdlog::info("Destroyed block at ({}, {}, {}) [using game mode]", blockPos.x, blockPos.y, blockPos.z);
        return;
    }

    auto actionPkt = MinecraftPackets::createPacket<PlayerActionPacket>();
    actionPkt->mPos = blockPos;
    actionPkt->mResultPos = blockPos;
    actionPkt->mFace = side;
    actionPkt->mAction = PlayerActionType::StopDestroyBlock;
    actionPkt->mRuntimeId = player->getRuntimeID();
    actionPkt->mtIsFromServerPlayerMovementSystem = false;

    PacketUtils::queueSend(actionPkt, false);
    auto pkt = MinecraftPackets::createPacket<InventoryTransactionPacket>();

    auto cit = std::make_unique<ItemUseInventoryTransaction>();
    cit->mActionType = ItemUseInventoryTransaction::ActionType::Destroy;
    int slot = player->getSupplies()->mSelectedSlot;
    cit->mSlot = slot;
    cit->mItemInHand = NetworkItemStackDescriptor(*player->getSupplies()->getContainer()->getItem(slot));
    cit->mBlockPos = blockPos;
    cit->mFace = side;
    cit->mTargetBlockRuntimeId = 0x92;
    cit->mPlayerPos = *player->getPos();

    cit->mClickPos = glm::vec3(0, 0, 0); // this is correct, i actually checked it this time
    pkt->mTransaction = std::move(cit);
    PacketUtils::queueSend(pkt, false);
    clearBlock(blockPos);
    spdlog::info("Destroyed block at ({}, {}, {}) [using transac]", blockPos.x, blockPos.y, blockPos.z);
}

bool BlockUtils::isMiningPosition(glm::ivec3 blockPos) {
    auto player = ClientInstance::get()->getLocalPlayer();

    if (0 < player->getGameMode()->mBreakProgress && player->getLevel()->getHitResult()->mBlockPos == blockPos) return true;

    if (Regen::mIsMiningBlock && Regen::mCurrentBlockPos == blockPos) return true;

    if (OreMiner::mIsMiningBlock && OreMiner::mCurrentBlockPos == blockPos) return true;

    return false;
}

std::vector<ChunkPos> BlockUtils::getChunkList(const glm::ivec3 min, const glm::ivec3 max)
{
    std::vector<ChunkPos> chunks;
    for (int x = min.x; x < max.x; x += 16)
        for (int y = min.y; y < max.y; y += 16)
            for (int z = min.z; z < max.z; z += 16)
                chunks.emplace_back(BlockPos(x, y, z));

    return chunks;
}
