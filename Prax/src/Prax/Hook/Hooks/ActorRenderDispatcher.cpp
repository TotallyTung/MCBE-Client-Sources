//
// Created by vastrakai on 9/4/2023.
//

#include <src/Prax/Module/Modules/Misc/NetSkip.h>
#include <Prax/Module/Modules/World/NoPacket.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Module/Modules/Visual/SelfCentered.h>
#include <Prax/Module/Modules/Visual/ShowRotations.h>
#include <xorstr.hpp>
#include <Prax/Module/Modules/Misc/Desync.h>
#include "ActorRenderDispatcher.h"

#include <Prax/SDK/Game/Entity/Components/MobBodyRotationComponent.h>
#include <Prax/Module/Modules/Player/Freecam.h>

#include "RakPeerHooks.h"


std::unique_ptr<Detour> ActorRenderDispatcher::ActorRenderDispatcherDetour = nullptr;

ActorRenderDispatcher* actorRenderDispatcher = nullptr;
void* actorRenderCtx = nullptr;
Actor* actor = nullptr;
Vector3* actorPos = nullptr;
Vector3* camPos = nullptr;
Vector2* vec2 = nullptr;
bool bl = false;

void* ActorRenderDispatcher::ActorRenderDispatcherDetourFunc(void *_this, void *a2, Actor *a3,
                                                             Vector3 *cameraPos, Vector3 *actorPosition, Vector2 *a6, bool a7) {
    auto oFunc = ActorRenderDispatcherDetour->GetFastcall<void*, void*, void*, Actor*, Vector3*, Vector3*, Vector2*, bool>();
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player || !ModuleManager::initialized) {
        return oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    bool isLocalPlayer = a3 == player;

    static auto selfCenteredModule = ModuleManager::GetModule<SelfCentered>();
    static auto noPacket = ModuleManager::GetModule<NoPacket>();
    static auto freecam = ModuleManager::GetModule<Freecam>();

    if (selfCenteredModule && selfCenteredModule->Enabled && a3->isPlayer()){
        a3 = player;
    }

    if (a3 != player || !isLocalPlayer) {
        return oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }




    // Stuff is rendered relative to the camera position
    // So, a position such as 10, 20, 10 is added to the camera position
    // To convert from a camera position, we just need to subtract the camera position
    // But if we want to render at a position of 10 20 10, we need to add the camera position to it

    if (freecam->Enabled) {
        auto newPos = *actorPosition - *cameraPos - *actorPosition + Freecam::LastPos;
        // We don't want to apply rotation changes to the freecam, we want to apply our own based on da last rotations in freecam

        auto actorRotations = a3->GetActorRotationComponent();
        auto actorHeadRotations = a3->GetActorHeadRotationComponent();
        auto mobBodyRot = a3->entityCtx.getComponent<MobBodyRotationComponent>();

        float realOldYaw = actorRotations->oldYaw;
        float realYaw = actorRotations->yaw;
        float realOldPitch = actorRotations->oldPitch;
        float realPitch = actorRotations->pitch;
        float realHeadRot = actorHeadRotations->headRot;
        float realOldHeadRot = actorHeadRotations->oldHeadRot;
        float realBodyYaw = mobBodyRot->yBodyRot;
        float realOldBodyYaw = mobBodyRot->yOldBodyRot;

        actorRotations->oldYaw = Freecam::LastYaw.x;
        actorRotations->yaw = Freecam::LastYaw.y;
        actorRotations->oldPitch = Freecam::LastPitch.x;
        actorRotations->pitch = Freecam::LastPitch.y;
        actorHeadRotations->headRot = Freecam::LastHeadRot.x;
        actorHeadRotations->oldHeadRot = Freecam::LastHeadRot.y;
        mobBodyRot->yBodyRot = Freecam::LastBodyRot.x;
        mobBodyRot->yOldBodyRot = Freecam::LastBodyRot.y;

        auto ret = oFunc(_this, a2, a3, cameraPos, &newPos, a6, a7);

        actorRotations->oldYaw = realOldYaw;
        actorRotations->yaw = realYaw;
        actorRotations->oldPitch = realOldPitch;
        actorRotations->pitch = realPitch;
        actorHeadRotations->headRot = realHeadRot;
        actorHeadRotations->oldHeadRot = realOldHeadRot;
        mobBodyRot->yBodyRot = realBodyYaw;
        mobBodyRot->yOldBodyRot = realOldBodyYaw;

        return ret;

    }

    if (noPacket->Enabled) {

        if (noPacket->VisualizeSetting.Enabled)
        {
            ApplyRotationChanges(_this, a2, a3, cameraPos, actorPosition, a6, a7);
        }

        auto newPos = *actorPosition - *cameraPos - *actorPosition + NoPacket::LastPos;
        return ApplyRotationChanges(_this, a2, a3, cameraPos, &newPos, a6, false);
    }

    if (RakPeerHooks::Visualize && RakPeerHooks::RenderCurrentAndLastPos && RakPeerHooks::Ticking) {
        ApplyRotationChanges(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    if (RakPeerHooks::Ticking && RakPeerHooks::Visualize) {
        auto newPos = *actorPosition - *cameraPos - *actorPosition + RakPeerHooks::LastPos;
        return ApplyRotationChanges(_this, a2, a3, cameraPos, &newPos, a6, false);
    }

    if (Desync::Ticking && Desync::Visualize) {
        auto newPos = *actorPosition - *cameraPos - *actorPosition + Desync::LastPos;
        return ApplyRotationChanges(_this, a2, a3, cameraPos, &newPos, a6, false);
    }



    if (!RakPeerHooks::Ticking || !RakPeerHooks::Visualize) {
        return ApplyRotationChanges(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    return nullptr;
}

// Include all the args of the original function
void* ActorRenderDispatcher::ApplyRotationChanges(void* _this, void* a2, Actor* a3, Vector3* cameraPos, Vector3* actorPosition, Vector2* a6, bool a7) {
    auto oFunc = ActorRenderDispatcherDetour->GetFastcall<void*, void*, void*, Actor*, Vector3*, Vector3*, Vector2*, bool>();
    bool firstPerson = Minecraft::ClientInstance->GetOptions()->game_thirdperson->value == 0;
    auto actorRotations = a3->GetActorRotationComponent();
    auto actorHeadRotations = a3->GetActorHeadRotationComponent();
    auto mobBodyRot = a3->entityCtx.getComponent<MobBodyRotationComponent>();
    static auto showRotations = ModuleManager::GetModule<ShowRotations>();

    if (!showRotations)
    {
        showRotations = ModuleManager::GetModule<ShowRotations>();
        return oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    if (firstPerson) {
        return oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    if (!showRotations->Enabled) {
        return oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);
    }

    float realOldPitch = actorRotations->oldPitch;
    float realPitch = actorRotations->pitch;
    float realHeadRot = actorHeadRotations->headRot;
    float realOldHeadRot = actorHeadRotations->oldHeadRot;
    float realBodyYaw = mobBodyRot->yBodyRot;
    float realOldBodyYaw = mobBodyRot->yOldBodyRot;

    actorRotations->oldPitch = ShowRotations::LerpedPitch;
    actorRotations->pitch = ShowRotations::LerpedPitch;
    actorHeadRotations->headRot = ShowRotations::LerpedHeadYaw;
    actorHeadRotations->oldHeadRot = ShowRotations::LerpedHeadYaw;
    mobBodyRot->yBodyRot = ShowRotations::LerpedBodyYaw;
    mobBodyRot->yOldBodyRot = ShowRotations::LerpedBodyYaw;

    auto ret = oFunc(_this, a2, a3, cameraPos, actorPosition, a6, a7);

    actorRotations->oldPitch = realOldPitch;
    actorRotations->pitch = realPitch;
    actorHeadRotations->headRot = realHeadRot;
    actorHeadRotations->oldHeadRot = realOldHeadRot;
    mobBodyRot->yBodyRot = realBodyYaw;
    mobBodyRot->yOldBodyRot = realOldBodyYaw;

    return ret;
}

void ActorRenderDispatcher::Init() {
    uintptr_t sigAddr = SigManager::GetSignatureAddress(SignatureEnum::ActorRenderDispatcher_render);
    ActorRenderDispatcherDetour = std::make_unique<Detour>("ActorRenderDispatcherDetour", sigAddr, &ActorRenderDispatcherDetourFunc);
    ActorRenderDispatcherDetour->Enable();
    this->Detours.emplace_back(ActorRenderDispatcherDetour.get());
}

void ActorRenderDispatcher::UnHook() {

}


