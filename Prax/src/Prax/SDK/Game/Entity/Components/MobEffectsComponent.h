//
// Created by vastrakai on 5/4/2024.
//

#pragma once

/*
const MobEffectInstance* Actor::getEffect(Actor* actor, const MobEffect* effect) {
    unsigned int priority;
    long long actorRegistry;
    unsigned long long effectComponent;
    _QWORD* mobEffectsComponent;
    gsl::details* gslDetails;
    _QWORD* effectData;
    int localArray[6];

    priority = PrioritizedGoal::getPriority(effect);
    actorRegistry = *((long long*)actor + 2);
    localArray[0] = *((int*)actor + 6);
    effectComponent = priority;
    mobEffectsComponent = (_QWORD*)entt::basic_registry<EntityId, std::allocator<EntityId>>::try_get<MobEffectsComponent>(actorRegistry, localArray);
    effectData = mobEffectsComponent;
    if (!mobEffectsComponent)
        gsl::details::terminate(gslDetails);
    if ((__int64)(mobEffectsComponent[1] - *mobEffectsComponent) >> 7 > effectComponent
        && (effectData = effectComponent << 7,
            (unsigned __int8)TypedServerNetId<ItemStackNetIdTag, int, 0>::operator!=(
                effectData + *mobEffectsComponent,
                &MobEffectInstance::NO_EFFECT))) {
        return (const MobEffectInstance*)(effectData + *effectData);
    }
    else {
        return nullptr;
    }
}*/


struct MobEffectsComponent {
    char pad_0x0000[0x18]; //0x0000
};

static_assert(sizeof(MobEffectsComponent) == 0x18, "MobEffectsComponent is not the correct size");
