#pragma once

#include "Misc.h"
#include "System/KeyManager.h"
#include "Math.h"
#include "String.h"
#include "Audio.h"
#include <windows.h>

#include <chrono>
#include <playsoundapi.h>
#include <algorithm>
#include <imgui_internal.h>
#include <Psapi.h>
#include <Prax/SDK/Game/Network/Other/MobEquipmentPacket.h>
#include <xorstr.hpp>
#include <TlHelp32.h>
#include <Prax/ExceptionHandler.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/SDK/Game/Network/Other/TextPacket.h>
#include <Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h>
#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>

Audio audioManager;



std::map<Actor*,AABB> Misc::GetAABBs(bool addLocalPlayer) {
    std::map<Actor*,AABB> aabbs;
    if (!Minecraft::ClientInstance->ThePlayer()) return aabbs;

    auto player = Minecraft::ClientInstance->ThePlayer();

    std::vector<Actor*> actors = Antibot::GetActors();

    for (Actor *actor: actors) {

        if (!addLocalPlayer && actor == player) continue;
        if (!actor->isAlive()) continue;

        Vector3 pos = *actor->GetRenderPositionComponent() - Vector3(0.f, 1.62f, 0.f);
        if (!actor->isPlayer())
        {
            pos = *actor->GetRenderPositionComponent();
        }

        if (actor == player){
            Vector3 pos2 = actor->GetStateVectorComponent()->pos - Vector3(0.f, 1.62f, 0.f);
            Vector3 posOld = actor->GetStateVectorComponent()->posOld - Vector3(0.f, 1.62f, 0.f);
            if (!actor->isPlayer())
            {
                pos2 = actor->GetStateVectorComponent()->pos;
                posOld = actor->GetStateVectorComponent()->posOld;
            }

            pos = posOld + (pos2 - posOld) * Minecraft::ClientInstance->GetDeltaTime();
        }

        float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
        float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
        // since the pos is in the center of the hitbox
        Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
        Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

        // Slightly increase the size of the AABB so it matches the actual hitbox
        aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
        aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);


        AABB aabb = AABB(aabbMin, aabbMax, true);
        aabbs.insert(std::pair<Actor*,AABB>(actor,aabb));
    }

    return aabbs;
}

// Returns rotations in degrees (pitch, yaw)
Vector2 Misc::GetRotations(Vector3* playerEyePos, Vector3* targetPos) {
    Vector3 delta = *targetPos - *playerEyePos;
    float yaw = atan2(delta.z, delta.x) * 180.0f / IM_PI;
    float pitch = atan2(delta.y, sqrt(delta.x * delta.x + delta.z * delta.z)) * 180.0f / IM_PI;
    return Vector2(-pitch, yaw - 90);
}

Vector2 Misc::GetRotations(Vector3& playerEyePos, AABB& targetAabb)
{
    // Rotate to the closest point on the AABB
    Vector3 targetPos = targetAabb.GetClosestPoint(playerEyePos);

    return GetRotations(&playerEyePos, &targetPos);
}

Vector2 Misc::GetMotion(float playerYaw, float value, bool allowStrafe)
{
    playerYaw += GetRotationKeyOffset(allowStrafe) + 90.f;

    float calcYaw = (playerYaw) * ((float) IM_PI / 180.0f);
    Vector2 movVec = { 0.f, 0.f};

    if (Misc::IsUsingMovementKeys()) {
        movVec.x = (float) cos(calcYaw) * ((float) value) / 10.0f;
        movVec.y = (float) sin(calcYaw) * ((float) value) / 10.0f;
    }
    return movVec;
}

float Misc::GetRotationKeyOffset(bool allowStrafe){
    float yawOffset = 0;
    /*bool w = KeyManager::Keys['W'];
    bool a = KeyManager::Keys['A'];
    bool s = KeyManager::Keys['S'];
    bool d = KeyManager::Keys['D'];*/
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return 0;
    auto moveInput = player->GetMoveInputComponent();
    bool w = moveInput->forward;
    bool a = moveInput->left;
    bool s = moveInput->backward;
    bool d = moveInput->right;

    if (w && a && allowStrafe)
        yawOffset = -45;
    else if (w && d && allowStrafe)
        yawOffset = 45;
    else if (s && a && allowStrafe)
        yawOffset = -135;
    else if (s && d && allowStrafe)
        yawOffset = 135;
    else if (w)
        yawOffset = 0;
    else if (a && allowStrafe)
        yawOffset = -90;
    else if (s)
        yawOffset = -180;
    else if (d && allowStrafe)
        yawOffset = 90;
    else
        yawOffset = 0;

    return yawOffset;
}

bool Misc::IsUsingMovementKeys(){
    //return KeyManager::Keys['W'] || KeyManager::Keys['A'] || KeyManager::Keys['S'] || KeyManager::Keys['D'];
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    auto moveInput = player->GetMoveInputComponent();
    return moveInput->forward || moveInput->left || moveInput->backward || moveInput->right;
}

