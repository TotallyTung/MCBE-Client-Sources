#pragma once
#include "../../Libs/entt/entt/entt.hpp"

#include "Attribute/Attribute.h"
#include "Attribute/BaseAttributeMap.h"
#include "../../Utils/MemoryUtil.h"
#include "Effect/MobEffect.h"
#include "Effect/MobEffectInstance.h"
#include "Item/ItemStack.h"
#include "Level/Level.h"

#include "ActorFlags.h"
#include "ActorFlags2.h"
#include "Components/AABBShapeComponent.h"
#include "Components/ActorEquipmentComponent.h"
#include "Components/MobHurtTimeComponent.h"
#include "Components/ActorHeadRotationComponent.h"
#include "Components/ActorRotationComponent.h"
#include "Components/ActorTypeComponent.h"
#include "Components/AttributesComponent.h"
#include "Components/AnimationComponent.h"
#include "Components/DimensionTypeComponent.h"
#include "Components/MobBodyRotationComponent.h"
#include "Components/FlagComponent.h"
#include "Components/MoveInputComponent.h"
#include "Components/RenderPositionComponent.h"
#include "Components/RuntimeIDComponent.h"
#include "Components/StateVectorComponent.h"
#include "Components/FallDistanceComponent.h"
#include "Components/MaxAutoStepComponent.h"

#include "EntityIdTraits.h"
#include "EntityID.h"
#include "EntityContext.h"

class Actor {
public:
    CLASS_MEMBER(Level*, level, 0x250);
    CLASS_MEMBER(StateVectorComponent*, stateVector, 0x290);
    CLASS_MEMBER(AABBShapeComponent*, aabbShape, 0x298);
    CLASS_MEMBER(ActorRotationComponent*, rotation, 0x2A0);
    CLASS_MEMBER(bool, isDestroying, 0x19F0);
public:
    Vec3<float> getHumanPos() {
        Vec3<float> targetPos = this->getEyePos();

        if (this->getActorTypeComponent()->id == 319) return targetPos.sub(Vec3<float>(0.f, 1.6f, 0.f));
        return targetPos;
    }

    template <typename T>
    T* getComponent() {
        return const_cast<T*>(getEntityContext()->getRegistry().try_get<T>(getEntityContext()->mEntity));
    }

