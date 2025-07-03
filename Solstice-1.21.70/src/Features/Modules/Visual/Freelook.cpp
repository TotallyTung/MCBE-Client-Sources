//
// Created by vastrakai on 11/2/2024.
//

#include "Freelook.hpp"


#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Options.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/Components/CameraComponent.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>

void Freelook::onEnable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player)
    {
        setEnabled(false);
        return;
    }

    gFeatureManager->mDispatcher->listen<BaseTickEvent, &Freelook::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Freelook::onPacketOutEvent>(this);

    auto gock = player->getActorHeadRotationComponent();
    mHeadYaw = { gock->mHeadRot, gock->mOldHeadRot };

    for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
    {
        player->mContext.mRegistry->set_flag<CameraAlignWithTargetForwardComponent>(id, false);
        auto storage = player->mContext.mRegistry->assure_t<UpdatePlayerFromCameraComponent>();
        if (storage->contains(id))
        {
            mCameras[id] = storage->get(id).mUpdateMode;
            storage->remove(id);
        }

        if (cameraComponent.getMode() == CameraMode::FirstPerson)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraDirectLookComponent>(id);
            if (gaming)
            {
                mOriginalRotRads[cameraComponent.getMode()] = gaming->mRotRads;
            }
        } else if (cameraComponent.getMode() == CameraMode::ThirdPerson || cameraComponent.getMode() == CameraMode::ThirdPersonFront)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraOrbitComponent>(id);
            if (gaming)
            {
                mOriginalRotRads[cameraComponent.getMode()] = gaming->mRotRads;
            }
        }
    }

    mLastCameraState = ClientInstance::get()->getOptions()->mThirdPerson->value;
    ClientInstance::get()->getOptions()->mThirdPerson->value = 1;
}

void Freelook::onDisable()
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player)
    {
        return;
    }

    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &Freelook::onBaseTickEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Freelook::onPacketOutEvent>(this);

    auto options = ClientInstance::get()->getOptions();
    options->mThirdPerson->value = mLastCameraState;
    mResetRot = true;
}

void Freelook::onBaseTickEvent(BaseTickEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player)
    {
        setEnabled(false);
        return;
    }

    auto gock = player->getActorHeadRotationComponent();
    gock->mHeadRot = mHeadYaw.x;
    gock->mOldHeadRot = mHeadYaw.y;
}

void Freelook::onPacketOutEvent(PacketOutEvent& event)
{
    if (event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        paip->mYHeadRot = mHeadYaw.x;
    } else if (event.mPacket->getId() == PacketID::MovePlayer)
    {
        auto mpp = event.getPacket<MovePlayerPacket>();
        mpp->mYHeadRot = mHeadYaw.x;
    }
}

void Freelook::onLookInputEvent(LookInputEvent& event)
{
    if (!mResetRot) return;

    auto player = ClientInstance::get()->getLocalPlayer();
    for (auto&& [id, cameraComponent] : player->mContext.mRegistry->view<CameraComponent>().each())
    {
        player->mContext.mRegistry->set_flag<CameraAlignWithTargetForwardComponent>(id, true);
        auto storage = player->mContext.mRegistry->assure_t<UpdatePlayerFromCameraComponent>();
        if (!storage->contains(id))
        {
            storage->emplace(id, UpdatePlayerFromCameraComponent(mCameras[id]));
        }

        if (cameraComponent.getMode() == CameraMode::FirstPerson)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraDirectLookComponent>(id);
            if (gaming)
            {
                gaming->mRotRads = mOriginalRotRads[cameraComponent.getMode()];
            }
        } else if (cameraComponent.getMode() == CameraMode::ThirdPerson || cameraComponent.getMode() == CameraMode::ThirdPersonFront)
        {
            auto* gaming = player->mContext.mRegistry->try_get<CameraOrbitComponent>(id);
            if (gaming)
            {
                gaming->mRotRads = mOriginalRotRads[cameraComponent.getMode()];
            }
        }
    }

    mResetRot = false;
}