bool Misc::IsUsingMovementKeys(bool W, bool A, bool S, bool D, bool SPACE) {
    //return (W && KeyManager::Keys['W']) || (A && KeyManager::Keys['A']) || (S && KeyManager::Keys['S']) || (D && KeyManager::Keys['D']) || (SPACE && KeyManager::Keys[VK_SPACE]);
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    auto moveInput = player->GetMoveInputComponent();
    return (W && moveInput->forward) || (A && moveInput->left) || (S && moveInput->backward) || (D && moveInput->right) || (SPACE && moveInput->isJumping);
}

int Misc::GetBlockPlaceFace(BlockPos pos) {
    if (!Minecraft::ClientInstance->ThePlayer()) return -1;
    if (!Minecraft::ClientInstance->ThePlayer()->level) return -1;

    if (!IsAirBlock(pos + BlockPos(0, -1, 0))) return 1;
    if (!IsAirBlock(pos + BlockPos(0, 0, 1))) return 2;
    if (!IsAirBlock(pos + BlockPos(0, 0, -1))) return 3;
    if (!IsAirBlock(pos + BlockPos(1, 0, 0))) return 4;
    if (!IsAirBlock(pos + BlockPos(-1, 0, 0))) return 5;
    if (!IsAirBlock(pos + BlockPos(0, 1, 0))) return 0;

    return -1;
}

BlockPos Misc::GetClosestPlacePos(BlockPos block, float maxDistance, bool yLocked, float lockedY) {
    if (IsValidPlacePos(block)) return block;

    for (int i = 2; i < 6; i++) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        BlockPos blockSel = block + GetSide(i);
        if (IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) return blockSel;
    }



    Vector3 playerPos = *Minecraft::ClientInstance->ThePlayer()->getPos() - Vector3(0.f, 2.62f, 0.f);
    playerPos.x = floorf(playerPos.x);
    playerPos.y = floorf(playerPos.y);
    playerPos.z = floorf(playerPos.z);

    Vector3 closestBlock = Vector3(INT_MAX, INT_MAX, INT_MAX);
    float closestDistance = FLT_MAX;

    for (int x = block.x - maxDistance; x < block.x + maxDistance; x++) {
        for (int y = block.y - maxDistance; y < block.y + maxDistance; y++) {
            if (yLocked && y != lockedY) continue;
            for (int z = block.z - maxDistance; z < block.z + maxDistance; z++) {
                Vector3 blockSel = Vector3(x, y, z);
                if (Misc::IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) {
                    float distance = blockSel.Distance(playerPos);
                    if (distance < closestDistance) {
                        closestDistance = distance;
                        closestBlock = blockSel;
                    }
                }
            }
        }
    }

    if (closestBlock.x != INT_MAX && closestBlock.y != INT_MAX && closestBlock.z != INT_MAX) {
        return BlockPos(closestBlock.x, closestBlock.y, closestBlock.z);
    }

    for (int i = 0; i < 1; i++) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        BlockPos blockSel = block + GetSide(i);
        if (IsValidPlacePos(blockSel)) return blockSel;
    }

    return BlockPos(INT_MAX, INT_MAX, INT_MAX);
}


BlockPos Misc::GetClosestPlacePos2(BlockPos block, float maxDistance, bool yLocked, float lockedY) {
    if (IsValidPlacePos(block)) return block;

    for (int i = 2; i < 6; i++) {
        auto player = Minecraft::ClientInstance->ThePlayer();
        BlockPos blockSel = block + GetSide(i);
        if (IsValidPlacePos(blockSel) && Misc::IsAirBlock(blockSel)) return blockSel;
    }

    return BlockPos(INT_MAX, INT_MAX, INT_MAX);
}

bool Misc::IsValidPlacePos(BlockPos pos) {
    return GetBlockPlaceFace(pos) != -1;
}


BlockPos Misc::GetSide(int side) {
    switch (side)
    {
        case 0:
            return {0, 1, 0};
        case 1:
            return {0, -1, 0};
        case 2:
            return {0, 0, 1};
        case 3:
            return {0, 0, -1};
        case 4:
            return {1, 0, 0};
        case 5:
            return {-1, 0, 0};
        default:
            return {0, 0, 0};
    }
}

bool Misc::IsAirBlock(BlockPos pos) {
    if (!Minecraft::ClientInstance->ThePlayer()) return false;
    if (!Minecraft::ClientInstance->ThePlayer()->level) return false;
    Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(pos);

    return block->blockLegacy->IsAir();
}

int64_t Misc::GetCurrentMs() {
    return std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now().time_since_epoch()).count();
}