    template <typename T>
    bool hasComponent() {
        return getEntityContext()->getRegistry().all_of<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void getOrEmplaceComponent() {
        return getEntityContext()->getRegistry().get_or_emplace<T>(getEntityContext()->mEntity);
    }

    template <typename T>
    void removeComponent() {
        getEntityContext()->getRegistry().remove<T>(getEntityContext()->mEntity);
    }


    ActorEquipmentComponent* getActorEquipmentComponent() {
        using func_t = ActorEquipmentComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getActorEquipmentComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    ActorHeadRotationComponent* getActorHeadRotationComponent() {
        using func_t = ActorHeadRotationComponent * (__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getActorHeadRotationComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    ActorTypeComponent* getActorTypeComponent() {
        using func_t = ActorTypeComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getActorTypeComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    DimensionTypeComponent* getDimensionTypeComponent() {
        // commands.tp.notSameDimension
        using func_t = DimensionTypeComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getDimensionTypeComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    MobBodyRotationComponent* getMobBodyRotationComponent() {
        using func_t = MobBodyRotationComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getMobBodyRotationComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    MoveInputComponent* getMoveInputHandler() {
        return getComponent<MoveInputComponent>();
    }

    RenderPositionComponent* getRenderPositionComponent() {
        using func_t = RenderPositionComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getRenderPositionComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    RuntimeIDComponent* getRuntimeIDComponent() {
        using func_t = RuntimeIDComponent*(__cdecl*)(__int64, __int64);
        static func_t Func = (func_t)(Addresses::try_getRuntimeIDComponent);
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }

    void setInvisible(bool invisible) {
        uintptr_t SetInvisableFunc = MemoryUtil::findSignature("48 89 74 24 ? 57 48 83 EC 50 48 8B 01 0F B6 F2 48 8B F9 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 40 3A C6");

        auto setInvis = reinterpret_cast<void(__thiscall*)(Actor*, bool)>(SetInvisableFunc); // setInvisible signature address
        return setInvis(this, invisible); // Return FunctionCall as Actor
    }

    Vec3<float> getPos() {
        return stateVector->pos;
    }

    Vec3<float> getEyePos() {
        return getRenderPositionComponent()->eyePos;
    }

    float calculateAttackDamage(Actor* target) {
        using func_t = float(__thiscall*)(Actor*, Actor*);
        static func_t Func = (func_t)Addresses::Actor_calculateAttackDamage;
        return Func(this, target);
    }

   /* std::string* getNametag() {
        static void* signature;

        if (signature == NULL) {
            signature = Memory::findSig(xorstr_("48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 3B 48 8B 08 BA ? ? ? ? 48 8B 40 ? 48 2B C1 48 C1 F8 ? 66 3B D0 73 17"));
        }
        //void* signature = Memory::findSig("48 83 EC ? 48 8B 81 ? ? ? ? 48 85 C0 74 3B 48 8B 08 BA ? ? ? ? 48 8B 40 ? 48 2B C1 48 C1 F8 ? 66 3B D0 73 17");

        auto getNameTag = reinterpret_cast<std::string * (__thiscall*)(Actor*)>(signature); // NameTags signature address
        return getNameTag(this); // Return FunctionCall as Actor
    }
*/
    std::string* getNameTag() {
        using func_t = std::string*(__thiscall*)(Actor*);
        static func_t Func = (func_t)Addresses::Actor_getNameTag;
        return Func(this);
    }
    void setIsOnGround(bool state) {
        if (state) { getOrEmplaceComponent<OnGroundFlagComponent>(); }
        else { removeComponent<OnGroundFlagComponent>(); }
    };

    bool isOnGround() {
        return hasComponent<OnGroundFlagComponent>();
    }

    void jumpFromGround() {
        getOrEmplaceComponent<FlagComponent<JumpFromGroundRequestFlag>>();
    }

    inline Actor* getVehicle() {
        return *(Actor**)((uintptr_t)this + 0x4D8);
    }

    ItemStack* getOffhandSlot() {
        ActorEquipmentComponent* actorEquipmentComponent = getActorEquipmentComponent();
        if (actorEquipmentComponent == nullptr)
            return nullptr;

        SimpleContainer* offhandContainer = actorEquipmentComponent->offhandContainer;

        return (ItemStack*)(*(__int64(__fastcall**)(SimpleContainer*, __int64))(*(uintptr_t*)offhandContainer + 56i64))(
            offhandContainer,
            1i64);
    }

    ItemStack* getArmor(int slot) {
        ActorEquipmentComponent* actorEquipmentComponent = getActorEquipmentComponent();
        if (actorEquipmentComponent == nullptr)
            return nullptr;

        return actorEquipmentComponent->armorContainer->getItem(slot);
    }

    bool hasEffect(MobEffect* effect) {
        using hasEffect_t = bool(__thiscall*)(Actor*, MobEffect*);
        static hasEffect_t func = (hasEffect_t)Addresses::Actor_hasEffect;
        return func(this, effect);
    }

    MobEffectInstance* getEffect(MobEffect* effect) {
        using getEffect_t = MobEffectInstance*(__thiscall*)(Actor*, MobEffect*);
        static getEffect_t func = (getEffect_t)Addresses::Actor_getEffect;
        return func(this, effect);
    }

    void setPos(const Vec3<float>& pos) {
        using func_t = __int64(__thiscall*)(Actor*, const Vec3<float>&);
        static func_t Func = (func_t)(Addresses::Actor_setPos);
        Func(this, pos);
    }

    bool hasHorizontalCollision() {
        using func_t = bool(__thiscall*)(__int64, __int64);
        static func_t Func = (func_t)Addresses::ActorCollision_hasHorizontalCollision;
        __int64 a1 = *(__int64*)((__int64)this + 0x10);
        __int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
        return Func(a1, a2);
    }
    AttributesComponent* getAttributesComponent() {
        return getComponent<AttributesComponent>();
    }

    AttributeInstance* getAttribute(int id) {
        auto& map = getAttributesComponent()->baseAttributeMap.attributes;
        auto it = map.find(id);
        if (it != map.end()) {
            return &it->second;
        }
        return nullptr;
    }

    int32_t getHurtTime() {
        return getComponent<MobHurtTimeComponent>()->hurtTime;
    }

    AttributeInstance* getAttribute(AttributeId id) {
        return getAttribute((int)id);
    }
    float getFallDistance() {
        return getComponent<FallDistanceComponent>()->fallDistance;
    }

    void setFallDistance(float distance) {
        getComponent<FallDistanceComponent>()->fallDistance = distance;
    }

    float getAbsorption() {
        return getAttribute(AttributeId::Absorption)->currentValue;
    }

    float getHealth() {
        return getAttribute(AttributeId::Health)->currentValue;
    }

    void lerpMotion(const Vec3<float>& delta) {
        MemoryUtil::CallVFunc<24, void, const Vec3<float>&>(this, delta);
    }

    bool isAlive() {
        return MemoryUtil::CallVFunc<50, bool>(this);
    }

    ItemStack* getCarriedItem() {
        return MemoryUtil::CallVFunc<83, ItemStack*>(this);
    }

    void setOffhandSlot(ItemStack* itemStack) {
        MemoryUtil::CallVFunc<84, void, ItemStack*>(this, itemStack);
    }

    AnimationComponent* getAnimationComponent() {
        return MemoryUtil::CallVFunc<115, AnimationComponent*>(this);
    }

    void swing() {
        MemoryUtil::CallVFunc<117, void>(this);
    }
    EntityContext* getEntityContext()
    {
        uintptr_t address = reinterpret_cast<uintptr_t>(this);
        return reinterpret_cast<EntityContext*>((uintptr_t)this + 0x8); // 1.21.2
    }
public:
    virtual bool fetchStatusFlagComponent(ActorFlags flag);
    virtual void setStatusFlag(ActorFlags flag, bool value);
};
