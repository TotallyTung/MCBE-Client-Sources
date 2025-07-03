//
// Created by vastrakai on 7/12/2024.
//

#include "MidclickAction.hpp"

#include "Nuker.hpp"

#include <Features/FeatureManager.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Modules/Misc/Friends.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Components/ActorOwnerComponent.hpp>
#include <SDK/Minecraft/Actor/Components/RuntimeIDComponent.hpp>
#include <SDK/Minecraft/Inventory/Item.hpp>
#include <SDK/Minecraft/Inventory/ItemStack.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/World/BlockLegacy.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>

void MidclickAction::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &MidclickAction::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &MidclickAction::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
}

void MidclickAction::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &MidclickAction::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &MidclickAction::onPacketOutEvent>(this);
}



void MidclickAction::onBaseTickEvent(BaseTickEvent& event)
{
    if (ClientInstance::get()->getMouseGrabbed()) return;
    auto player = event.mActor;

    int slot = -1;
    for (int i = 0; i < 36; i++)
    {
        auto itemStack = player->getSupplies()->getContainer()->getItem(i);
        if (!itemStack) continue;
        if (!itemStack->mItem) continue;

        auto item = itemStack->getItem();

        if (item->mName == "ender_pearl")
        {
            slot = i;
            break;
        }
    }

    if (mHotbarOnly.mValue && slot > 8) slot = -1;

    if (mThrowNextTick && slot != -1)
    {
        mThrowNextTick = false;
        ItemUtils::useItem(slot);
        spdlog::info("Used ender pearl!");
    }

    static bool lastMidClick = false;
    bool midClick = ImGui::IsMouseDown(2);

    if (midClick && !lastMidClick)
    {
        spdlog::info("Midclick!");
        auto hitResult = player->getLevel()->getHitResult();
        if (hitResult->mType == ENTITY && mAddFriend.mValue)
        {
            auto entity = hitResult->mEntity.id;
            Actor* target = nullptr;
            for (auto&& [id, moduleOwner, type, ridc] : player->mContext.mRegistry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each()) {
                if (type.mType != ActorType::Player) continue;
                if (id == entity) {
                    target = moduleOwner.mActor;
                    break;
                }
            }

            if (target)
            {
                bool isFriend = gFriendManager->isFriend(target);
                if (isFriend)
                {
                    gFriendManager->removeFriend(target);
                }
                else
                {
                    gFriendManager->addFriend(target);
                }

                ChatUtils::displayClientMessage(!isFriend ? "§aAdded " + target->getRawName() + " to your friends list!" : "§cRemoved " + target->getRawName() + " from your friends list!");
            }
        }
        else if (hitResult->mType == BLOCK && mSetNukerBlock) {
            Block* selectedBlock = ClientInstance::get()->getBlockSource()->getBlock(hitResult->mBlockPos);
            if (selectedBlock->mLegacy->getBlockId() != 0) {
                static Nuker* nukerModule = gFeatureManager->mModuleManager->getModule<Nuker>();
                std::string blockName = selectedBlock->mLegacy->getmName();
                nukerModule->specifiedBlockID = blockName;
                ChatUtils::displayClientMessage("§aSet block to " + blockName);
            }
        }
        else if (mThrowPearls.mValue)
        {
            if (slot != -1)
            {
                mThrowNextTick = true;
                mRotateNextTick = true;
            }
            else {
                NotifyUtils::notify("No pearls found!", 4.f, Notification::Type::Warning);
                ClientInstance::get()->playUi("note.bass", 0.75f, 0.5);
            }
        }
    }
    lastMidClick = midClick;
}

void MidclickAction::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput && mRotateNextTick)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        auto player = ClientInstance::get()->getLocalPlayer();
        if (!player) return;

        glm::vec2 realRots = { player->getActorRotationComponent()->mPitch, player->getActorRotationComponent()->mYaw };

        paip->mRot = realRots;
        paip->mYHeadRot = realRots.y;
        mRotateNextTick = false;
    }
}
