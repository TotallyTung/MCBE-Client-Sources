#pragma once
//
// Created by vastrakai on 7/8/2024.
//

#include <Hook/Hook.hpp>
#include <Hook/HookManager.hpp>


class ActorRenderDispatcher;
class BaseActorRenderContext;
class Actor;

class ActorRenderDispatcherHook : public Hook {
public:
    ActorRenderDispatcherHook() : Hook() {
        mName = "ActorRenderDispatcher::render";
    }

    static std::unique_ptr<Detour> mDetour;

    static void render(ActorRenderDispatcher* _this, class BaseActorRenderContext* entityRenderContext, Actor* entity, glm::vec3* cameraTargetPos, glm::vec3* pos, glm::vec2* rot, bool ignoreLighting);
    void init() override;
};

