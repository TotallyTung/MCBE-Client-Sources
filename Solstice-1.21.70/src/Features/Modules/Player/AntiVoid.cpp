//
// Created by vastrakai on 9/8/2024.
//

#include "AntiVoid.hpp"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>

#include "Freecam.hpp"

AntiVoid::AntiVoid(): ModuleBase("AntiVoid", "Prevents you from falling into the void", ModuleCategory::Player, 0, false)
{
    addSettings(&mFallDistance, &mTpOnce, &mTeleport, &mToggleFreecam);

    mNames = {
        {Lowercase, "antivoid"},
        {LowercaseSpaced, "anti void"},
        {Normal, "AntiVoid"},
        {NormalSpaced, "Anti Void"}
    };
}

void AntiVoid::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &AntiVoid::onBaseTickEvent>(this);
    auto player = ClientInstance::get()->getLocalPlayer();

    if (!player) return;

}

void AntiVoid::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &AntiVoid::onBaseTickEvent>(this);
}

void AntiVoid::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (player->isOnGround())
    {
        mCanTeleport = true;
        mTeleported = false;

        if (mTeleport.mValue) {
            mOnGroundPositions.push_back(*player->getPos());
            // if we have more than 40 positions, remove the first one
            if (mOnGroundPositions.size() > 40)
            {
                mOnGroundPositions.erase(mOnGroundPositions.begin());
            }
        }
        return;
    }

    bool hasTeleported = mTeleported && mTpOnce.mValue;

    if (player->getFallDistance() > mFallDistance.mValue && (!hasTeleported || !mTpOnce.mValue) && mCanTeleport && BlockUtils::isOverVoid(*player->getPos() - PLAYER_HEIGHT_VEC))
    {
        if (!mTeleport.mValue)
        {
            if (mToggleFreecam.mValue) {
                static auto freecam = gFeatureManager->mModuleManager->getModule<Freecam>();
                if (freecam)
                {
                    freecam->mMode.mValue = Freecam::Mode::Normal;
                    freecam->setEnabled(true);
                }

                player->setFallDistance(0.0f);
                mOnGroundPositions.clear();
                if (mTpOnce.mValue)
                {
                    mCanTeleport = false;
                }
                mTeleported = true;
            }
            return;
        }

        glm::vec3 bestPos = glm::ivec3(0, 0, 0);
        bool found = false;
        auto inverted = mOnGroundPositions;
        std::ranges::reverse(inverted);
        for (auto& pos : inverted)
        {
            auto blockPos = glm::ivec3(pos + glm::vec3(0, -1, 0) - PLAYER_HEIGHT_VEC);
            if (BlockUtils::isGoodBlock(blockPos))
            {
                bestPos = blockPos;
                found = true;
                break;
            }
        }

        if (!found)
        {
            NotifyUtils::notify("Could not find a safe position to teleport to!", 5.0f, Notification::Type::Error);
            return;
        }

        player->setPosition(bestPos + glm::vec3(0, 2 + PLAYER_HEIGHT, 0));
        player->setFallDistance(0.0f);
        NotifyUtils::notify("Teleported!", 5.0f, Notification::Type::Info);
        mTeleported = true;
        if (mTpOnce.mValue) {
            mOnGroundPositions.clear();
            mCanTeleport = false;
        }
    }
}
