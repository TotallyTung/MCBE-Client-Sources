#include "Actor.h"

uint64_t Actor::getRuntimeId() {
    if(auto* ric = this->ctx.tryGetComponent<RuntimeIDComponent>()) {
        return ric->mRuntimeId;
    };
    return 0;
};

uint8_t Actor::getEntityTypeId() {
    static void* sig = Mem::getRef("E8 ? ? ? ? 83 F8 5F"); // 48 83 EC 38 8B 41 18 48 8D 54 24 20 48 8B 49 10 89 44 24 20 E8 17 8B 01 00 48
    using Func = uint8_t (__thiscall*)(const Actor*);
    Func func = (Func)sig;

    auto tic = this->ctx.tryGetComponent<ActorTypeComponent>();
    
    return func ? func(this) : (tic ? tic->mTypeId : 0);
};

bool Actor::isAlive() {
    return Mem::CallVFunc<bool>(this, 48);
};

bool Actor::isMoving() {
    if(auto svc = this->ctx.tryGetComponent<StateVectorComponent>()) {
        return svc->mMotion.magnitudeXZ() >= 0.1f;
    };
    return false;
};

bool Actor::isJumping() {
    return this->ctx.hasComponent<MobIsJumpingFlagComponent>();
};

Vec2<float> Actor::getBodyRot() {
    if(auto arc = this->ctx.tryGetComponent<ActorRotationComponent>()) {
        return arc->mRot;
    };
    return {};
};

Vec3<float> Actor::getPosition() {
    if(auto svc = this->ctx.tryGetComponent<StateVectorComponent>()) {
        return svc->mPos;
    };
    return {};
};

void Actor::setPosition(const Vec3<float>& position) {
    if(!this->ctx.hasComponent<ActorSetPositionRequestComponent>()) {
        this->ctx.addComponent<ActorSetPositionRequestComponent>(std::move(position));
    };
};