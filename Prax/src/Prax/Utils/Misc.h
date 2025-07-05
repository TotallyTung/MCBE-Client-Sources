#pragma once

#include <imgui.h>
#include <iostream>
#include <Prax/SDK/Game/Misc/Structs.h>
#include <random>


enum class PlayerAttributes {
    HEALTH,
    HUNGER,
    MOVEMENT_SPEED,
    ABSORPTION,
    SATURATION,
    FOLLOW_RANGE,
    LEVEL,
    EXPERIENCE
};

class Actor;

struct BlockPosPair {
    class Block* block = nullptr;
    Vector3 blockPos = Vector3();

    BlockPosPair(Vector3 pos, class Block* block) {
        this->block = block;
        this->blockPos = pos;
    }

    AABB GetAABB() {
        return AABB(blockPos, Vector3(1, 1, 1));
    }

    Vector3 GetCenteredPos() {
        // Floor and center
        return Vector3(floor(blockPos.x) + 0.5f, floor(blockPos.y) + 0.5f, floor(blockPos.z) + 0.5f);
    }
};

struct BlockPosIdPair {
    Vector3 blockPos;
    int blockId;

    BlockPosIdPair(Vector3 pos, int id) {
        this->blockPos = pos;
        this->blockId = id;
    }

    AABB GetAABB() {
        return AABB(blockPos, Vector3(1, 1, 1));
    }
};

class Misc {
public:
    static int64_t ID;

    static std::map<Actor*,AABB> GetAABBs(bool addLocalPlayer);

    static Vector2 GetMotion(float playerYaw, float value, bool allowStrafe = true);

    static float GetRotationKeyOffset(bool allowStrafe = true);

    static bool IsUsingMovementKeys();

    static bool IsUsingMovementKeys(bool W, bool A, bool S, bool D, bool SPACE);

    static BlockPos GetClosestPlacePos(BlockPos block, float maxDistance, bool yLocked, float lockedY);

    static bool IsValidPlacePos(BlockPos pos);

    static BlockPos GetSide(int side);

    static bool IsAirBlock(BlockPos pos);

    static int GetBlockPlaceFace(BlockPos pos);

    static int64_t GetCurrentMs();

    static void SetClipboard(std::string str);

    static void PlaySoundFromUrl(const std::string& url, float volume = 1.f, bool loop = false);

    static Vector2 GetRotations(Vector3* playerEyePos, Vector3 *targetPos);

    static Vector2 GetRotations(Vector3& playerEyePos, AABB& targetPos);

    static Vector3 GetFastestBreakableSide(Vector3 block);

    static int GetBestBreakingTool(class Block *block);

    static float GetDestroySpeed(int slot, Block *block, float destroySpeedDivisor);

    static void SwitchAndEquip(int slot, bool switchBack = true);

    static bool SpoofSwitch(int slot);

    static bool CheckForHexEdit();

    template<typename T>
    static T RandomNum(T min, T max) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_int_distribution<T> dis(min, max);
        return dis(gen);
    }

    static bool IsOverVoid(Vector3 pos, bool check3x3 = true);
    static int GetExposedBlockFace(BlockPos bp);
    static Vector3 GetClosestExposedRedstone(float maxDistance);
    static Vector3 GetClosestExposedLitRedstone(float maxDistance);
    static int64_t GetBlockBreakTime(float destroySpeed);
    static bool IsOreBlock(Block* block);
    static std::map<int, BlockPosPair> GetSurroundingBlocks(Vector3 pos, int doubleThick = 0);
    static bool IsBlockNear(Vector3 position, float distance);


    static std::map<int, Vector3> SideMap;



    static bool PlaceBlock(Vector3 bp, int side, bool swing = true, bool calcSide = true, bool rayTraceResult = true, int slot = -1, int raycastMode = 0);

    static int GetPlaceableItemOnBlock(Vector3 blockPos, bool hotbarOnly, bool prioritizeHighest);

    static AABB GetActorAABB(Actor *actor, bool inc = true);
    static Vector3 GetClosestAabbPos(Actor *actor, Vector3 *playerEyePos);
    static Vector3 GetLerpedActorPos(Actor *actor);
    static float GetClosestBlockDistance();
    static std::string GetModuleOfAddress(uintptr_t address);

    static std::string GetNameFromActor(Actor *target);
    static uint32_t Hashfnv1a32(const char* str);
    static int GetItemSlot(const std::string& name, bool hotbarOnly);

    static Actor *GetActorFromRuntimeID(int64_t id);
    static std::vector<Vector3> GetBlocksWithinDistance(Vector3 *pos, float maxDistance);
    static std::vector<BlockPosPair> GetBlocksWithinDistance2(Vector3 *pos, float maxDistance, bool exposedOnly = false, bool sort = true);
    static std::vector<BlockPosIdPair> GetBlocksWithinDistance3(Vector3* pos, float maxDistance, bool exposedOnly, bool sort);
    static BlockPos GetClosestPlacePos2(BlockPos block, float maxDistance, bool yLocked, float lockedY);
    static int GetModuleCount(); // Gets the amount of modules loaded in the current process
    static int CountExposedSides(BlockPos bp);
};