void Misc::SetClipboard(std::string str) {
    OpenClipboard(nullptr);
    EmptyClipboard();
    HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, str.size() + 1);
    if (!hg) {
        CloseClipboard();
        return;
    }
    memcpy(GlobalLock(hg), str.c_str(), str.size() + 1);
    GlobalUnlock(hg);
    SetClipboardData(CF_TEXT, hg);
    CloseClipboard();
    GlobalFree(hg);
}

void Misc::PlaySoundFromUrl(const std::string& url, float volume, bool loop) {
    FileSystem::CreateDirectoryP(FileSystem::AssetDirectory);

    //dw about this
    audioManager.BasePath = FileSystem::AssetDirectory;

    // Get the file name from the url
    std::string fileName = url.substr(url.find_last_of('/') + 1);
    std::string filePath = FileSystem::AssetDirectory + "\\" + fileName;

    // Download the file if it doesn't exist
    if (!FileSystem::FileExists(filePath)) {
        Logger::Write("PlaySound", "Downloading sound from url " + url + " to " + filePath, Logger::LogType::Debug);
        auto file = Internet::DownloadFile(url, filePath);
        if (!file) {
            Logger::Write("PlaySound", "Failed to download sound with name " + fileName, Logger::LogType::Error);
            return;
        }
    }

    //Logger::Write("PlaySound", "Playing sound with name " + filePath, Logger::LogType::Debug);
    audioManager.Play(fileName, volume, loop);
}

Vector3 Misc::GetFastestBreakableSide(Vector3 block){

    if (!Minecraft::ClientInstance->ThePlayer()) return Vector3(INT_MAX, INT_MAX, INT_MAX);
    auto player = Minecraft::ClientInstance->ThePlayer();
    player->getsupplies()->GetBestPickaxe();
    int pickSlot = player->getsupplies()->GetBestPickaxe();
    if (pickSlot == -1) return Vector3(INT_MAX, INT_MAX, INT_MAX);

    ItemStack* pickItem = player->getsupplies()->inventory->getItem(pickSlot);
    if (!pickItem) return Vector3(INT_MAX, INT_MAX, INT_MAX);

    float bestSpeed = 0;
    Vector3 bestBlock = Vector3(INT_MAX, INT_MAX, INT_MAX);

    for (int i = 0; i < 5; i++) {
        Vector3 blockSel = block + GetSide(i);
        Block* blocki = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockSel);
        int bestTool = Misc::GetBestBreakingTool(blocki);
        if (bestTool == -1) continue;
        float destroySpeed = Misc::GetDestroySpeed(bestTool, blocki, 1.0f);
        if (destroySpeed == 1.f) return blockSel;

        if (destroySpeed > bestSpeed) {
            bestSpeed = destroySpeed;
            bestBlock = blockSel;
        }
    }

    return bestBlock;
}

int Misc::GetBestBreakingTool(Block* block){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;

    int bestSlot = -1;
    float bestSpeed = 0;

    for (int i = 0; i < 36; i++)
    {
        auto itemStack = player->getsupplies()->inventory->getItem(i);
        if (!itemStack) continue;
        if (!itemStack->item) continue;

        float speed = Misc::GetDestroySpeed(i, block, 1.0f);

        if (speed > bestSpeed)
        {
            bestSpeed = speed;
            bestSlot = i;
        }
    }

    return bestSlot;
}

float Misc::GetDestroySpeed(int slot, Block *block, float destroySpeedDivisor = 1.0f) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;

    int currentSlot = player->getsupplies()->selectedHotbarSlot;
    player->SelectSlot(slot);
    bool onGround = player->isOnGround();
    player->setOnGround(true);
    float destroySpeed = player->gamemode->getDestroyRate(*block);
    player->setOnGround(onGround);

    player->SelectSlot(currentSlot);

    return destroySpeed / destroySpeedDivisor;
}

/*
float Misc::GetDestroySpeed(int slot, Block *block, float destroySpeedDivisor = 1.0f) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;

    // Get the item in the slot
    auto itemStack = player->getsupplies()->inventory->getItem(slot);
    if (!itemStack) return -1;

    auto item = *itemStack->item;
    if (!item) return -1;

    // Get the destroy speed of the item
    int itemId = item->id;
    int blockId = block->blockLegacy->id;

    static std::map<std::pair<int, int>, float> speedCache = std::map<std::pair<int, int>, float>();

    float destroySpeed = 0.01f;

    if (speedCache.find(std::pair(itemId, blockId)) != speedCache.end()) {
        destroySpeed = speedCache[std::pair(itemId, blockId)];
    } else {
        int currentSlot = player->getsupplies()->selectedHotbarSlot;
        player->SelectSlot(slot);
        bool onGround = player->isOnGround();
        player->setOnGround(true);
        destroySpeed = player->gamemode->getDestroyRate(*block);
        player->setOnGround(onGround);
        player->SelectSlot(currentSlot);
        speedCache[std::pair(itemId, blockId)] = destroySpeed;
    }

    return destroySpeed / destroySpeedDivisor;
}
 */

