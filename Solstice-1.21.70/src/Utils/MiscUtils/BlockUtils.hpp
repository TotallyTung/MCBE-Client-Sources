#pragma once
//
// Created by vastrakai on 7/7/2024.
//
#include <map>
#include <Utils/Structs.hpp>
#include <SDK/Minecraft/World/Block.hpp>
#include <vector>



struct BlockInfo {
    Block* mBlock;
    glm::ivec3 mPosition;

    AABB getAABB() {
        return AABB(mPosition, glm::vec3(1, 1, 1));
    }

    float getDistance(glm::vec3 pos) {
        glm::vec3 closest = getAABB().getClosestPoint(pos);
        return glm::distance(closest, pos);
    }

    BlockInfo(Block* block, glm::ivec3 position) : mBlock(block), mPosition(position) {}
};

struct DestroySpeedInfo {
    std::string blockName;
    float destroySpeed;
};

class BlockUtils {
public:
    static inline std::map<int, glm::vec3> clickPosOffsets = {
        {0, {0.5, -0, 0.5}},
        {1, {0.5, 1, 0.5}},
        {2, {0.5, 0.5, 0}},
        {3, {0.5, 0.5, 1}},
        {4, {0, 0.5, 0.5}},
        {5, {1, 0.5, 0.5}},
    };
    static std::vector<BlockInfo> getBlockList(const glm::ivec3& position, float r);
    static bool isOverVoid(glm::vec3 vec);
    static glm::vec3 findClosestBlockToPos(glm::vec3 pos);
    //static std::vector<BlockInfo> getChunkBasedBlockList(const glm::ivec3& position, float r);

    /*if (!IsAirBlock(pos + BlockPos(0, -1, 0))) return 1;
if (!IsAirBlock(pos + BlockPos(0, 0, 1))) return 2;
if (!IsAirBlock(pos + BlockPos(0, 0, -1))) return 3;
if (!IsAirBlock(pos + BlockPos(1, 0, 0))) return 4;
if (!IsAirBlock(pos + BlockPos(-1, 0, 0))) return 5;
if (!IsAirBlock(pos + BlockPos(0, 1, 0))) return 0;*/
    static inline std::map<int, glm::vec3> blockFaceOffsets = {
        {1, glm::ivec3(0, -1, 0)},
        {2, glm::ivec3(0, 0, 1)},
        {3, glm::ivec3(0, 0, -1)},
        {4, glm::ivec3(1, 0, 0)},
        {5, glm::ivec3(-1, 0, 0)},
        {0, glm::ivec3(0, 1, 0)}
    };

    // Dynamic Destroy Speed
    static inline std::vector<DestroySpeedInfo> mDynamicSpeeds = { // make sure to include minecraft: before block names
        {"minecraft:green_concrete", 0.65f},
        {"minecraft:lime_terracotta", 0.65f},
        {"minecraft:sand", 0.67f},
        {"minecraft:dirt", 0.65f},
        {"minecraft:grass_block", 0.67f},
        {"minecraft:stone", 0.67f},
        {"minecraft:sandstone", 0.57f},
        {"minecraft:sandstone_slab", 0.57f},
        {"minecraft:moss_block", 0.57f},
    };

    // Dynamic Destroy Speed 2
    static inline std::vector<DestroySpeedInfo> mNukeSpeeds = { // make sure to include minecraft: before block names
        {"minecraft:sand", 0.24f},
        {"minecraft:dirt", 0.24f},
    };

    static int getBlockPlaceFace(glm::ivec3 blockPos);
    static int getExposedFace(glm::ivec3 blockPos, bool useFilter = false);
    static bool isGoodBlock(glm::ivec3 blockPos);
    static bool isAirBlock(glm::ivec3 blockPos);
    static glm::ivec3 getClosestPlacePos(glm::ivec3 pos, float distance);
    static bool isValidPlacePos(glm::ivec3 pos);
    static void placeBlock(glm::vec3 pos, int side);
    static void startDestroyBlock(glm::vec3 pos, int side);
    static void clearBlock(const glm::ivec3& pos);
    static void setBlock(const glm::ivec3& pos, Block* block);
    static void setBlock(const glm::ivec3& pos, unsigned int runtimeId);
    static void destroyBlock(glm::vec3 pos, int side, bool useTransac = false);
    static bool isMiningPosition(glm::ivec3 blockPos);
    // Converts a min pos, and max pos, to a list of chunks that are in that range
    static std::vector<struct ChunkPos> getChunkList(const glm::ivec3 min, const glm::ivec3 max);

};