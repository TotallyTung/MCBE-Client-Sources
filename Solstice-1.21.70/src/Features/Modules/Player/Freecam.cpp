//
// Created by vastrakai on 7/22/2024.
//

#include "Freecam.hpp"

#include <Features/Events/ActorRenderEvent.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/CameraInstructionPacket.hpp>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/RenderEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Hook/Hooks/NetworkHooks/PacketReceiveHook.hpp>
#include <Hook/Hooks/RenderHooks/ActorRenderDispatcherHook.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/KeyboardMouseSettings.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Inventory/PlayerInventory.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>
#include <SDK/Minecraft/World/HitResult.hpp>
#include <SDK/Minecraft/World/Level.hpp>

void Freecam::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Freecam::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &Freecam::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Freecam::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->listen<RenderEvent, &Freecam::onRenderEvent>(this);



    mLastRot = *player->getActorRotationComponent();
    mLastRot.mOldYaw = mLastRot.mYaw;
    mLastRot.mOldPitch = mLastRot.mPitch;

    mLastHeadRot = *player->getActorHeadRotationComponent();
    mLastHeadRot.mOldHeadRot = mLastHeadRot.mHeadRot;

    mLastBodyRot = *player->getMobBodyRotationComponent();
    mLastBodyRot.yOldBodyRot = mLastBodyRot.yBodyRot;

    mOldAABB = player->getAABB();
    mAABBMin = player->getAABBShapeComponent()->mMin;
    mAABBMax = player->getAABBShapeComponent()->mMax;
    mSvPos = player->getStateVectorComponent()->mPos;
    mSvPosOld = player->getStateVectorComponent()->mPos;
    mOldPos = player->getRenderPositionComponent()->mPosition;
    mLastVelocity = player->getStateVectorComponent()->mVelocity;

    mOldAABB.mMin += mLastVelocity;
    mOldAABB.mMax += mLastVelocity;

    mOrigin = mSvPos;
    mOldOrigin = mSvPosOld;

    mShouldDisable = false;


    auto gock = player->getActorHeadRotationComponent();
    mHeadYaw = { gock->mHeadRot, gock->mOldHeadRot };
}

void Freecam::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Freecam::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &Freecam::onPacketInEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Freecam::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<RenderEvent, &Freecam::onRenderEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mMode.mValue == Mode::Normal && !mShouldDisable)
    {
        player->getAABBShapeComponent()->mMin = mAABBMin;
        player->getAABBShapeComponent()->mMax = mAABBMax;
        player->getStateVectorComponent()->mPos = mSvPos;
        player->getStateVectorComponent()->mPosOld = mSvPosOld;
        player->getStateVectorComponent()->mVelocity = mLastVelocity;
    }
    player->getMoveInputComponent()->reset( false);


    mResetRot = true;
}

void Freecam::onPacketInEvent(PacketInEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto mpp = event.getPacket<MovePlayerPacket>();
        if (mpp->mPlayerID == player->getRuntimeID())
        {
            NotifyUtils::notify("Lagback detected!", 5.f, Notification::Type::Warning);
            spdlog::debug("[Freecam] Listening to lagback, setting position to {}, {}, {}", mpp->mPos.x, mpp->mPos.y, mpp->mPos.z);
            mShouldDisable = true;
            setEnabled(false);
            return;
        }
    }
}

void Freecam::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player || mShouldDisable) {
        return;
    }

    if ((event.mPacket->getId() == PacketID::PlayerAuthInput || event.mPacket->getId() == PacketID::MovePlayer))
        event.mCancelled = true;
}

void Freecam::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = event.mActor;
    if (!player) return;

    if (mShouldDisable) {
        return;
    }

    glm::vec3 motion = glm::vec3(0, 0, 0);

    if (Keyboard::isUsingMoveKeys(true))
    {

        glm::vec2 calc = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, mSpeed.mValue / 10);
        motion.x = calc.x;
        motion.z = calc.y;
        auto& keyboard = *ClientInstance::get()->getKeyboardSettings();

        bool isJumping = Keyboard::mPressedKeys[keyboard["key.jump"]];
        bool isSneaking = Keyboard::mPressedKeys[keyboard["key.sneak"]];

        if (isJumping)
            motion.y += mSpeed.mValue / 10;
        else if (isSneaking)
            motion.y -= mSpeed.mValue / 10;
    }

    if (mMode.mValue == Mode::Normal) player->getStateVectorComponent()->mVelocity = motion;
}

void Freecam::onRenderEvent(RenderEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!ClientInstance::get()->getLevelRenderer() || !player) return;
    
    AABB aabb = mOldAABB;

    auto drawList = ImGui::GetBackgroundDrawList();
    auto themeColor = ColorUtils::getThemedColor(0);

    std::vector<ImVec2> imPoints = MathUtils::getImBoxPoints(aabb);

    drawList->AddConvexPolyFilled(imPoints.data(), imPoints.size(), ImColor(themeColor.Value.x, themeColor.Value.y, themeColor.Value.z, 0.25f));
}