void Misc::SwitchAndEquip(int slot, bool switchBack) {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!player) return;
    int currentSlot = player->getsupplies()->selectedHotbarSlot;
    player->SelectSlot(slot);
    // Get itemstack at slot
    auto itemStack = player->getsupplies()->inventory->getItem(slot);
    player->gamemode->baseUseItem(itemStack);
    if (switchBack) player->SelectSlot(currentSlot);
}

bool Misc::SpoofSwitch(int slot){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    auto itemStack = player->supplies->inventory->getItem(slot);
    if (!itemStack)
    {
        Logger::Write("SpoofSwitch", "Itemstack is null", Logger::LogType::Debug);
        return false;
    }


    auto mep = MinecraftPackets::CreatePacket<MobEquipmentPacket>();
    MobEquipmentPacket::Create((mep.get()), player->getRuntimeIDComponent()->runtimeID, *itemStack, slot, slot);
    Minecraft::ClientInstance->packetSender->send(mep.get());

    return true;
}

long long Misc::ID = 1122334880605417613; // Id of user who is allowed to use this version of Prax

bool Misc::CheckForHexEdit() {
    return false;
}

bool isOverVoidi(Vector3 pos) {
    auto* player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;
    auto* blockSource = Minecraft::ClientInstance->GetBlockSource();
    if (!blockSource) return false;

    Vector3 playerPos = pos;

    int minWorldY = -64;

    // For each y below the player pos to the min world y, check if the block IsAir
    for (int y = playerPos.y; y >= minWorldY; y--) {
        Vector3 blockPos = Vector3(playerPos.x, (float)y, playerPos.z);
        Block *block = blockSource->getBlock(blockPos);
        if (!block) continue;
        if (!block->blockLegacy->IsAir()) return false;
    }

    return true;
}

bool Misc::IsOverVoid(Vector3 pos, bool check3x3) {
    bool result = isOverVoidi(pos);

    if (check3x3) {
        // all the returns from isOverVoidi must be true for the 3x3 to be true
        if (!result) return false;

        // check 3x3
        Vector3 playerPos = pos;
        std::vector<Vector3> positions = {
            Vector3(playerPos.x + 1, playerPos.y, playerPos.z),
            Vector3(playerPos.x - 1, playerPos.y, playerPos.z),
            Vector3(playerPos.x, playerPos.y, playerPos.z + 1),
            Vector3(playerPos.x, playerPos.y, playerPos.z - 1),
            Vector3(playerPos.x + 1, playerPos.y, playerPos.z + 1),
            Vector3(playerPos.x + 1, playerPos.y, playerPos.z - 1),
            Vector3(playerPos.x - 1, playerPos.y, playerPos.z + 1),
            Vector3(playerPos.x - 1, playerPos.y, playerPos.z - 1)
        };

        for (auto& pos : positions) {
            if (!isOverVoidi(pos)) return false;
        }

    } else {
        return result;
    }

    return true;
}
// side and offset map
//std::map<int, BlockPos> Sides = {
//    {0, BlockPos(0, 1, 0)},
//    {1, BlockPos(0, -1, 0)},
//    {2, BlockPos(0, 0, 1)},
//    {3, BlockPos(0, 0, -1)},
//    {4, BlockPos(1, 0, 0)},
//    {5, BlockPos(-1, 0, 0)}
//};

int Misc::GetExposedBlockFace(BlockPos bp){
    if (Misc::IsAirBlock(bp + BlockPos(0, -1, 0))) return 1;
    if (Misc::IsAirBlock(bp + BlockPos(0, 0, 1))) return 2;
    if (Misc::IsAirBlock(bp + BlockPos(0, 0, -1))) return 3;
    if (Misc::IsAirBlock(bp + BlockPos(1, 0, 0))) return 4;
    if (Misc::IsAirBlock(bp + BlockPos(-1, 0, 0))) return 5;
    if (Misc::IsAirBlock(bp + BlockPos(0, 1, 0))) return 0;

    return -1;
}

int Misc::CountExposedSides(BlockPos bp){
    int count = 0;
    for (int i = 0; i < 6; i++){
        if (Misc::IsAirBlock(bp + GetSide(i))) count++;
    }
    return count;
}

Vector3 Misc::GetClosestExposedRedstone(float maxDistance) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    Vector3* pos = player->getPos();
    std::vector<Vector3> blocks = std::vector<Vector3>();

    for (int x = (float)pos->x - maxDistance; x < pos->x + maxDistance; x++){
        for (int y = (float)pos->y - maxDistance; y < pos->y + maxDistance; y++){
            for (int z = (float)pos->z - maxDistance; z < pos->z + maxDistance; z++){
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(Vector3(x, y, z));

                if (block->blockLegacy->id == 73  || block->blockLegacy->id == 74) {
                    if (Misc::GetExposedBlockFace(Vector3(x, y, z)) == -1) continue;
                    Vector3 blockPos = Vector3(x, y, z);

                    blocks.emplace_back(*blockPos);
                }
            }
        }
    }

    // Sort the blocks by distance from the player
    std::sort(blocks.begin(), blocks.end(), [pos](Vector3 a, Vector3 b) {
        return a.Distance(*pos) < b.Distance(*pos);
    });

    if (blocks.size() == 0) return Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

    return blocks[0];
}


