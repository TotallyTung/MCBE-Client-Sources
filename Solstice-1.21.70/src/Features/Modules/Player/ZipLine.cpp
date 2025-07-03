//
// 3/01/2025.
//

#include "ZipLine.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>

std::vector<glm::ivec3> ZipLine::getCollidingBlocks(Actor* target)
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

glm::ivec3 ZipLine::getClosestPlacePos(glm::ivec3 pos, float distance, std::vector<glm::ivec3> collidingBlocks)
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

glm::vec3 ZipLine::getRotBasedPos(float extend, float yPos)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    glm::vec2 playerRots = glm::vec2(player->getActorRotationComponent()->mPitch,
        player->getActorRotationComponent()->mYaw + MathUtils::getRotationKeyOffset());

    float correctedYaw = (playerRots.y + 90) * ((float)IM_PI / 180);
    float inFrontX = cos(correctedYaw) * extend;
    float inFrontZ = sin(correctedYaw) * extend;
    float placeX = player->getPos()->x + inFrontX;
    float placeY = yPos;
    float placeZ = player->getPos()->z + inFrontZ;

    // Floor the values
    return { floor(placeX), floor(placeY), floor(placeZ) };
}

glm::vec3 ZipLine::getPlacePos(float extend)
{
    float yPos = mStartY;
    glm::ivec3 blockSel = { INT_MAX, INT_MAX, INT_MAX };

    blockSel = getRotBasedPos(extend, yPos);

    int side = BlockUtils::getBlockPlaceFace(blockSel);

    if (side == -1)
    {
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return { FLT_MAX, FLT_MAX, FLT_MAX };

        if (player->getFallDistance() > 3.f)
        {
            blockSel.y = player->getPos()->y - 3.62f;
        }

        // Find da block
        blockSel = getClosestPlacePos(blockSel, mRange.as<float>(), getCollidingBlocks(player));
        if (blockSel.x == INT_MAX) return { FLT_MAX, FLT_MAX, FLT_MAX };
        side = BlockUtils::getBlockPlaceFace(blockSel);

        if (side == -1) return { FLT_MAX, FLT_MAX, FLT_MAX };
    }

    if (blockSel.x == INT_MAX) return { FLT_MAX, FLT_MAX, FLT_MAX };

    return blockSel;
}

bool ZipLine::tickPlace() {
    glm::vec3 blockPos = getPlacePos(0.f);
    if (!BlockUtils::isAirBlock(blockPos))
    {
        for (float i = 0.f; i < mExtend.mValue; i += 1.f)
        {
            blockPos = getPlacePos(i);
            if (BlockUtils::isAirBlock(blockPos)) break;
        }
    }

    if (!BlockUtils::isValidPlacePos(blockPos) || !BlockUtils::isAirBlock(blockPos)) {
        return false;
    }

    int side = BlockUtils::getBlockPlaceFace(blockPos);
    if (side == -1) {
        return false;
    }
    int blockSlot = ItemUtils::getPlaceableItemOnBlock(blockPos, mHotbarOnly.mValue, false);
    ClientInstance::get()->getLocalPlayer()->getSupplies()->mSelectedSlot = blockSlot;
    BlockUtils::placeBlock(blockPos, side);
    return true;
}

void ZipLine::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &ZipLine::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &ZipLine::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    mLastSlot = player->getSupplies()->mSelectedSlot;
    mStartY = player->getPos()->y + 1.f;
}

void ZipLine::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &ZipLine::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &ZipLine::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    player->getSupplies()->mSelectedSlot = mLastSlot;
}

void ZipLine::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    auto supplies = player->getSupplies();

    if (ItemUtils::getAllPlaceables(mHotbarOnly.mValue) < 1) return;

    auto currentY = player->getPos()->y + 1.f;
    if (!mLockY.mValue) mStartY = currentY;

    for (int i = 0; i < mPlaces.mValue; i++)
    {
        if (!tickPlace()) break;
    }
}

void ZipLine::onPacketOutEvent(PacketOutEvent& event)
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