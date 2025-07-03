//
// 2/21/2025.
//

#include "Surround.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>

std::vector<glm::ivec3> Surround::getCollidingBlocks(Actor* target)
{
    std::vector<glm::ivec3> collidingBlockList;
    auto player = target;
    if (!player) return collidingBlockList;
    AABBShapeComponent* aabb = player->getAABBShapeComponent();
    glm::vec3 playerPos = *player->getPos() - PLAYER_HEIGHT_VEC;
    glm::vec3 lower = playerPos;
    glm::vec3 upper = playerPos;
    float width = (aabb->mWidth / 2) - 0.01f;

    lower.x -= width;
    lower.z -= width;

    upper.x += width;
    upper.y += aabb->mHeight;
    upper.z += width;

    for (int y = floor(lower.y); y <= floor(upper.y); y++)
        for (int x = floor(lower.x); x <= floor(upper.x); x++)
            for (int z = floor(lower.z); z <= floor(upper.z); z++) {
                glm::ivec3 blockPos = { x, y, z };
                collidingBlockList.emplace_back(blockPos);
            }

    return collidingBlockList;
}

std::vector<glm::ivec3> Surround::getPlacePositions(std::vector<glm::ivec3> blockList) {
    std::vector<glm::ivec3> placePositions;
    if (blockList.size() < 1)
        return placePositions;
    glm::ivec3 start = blockList[0];
    glm::ivec3 end = blockList[blockList.size() - 1];

    // ground
    for (int x = start.x; x <= end.x; x++)
        for (int z = start.z; z <= end.z; z++) {
            glm::ivec3 blockPos = { x, start.y - 1, z };
            placePositions.emplace_back(blockPos);
        }

    // wall
    for (int y = start.y; y <= end.y; y++) {

        for (int z = start.z; z <= end.z; z++) {
            glm::ivec3 blockPos = { start.x - 1, y, z };
            glm::ivec3 blockPos2 = { end.x + 1, y, z };
            placePositions.emplace_back(blockPos);
            placePositions.emplace_back(blockPos2);
        }

        for (int x = start.x; x <= end.x; x++) {
            glm::ivec3 blockPos = { x, y, start.z - 1 };
            glm::ivec3 blockPos2 = { x, y, end.z + 1 };
            placePositions.emplace_back(blockPos);
            placePositions.emplace_back(blockPos2);
        }

    }

    // ceiling
    for (int x = start.x; x <= end.x; x++)
        for (int z = start.z; z <= end.z; z++) {
            glm::ivec3 blockPos = { x, end.y + 1, z };
            placePositions.emplace_back(blockPos);
        }

    return placePositions;
}

glm::ivec3 Surround::getClosestPlacePos(glm::ivec3 pos, float distance, std::vector<glm::ivec3> collidingBlocks)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return pos;

    auto closestBlock = glm::vec3(INT_MAX, INT_MAX, INT_MAX);
    float closestDist = FLT_MAX;

    for (int x = pos.x - distance; x <= pos.x + distance; x++)
        for (int y = pos.y - distance; y <= pos.y + distance; y++)
            for (int z = pos.z - distance; z <= pos.z + distance; z++)
            {
                auto blockSel = glm::vec3(x, y, z);
                bool include = std::find(collidingBlocks.begin(), collidingBlocks.end(), glm::ivec3(blockSel)) != collidingBlocks.end();
                if (BlockUtils::isValidPlacePos(blockSel) && BlockUtils::isAirBlock(blockSel) && !include) {
                    float distance = glm::distance(glm::vec3(pos), blockSel);
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
        glm::ivec3 blockSel = pos + glm::ivec3(BlockUtils::blockFaceOffsets[i]);
        if (BlockUtils::isValidPlacePos(blockSel)) return blockSel;
    }

    return { INT_MAX, INT_MAX, INT_MAX };
}

void Surround::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Surround::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Surround::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mLastSlot = player->getSupplies()->mSelectedSlot;
}

void Surround::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Surround::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Surround::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->getSupplies()->mSelectedSlot = mLastSlot;
}

void Surround::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    auto supplies = player->getSupplies();

    if (mLastBlockPlaced + mDelay.mValue > NOW)
        return;

    Actor* target = nullptr;
    auto actors = ActorUtils::getActorList(false, true);
    std::ranges::sort(actors, [&](Actor* a, Actor* b) -> bool
        {
            return a->distanceTo(player) < b->distanceTo(player);
        });

    for (auto actor : actors) {
        if (actor == player) continue;
        if (actor->distanceTo(player) > mRange.mValue) continue;
        if (actor == nullptr || !actor->getActorTypeComponent() || !actor->isPlayer()) continue;

        target = actor;
        break;
    }

    if (target == nullptr)
        return;

    if (ItemUtils::getAllPlaceables(mHotbarOnly.mValue) < 1) return;

    std::vector<glm::ivec3> collidingBlocks = getCollidingBlocks(target);

    std::vector<glm::ivec3> placePositions = getPlacePositions(collidingBlocks);

    int spoofSlot = 0;
    bool placed = false;

    for (glm::ivec3 placePos : placePositions) {
        if (BlockUtils::isAirBlock(placePos)) {
            glm::ivec3 blockPos = getClosestPlacePos(placePos, 1, collidingBlocks);
            if (blockPos.x == INT_MAX)
                continue;
            if (BlockUtils::isAirBlock(blockPos)) {
                int blockSlot = ItemUtils::getPlaceableItemOnBlock(placePos, mHotbarOnly.mValue, false);
                if (blockSlot == -1)
                    continue;
                supplies->mSelectedSlot = blockSlot;

                int face = 1;
                glm::ivec3 hitPos = { blockPos.x, blockPos.y - 1, blockPos.z };
                if (BlockUtils::isAirBlock(hitPos)) {
                    face = BlockUtils::getBlockPlaceFace(blockPos);
                }
                BlockUtils::placeBlock(blockPos, face);
                if (mDebug.mValue) {
                    ChatUtils::displayClientMessage("Placed Block:" + std::to_string(blockPos.x) + ", " + std::to_string(blockPos.y) + ", " + std::to_string(blockPos.z));
                }
                mLastBlockPlaced = NOW;
                break;
            }
        }
    }
}

void Surround::onPacketOutEvent(PacketOutEvent& event) 
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::InventoryTransaction)
    {
        if (const auto it = event.getPacket<InventoryTransactionPacket>(); it->mTransaction->type ==
            ComplexInventoryTransaction::Type::ItemUseTransaction)
        {
            const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
            if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
            {
                transac->mClickPos = BlockUtils::clickPosOffsets[transac->mFace];
                for (int i = 0; i < 3; i++)
                {
                    if (transac->mClickPos[i] == 0.5)
                    {
                        transac->mClickPos[i] = MathUtils::randomFloat(-0.49f, 0.49f);
                    }
                }
                glm::ivec3 playerPos = *player->getPos() - PLAYER_HEIGHT_VEC;
                if (transac->mBlockPos.y < playerPos.y) {
                    transac->mClickPos = { MathUtils::randomFloat(-0.49f, 0.49f), 1.0f, MathUtils::randomFloat(-0.49f, 0.49f) };
                }
                transac->mTriggerType = ItemUseInventoryTransaction::TriggerType::PlayerInput;
            }
        }
    }
}