Vector3 Misc::GetClosestExposedLitRedstone(float maxDistance) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    Vector3* pos = player->getPos();
    std::vector<Vector3> blocks = std::vector<Vector3>();

    for (int x = (float)pos->x - maxDistance; x < pos->x + maxDistance; x++){
        for (int y = (float)pos->y - maxDistance; y < pos->y + maxDistance; y++){
            for (int z = (float)pos->z - maxDistance; z < pos->z + maxDistance; z++){
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(Vector3(x, y, z));

                if (block->blockLegacy->id == 74) {
                    if (Misc::GetExposedBlockFace(Vector3(x, y, z)) == -1) continue;
                    Vector3 blockPos = Vector3(x, y, z);

                    blocks.emplace_back(*blockPos);
                }
            }
        }
    }

    // Sort the blocks by distance from the player
    std::sort(blocks.begin(), blocks.end(), [pos](Vector3 a, Vector3 b) {
        return a.Distance(*pos) < b.Distance(*pos);
    });

    if (blocks.size() == 0) return Vector3(FLT_MAX, FLT_MAX, FLT_MAX);

    return blocks[0];
}

int64_t Misc::GetBlockBreakTime(float destroySpeed)
{
    int64_t now = Misc::GetCurrentMs();

    // For each tick we will add 50ms to the time
    int64_t time = now;
    float progress = 0.0f;

    if (destroySpeed == 0.0f) {
        Logger::Write("Utils", "WARNING: destroySpeed is 0.0f", Logger::LogType::Warning);
        return time;
    }

    while (true) {
        progress += destroySpeed;
        if (progress >= 1.0f) {
            return time;
        }

        // make sure more than 50ms hasnt passed
        if (Misc::GetCurrentMs() - time >= 50) {
            Logger::Write("Utils", "HELP HELP HELP SOME SHIT WENT WRONG FUCK!!!", Logger::LogType::Error);
            return time;
        }

        time += 50;
    }
}

bool Misc::IsOreBlock(Block* block)
{
    if (!block) return false;
    if (!block->blockLegacy) return false;
    if (!block->blockLegacy->material) return false;

    std::string material = block->blockLegacy->name;

    return String::ContainsIgnoreCase(material, "_ore");
}

std::map<int, BlockPosPair> Misc::GetSurroundingBlocks(Vector3 pos, int doubleThick)
{
    std::vector<std::pair<int, BlockPosPair>> blocks;

    if (doubleThick != 2) {
        for (int i = 0; i < 6; i++) {
            Vector3 blockPos = pos + GetSide(i);
            Block *block = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos);
            blocks.emplace_back(std::pair(i, BlockPosPair(blockPos, block)));
        }
    }

    bool hasDoubleThickBlocks = false;

    if (doubleThick != 0)
    {
        /*for (int i = 0; i < 6; i++)
        {
            Vector3 blockPos = pos + GetSide(i) * 2;
            Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos);
            if (block->blockLegacy->id == 0) continue;
            if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) continue;
            hasDoubleThickBlocks = true;
            blocks.emplace_back(i + 6, BlockPosPair(blockPos, block));
        }

        if (doubleThick == 2) {
            for (int i = 0; i < 6; i++)
            {
                Vector3 blockPos = pos + GetSide(i) * 3;
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos);
                if (block->blockLegacy->id == 0) continue;
                if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) continue;
                blocks.emplace_back(i + 12, BlockPosPair(blockPos, block));
            }
        }*/


        for (int i = 0; i < doubleThick; i++)
        {
            for (int j = 0; j < 6; j++)
            {
                Vector3 blockPos = pos + GetSide(j) * (i + 1);
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(blockPos);
                if (block->blockLegacy->id == 0) continue;
                if (block->blockLegacy->id == 73 || block->blockLegacy->id == 74) continue;
                hasDoubleThickBlocks = true;
                blocks.emplace_back(i * 6 + j + 6, BlockPosPair(blockPos, block));
            }
        }
    }


    if (hasDoubleThickBlocks)
    {
        // Sort the blocks by key from least to greatest
        std::sort(blocks.begin(), blocks.end(), [](std::pair<int, BlockPosPair> a, std::pair<int, BlockPosPair> b) {
            return a.first < b.first;
        });
    } else {
        // Sort the blocks by distance from the player
        std::sort(blocks.begin(), blocks.end(), [pos](std::pair<int, BlockPosPair> a, std::pair<int, BlockPosPair> b) {
            return a.second.blockPos.Distance(pos) < b.second.blockPos.Distance(pos);
        });
    }

    std::map<int, BlockPosPair> blocksMap = std::map<int, BlockPosPair>();
    for (auto& block : blocks)
    {
        blocksMap.insert(block);
    }

    return blocksMap;
}

