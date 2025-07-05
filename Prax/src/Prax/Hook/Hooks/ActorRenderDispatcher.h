#pragma once
//
// Created by vastrakai on 9/4/2023.
//


#include <Prax/Hook/Hook.h>
#include <Prax/SDK/Game/Entity/Actor.h>

class ActorRenderDispatcher : public Hook {
public:
    ActorRenderDispatcher() : Hook("ActorRenderDispatcher") {
        InitOnConstruct = true;
    }
    /*
unsigned __int64 __fastcall ActorRenderDispatcher::render(
        ActorRenderDispatcher *this,
        BaseActorRenderContext *a2,
        Actor *a3,
        __int64 a4,
        __int64 a5,
        _QWORD *a6,
        char a7)
{
     */
    static std::unique_ptr<Detour> ActorRenderDispatcherDetour;

    static void* ActorRenderDispatcherDetourFunc(void* _this, void* a2, Actor* a3, Vector3* cameraPos, Vector3* actorPosition, Vector2* a6, bool a7);
    void Init() override;
    void UnHook() override;

    static void *ApplyRotationChanges(void *_this, void *a2, Actor *a3, Vector3 *cameraPos, Vector3 *actorPosition, Vector2 *a6,
                         bool a7);
};