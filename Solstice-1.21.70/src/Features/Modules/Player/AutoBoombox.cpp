//
// 8/11/2024.
//

#include "AutoBoombox.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Modules/Combat/Aura.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/World/BlockSource.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthinputPacket.hpp>

void AutoBoombox::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AutoBoombox::onBaseTickEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &AutoBoombox::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
}

void AutoBoombox::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AutoBoombox::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &AutoBoombox::onPacketOutEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;
    auto supplies = player->getSupplies();
    if (mSelectedSlot)
    {
        supplies->mSelectedSlot = mPreviousSlot;
        mSelectedSlot = false;
    }
    mShouldRotate = false;
}

void AutoBoombox::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    auto supplies = player->getSupplies();
    if (mLastBoomboxPlace + mDelay.mValue > NOW) {
        if (mSelectedSlot) {
            supplies->mSelectedSlot = mPreviousSlot;
            mSelectedSlot = false;
        }
        return;
    }

    if (!Aura::sHasTarget || !Aura::sTarget->getActorTypeComponent()) return;
    if (!Aura::sTarget->isPlayer()) return;

    int boomboxSlot = ItemUtils::getBoombox(mHotbarOnly.mValue, mMode.mValue == Mode::TNT);
    if (boomboxSlot == -1) return;

    if (mMode.mValue == Mode::Boombox) {
        if (supplies->mSelectedSlot != boomboxSlot) {
            mPreviousSlot = supplies->mSelectedSlot;
            supplies->mSelectedSlot = boomboxSlot;
            mSelectedSlot = true;
            return;
        }
    }

    glm::ivec3 placePos = *Aura::sTarget->getPos();
    int side = 0;
    float maxDist = 0;

    for (auto& offset : mPlaceOffsets) {
        glm::vec3 pos = *Aura::sTarget->getPos() + offset;
        float dist = glm::distance(*player->getPos(), pos);
        if (maxDist < dist) {
            placePos = pos;
            maxDist = dist;
        }
    }

    bool found = false;
    for (int i = 0; i <= 5; i++) {
        glm::ivec3 pos = placePos - glm::ivec3(0, i, 0);
        if (!BlockUtils::isAirBlock(pos + glm::ivec3(0, -1, 0))) {
            placePos = pos;
            found = true;
            break;
        }
    }
    if (!found) return;

    if (mMode.mValue == Mode::TNT) {
        mPreviousSlot = supplies->mSelectedSlot;
        supplies->mSelectedSlot = boomboxSlot;
        mSelectedSlot = true;
    }

    side = BlockUtils::getBlockPlaceFace(placePos);

    mCurrentPlacePos = placePos;
    mShouldRotate = true;
    BlockUtils::placeBlock(placePos, side);
    BlockUtils::clearBlock(placePos);     // Clear the place pos so we dont place on the block again
    mLastBoomboxPlace = NOW;
}

void AutoBoombox::onPacketOutEvent(PacketOutEvent& event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        if (mShouldRotate)
        {
            const glm::vec3 blockPos = mCurrentPlacePos;
            auto blockAABB = AABB(blockPos, glm::vec3(1, 1, 1));
            glm::vec2 rotations = MathUtils::getRots(*player->getPos(), blockAABB);
            paip->mRot = rotations;
            paip->mYHeadRot = rotations.y;
            mShouldRotate = false;
        }
    }
    else if (event.mPacket->getId() == PacketID::InventoryTransaction)
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
            }
        }
    }
}