bool Misc::IsBlockNear(Vector3 position, float distance)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return false;

    // Get all the blocks within the distance
    std::vector<BlockPosPair> blocks = Misc::GetBlocksWithinDistance2(&position, distance, false, false);
    for (auto& block : blocks)
    {
        if (block.block->blockLegacy->id != 0) return true;
    }
    return false;

};

std::map <int, Vector3> Misc::SideMap = {
        {0, Vector3(0.f, 0.5f, 0.f)},
        {1, Vector3(0.f, -0.5f, 0.f)},
        {2, Vector3(0.f, 0.f, 0.5f)},
        {3, Vector3(0.f, 0.f, -0.5f)},
        {4, Vector3(0.5f, 0.f, 0.f)},
        {5, Vector3(-0.5f, 0.f, 0.f)}
};

bool Misc::PlaceBlock(Vector3 vecPos, int side, bool swing, bool calcSide, bool rayTraceResult, int slot, int raycastMode) {
    // - RaycastMode
    // 0 - CenterSide
    // 1 - CenterBlock
    auto player = Minecraft::ClientInstance->ThePlayer();
    BlockPos bp = BlockPos((int)vecPos.x, (int)vecPos.y, (int)vecPos.z);
    if (calcSide) side = Misc::GetBlockPlaceFace(bp);

    Vector3 lolVec = bp;

    // offset the lolVec based on the side and the mode
    if (raycastMode == 0) {
        lolVec += SideMap[side];
    } else if (raycastMode == 1) {
        lolVec += Vector3(0.5f, 0.5f, 0.5f);
    }

    HitResult *res = player->level->getRayTraceResult();
    HitResult resOld = *res;

    if (rayTraceResult) {
        lolVec += SideMap[side];

        res->blockPos = lolVec;
        res->facing = side;

        res->type = RaycastTypes::BLOCK;
        res->indirectHit = false;
        res->rayDir = lolVec;
        res->Pos = bp;
    }




    if (swing) player->swing();

    bool oldSwing = player->isSwinging;
    int oldSwimgTime = player->swingProgress;
    int oldSlot = player->supplies->selectedHotbarSlot;
    if (slot != -1) player->SelectSlot(slot);
    player->gamemode->buildBlock(bp + GetSide(side), side, true);

    if (slot != -1) player->SelectSlot(oldSlot);
    if (!oldSwing && !swing) player->isSwinging = false;
    if (oldSwing) player->swingProgress = oldSwimgTime;

    if (rayTraceResult) {
        lolVec += SideMap[side];

        res->blockPos = lolVec;
        res->facing = side;

        res->type = RaycastTypes::BLOCK;
        res->indirectHit = false;
        res->rayDir = lolVec;
        res->Pos = bp;
    }

    return true;
}

uint32_t  Misc::Hashfnv1a32(const char* apStr) {
    uint32_t hash = 2166136261U; // 32 bit offset_basis = 2166136261U

    for (uint32_t idx = 0; apStr[idx] != 0; ++idx) {
        // 32 bit FNV_prime = 224 + 28 + 0x93 = 16777619
        hash = 16777619U * (hash ^ static_cast<unsigned char>(apStr[idx]));
    }

    return hash;
}

int Misc::GetItemSlot(const std::string& name, bool hotbarOnly) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;
    auto supplies = player->supplies;

    int searchDepth = hotbarOnly ? 9 : 36;

    for (int i = 0; i < searchDepth; i++)
    {
        ItemStack* item = supplies->GetItemAtSlot(i);
        if (!item || !item->item)
            continue;
        Item* itemPtr = *item->item;

        if (String::ContainsIgnoreCase(itemPtr->GetName(), name))
        {
            return i;
        }
    }

    return -1;
}

int Misc::GetPlaceableItemOnBlock(Vector3 blockPos, bool hotbarOnly = true, bool prioritizeHighest = true)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return -1;
    auto supplies = player->supplies;

    int slot = -1;
    // slot, count
    std::map<int, int> items = std::map<int, int>();
    for (int i = 0; i < 36; i++)
    {
        ItemStack* item = supplies->GetItemAtSlot(i);
        if (!item || !item->item)
            continue;
        if (hotbarOnly && i > 8)
            continue;
        Item* itemPtr = *item->item;

        if (item->blockPtr)
        {

            if (String::ContainsIgnoreCase(itemPtr->GetName(), "netherreactor") ||
                String::ContainsIgnoreCase(itemPtr->GetName(), "boombox")) continue;
            if (String::ContainsIgnoreCase(itemPtr->GetName(), "button") ||
                String::ContainsIgnoreCase(itemPtr->GetName(), "torch")) continue;
            if (String::ContainsIgnoreCase(itemPtr->GetName(), "fence")) continue;


            Block* heldBlock = item->blockPtr;
            if (!heldBlock->blockLegacy->mayPlace(*Minecraft::ClientInstance->GetBlockSource(), blockPos)) continue;
            if (!heldBlock->blockLegacy->mayPlaceOn(*Minecraft::ClientInstance->GetBlockSource(), blockPos)) continue;
            // Check some material stuff to see if it's placeable

            if (!prioritizeHighest)
            {
                slot = i;
                break;
            }

            items.emplace(i, item->amount);
        }
    }

    if (prioritizeHighest)
    {
        int highest = 0;
        for (const auto& item : items)
        {
            if (item.second > highest)
            {
                highest = item.second;
                slot = item.first;
            }
        }
    }

    return slot;
}

