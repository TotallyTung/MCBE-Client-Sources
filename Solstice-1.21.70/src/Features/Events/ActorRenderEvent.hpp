//
// Created by vastrakai on 7/8/2024.
//

#pragma once

#include "Event.hpp"

#include <memory>
#include <Hook/Hook.hpp>

class ActorRenderDispatcher;
class BaseActorRenderContext;
class Actor;

struct ActorRenderEvent : public CancelableEvent {
    ActorRenderDispatcher* _this;
    BaseActorRenderContext* mEntityRenderContext;
    Actor* mEntity;
    glm::vec3* mCameraTargetPos;
    glm::vec3* mPos;
    glm::vec2* mRot;
    bool mIgnoreLighting;
    Detour* mDetour;

    explicit ActorRenderEvent(ActorRenderDispatcher* _this, BaseActorRenderContext* entityRenderContext, Actor* entity, glm::vec3* cameraTargetPos, glm::vec3* pos, glm::vec2* rot, bool ignoreLighting, Detour* detour) : _this(_this), mEntityRenderContext(entityRenderContext), mEntity(entity), mCameraTargetPos(cameraTargetPos), mPos(pos), mRot(rot), mIgnoreLighting(ignoreLighting), mDetour(detour) {}

    void setPlayerPos(const glm::vec3& pos) const {
        *mPos = *mPos - *mCameraTargetPos - *mPos + pos;
    }

    // gets the player's position (world origin)
    [[nodiscard]] glm::vec3 getPlayerPos() const {
        return *mCameraTargetPos + *mPos;
    }
};