AABB Misc::GetActorAABB(Actor* actor, bool inc) {
    Vector3 pos = actor->GetStateVectorComponent()->pos - Vector3(0.f, 1.62f, 0.f);
    Vector3 posOld = actor->GetStateVectorComponent()->posOld - Vector3(0.f, 1.62f, 0.f);
    if (!actor->isPlayer())
    {
        pos = actor->GetStateVectorComponent()->pos;
        posOld = actor->GetStateVectorComponent()->posOld;
    }

    // Interpolate the position so it's smoother
    pos = posOld + (pos - posOld) * Minecraft::ClientInstance->GetDeltaTime();

    float hitboxWidth = actor->GetAABBShapeComponent()->hitboxWidth;
    float hitboxHeight = actor->GetAABBShapeComponent()->hitboxHeight;
    // since the pos is in the center of the hitbox
    Vector3 aabbMin = Vector3(pos.x - hitboxWidth / 2, pos.y, pos.z - hitboxWidth / 2);
    Vector3 aabbMax = Vector3(pos.x + hitboxWidth / 2, pos.y + hitboxHeight, pos.z + hitboxWidth / 2);

    // Slightly increase the size of the AABB so it matches the actual hitbox
    if (inc)
    {
        aabbMin = aabbMin - Vector3(0.1f, 0.1f, 0.1f);
        aabbMax = aabbMax + Vector3(0.1f, 0.1f, 0.1f);
    }

    AABB aabb = AABB(aabbMin, aabbMax, true);

    return aabb;
}

Vector3 Misc::GetClosestAabbPos(Actor* actor, Vector3* playerEyePos)
{
    AABB aabb = GetActorAABB(actor);
    Vector3 aabbMin = aabb.lower;
    Vector3 aabbMax = aabb.upper;

    return {
            Math::Clamp(playerEyePos->x, aabbMin.x, aabbMax.x),
            Math::Clamp(playerEyePos->y, aabbMin.y, aabbMax.y),
            Math::Clamp(playerEyePos->z, aabbMin.z, aabbMax.z)
    };

}

Vector3 Misc::GetLerpedActorPos(Actor* actor)
{
    if (actor == nullptr) return Vector3(0.f, 0.f, 0.f);
    if (actor->GetStateVectorComponent() == nullptr) return Vector3(0.f, 0.f, 0.f);


    Vector3 pos = actor->GetStateVectorComponent()->pos - Vector3(0.f, 1.62f, 0.f);
    Vector3 posOld = actor->GetStateVectorComponent()->posOld - Vector3(0.f, 1.62f, 0.f);
    if (!actor->isPlayer())
    {
        pos = actor->GetStateVectorComponent()->pos;
        posOld = actor->GetStateVectorComponent()->posOld;
    }

    // Interpolate the position so it's smoother
    pos = posOld + (pos - posOld) * Minecraft::ClientInstance->GetDeltaTime();

    return pos;
}

// Gets the closest block distance from the player
float Misc::GetClosestBlockDistance() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return FLT_MAX;

    Vector3 playerPos = GetLerpedActorPos(player);
    float closestDistance = FLT_MAX;

    for (int x = playerPos.x - 5; x < playerPos.x + 5; x++) {
        for (int y = playerPos.y - 5; y < playerPos.y + 5; y++) {
            for (int z = playerPos.z - 5; z < playerPos.z + 5; z++) {
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(Vector3(x, y, z));
                if (Misc::IsAirBlock(Vector3(x, y, z))) continue;
                float distance = playerPos.Distance(Vector3(x, y, z));
                if (distance < closestDistance) closestDistance = distance;
            }
        }
    }

    return closestDistance;
}

std::string Misc::GetModuleOfAddress(uintptr_t address) {

    MEMORY_BASIC_INFORMATION mbi;
    if (VirtualQuery((void *) address, &mbi, sizeof(mbi))) {
        char moduleName[MAX_PATH];
        if (GetModuleFileNameA((HMODULE) mbi.AllocationBase, moduleName, sizeof(moduleName))) {
            // If it is in a module, return the offset from the base
            uintptr_t moduleBase = (uintptr_t) mbi.AllocationBase;

            std::string modName = moduleName;
            std::string result = modName + "+" + String::ToHex((address - moduleBase));
            // Remove the path from the module name
            return result.substr(result.find_last_of("\\") + 1);
        }
    }
    return std::string(String::ToHex(address));

}

std::string Misc::GetNameFromActor(Actor *actor) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return "";
    if(!actor) return "";

    std::string nameTagString = actor->_getNameTag();

    for (auto& pair : player->level->playerList) {
        const std::string& name = pair.second.name;

        if (String::ContainsIgnoreCase(nameTagString, name)){
            return name;
            break;
        }
    }

    return nameTagString;
}

Actor *Misc::GetActorFromRuntimeID(int64_t id) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return nullptr;

    for (auto &&[_, moduleOwner, type, ridc]: player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each())
        if (ridc.runtimeID == id) {
            return moduleOwner.actor;
        }


    return nullptr;
}

std::vector<Vector3> Misc::GetBlocksWithinDistance(Vector3* pos, float maxDistance) {
    // Use a key-value pair to store the block and its distance from the player
    std::vector<Vector3> blocks = std::vector<Vector3>();

    for (int x = (float) pos->x - maxDistance; x < pos->x + maxDistance; x++) {
        for (int y = (float) pos->y - maxDistance; y < pos->y + maxDistance; y++) {
            for (int z = (float) pos->z - maxDistance; z < pos->z + maxDistance; z++) {
                Block *block = Minecraft::ClientInstance->GetBlockSource()->getBlock(Vector3(x, y, z));

                if (block->blockLegacy->id == 0) continue;

                if (Misc::GetExposedBlockFace(Vector3(x, y, z)) == -1) continue;

                blocks.push_back(Vector3(x, y, z));
            }
        }
    }

    return blocks;
}

std::vector<BlockPosPair> Misc::GetBlocksWithinDistance2(Vector3* pos, float maxDistance, bool exposedOnly, bool sort)
{
    // Use a key-value pair to store the block and its distance from the player
    std::vector<BlockPosPair> blocks = std::vector<BlockPosPair>();

    for (int x = pos->x - maxDistance; x < pos->x + maxDistance; x++) {
        for (int y = pos->y - maxDistance; y < pos->y + maxDistance; y++) {
            for (int z = pos->z - maxDistance; z < pos->z + maxDistance; z++) {
                Vector3 pos = Vector3(x, y, z);
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(x, y, z);

                if (block->blockLegacy->id == 0) continue;

                if (exposedOnly && Misc::GetExposedBlockFace(pos) != -1) continue;

                blocks.emplace_back(pos, block);
            }
        }
    }

    // Sort the blocks by distance from the player
    if (sort)
        ranges::sort(blocks, [pos](BlockPosPair a, BlockPosPair b) {
            return a.blockPos.Distance(*pos) < b.blockPos.Distance(*pos);
        });

    return blocks;
}

std::vector<BlockPosIdPair> Misc::GetBlocksWithinDistance3(Vector3* pos, float maxDistance, bool exposedOnly, bool sort)
{
    // Use a key-value pair to store the block and its distance from the player
    std::vector<BlockPosIdPair> blocks = std::vector<BlockPosIdPair>();

    for (int x = pos->x - maxDistance; x < pos->x + maxDistance; x++) {
        for (int y = pos->y - maxDistance; y < pos->y + maxDistance; y++) {
            for (int z = pos->z - maxDistance; z < pos->z + maxDistance; z++) {
                Vector3 pos = Vector3(x, y, z);
                Block* block = Minecraft::ClientInstance->GetBlockSource()->getBlock(x, y, z);

                if (block->blockLegacy->id == 0) continue;

                if (exposedOnly && Misc::GetExposedBlockFace(pos) != -1) continue;

                blocks.emplace_back(pos, block->blockLegacy->id);
            }
        }
    }

    // Sort the blocks by distance from the player
    if (sort)
        ranges::sort(blocks, [pos](BlockPosIdPair a, BlockPosIdPair b) {
            return a.blockPos.Distance(*pos) < b.blockPos.Distance(*pos);
        });

    return blocks;
}


// GetModuleCount: Returns the number of modules loaded in the current process
int Misc::GetModuleCount()
{
    // Create a snapshot of the current process
    HANDLE hSnap = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE, GetCurrentProcessId());
    if (hSnap == INVALID_HANDLE_VALUE) return 0;

    // Get the first module from the snapshot
    MODULEENTRY32 me32;
    me32.dwSize = sizeof(MODULEENTRY32);
    if (!Module32First(hSnap, &me32)) {
        CloseHandle(hSnap);
        return 0;
    }

    // Loop through the modules and count them
    int count = 0;
    do {
        count++;
    } while (Module32Next(hSnap, &me32));

    CloseHandle(hSnap);
    return count;
}
