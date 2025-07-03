#pragma once

#include "Model/ActorModel.h"
#include "Attribute/Attribute.h"
#include "Attribute/BaseAttributeMap.h"
#include "Data/ActorRenderData.h"
#include "Data/SynchedActorDataEntityWrapper.h"
#include "Data/SynchedActorData.h"
#include "Data/DataItem.h"
#include "ActorDefinitionIdentifier.h"
#include "PropertySyncData.h"
#include "Animation/ActorAnimationPlayer.h"
#include "RenderParams.h"
#include "ActorRenderDispatcher.h"
#include "Control.h"

#include "Components/AttributesComponent.h"
#include "Components/ActorGameTypeComponent.h"
#include "Components/ActorTypeComponent.h"
#include "Components/ActorUniqueIDComponent.h"
#include "Components/JumpComponent.h"
#include "Components/RuntimeIDComponent.h"
#include "Components/ActorWalkAnimationComponent.h"
#include "Components/DepenetrationComponent.h"
#include "Components/ActorDataDirtyFlagsComponent.h"
#include "Components/ActorDefinitionIdentifierComponent.h"
#include "Components/StateVectorComponent.h"
#include "Components/AABBShapeComponent.h"
#include "Components/SubBBsComponent.h"
#include "Components/ActorDataFlagComponent.h"
#include "Components/BlockSourceComponent.h"
#include "Components/ActorEquipmentComponent.h"
#include "Components/SynchedActorDataComponent.h"
#include "Components/ActorHeadRotationComponent.h"
#include "Components/ActorRotationComponent.h"
#include "Components/ShadowOffsetComponent.h"
#include "Components/OffsetsComponent.h"
#include "Components/CameraComponent.h"
#include "Components/CameraDirectLookComponent.h"
#include "Components/DebugCameraComponent.h"
#include "Components/CameraPresetComponent.h"
#include "Components/UpdatePlayerFromCameraComponent.h"
#include "Components/CameraBobComponent.h"
#include "Components/ActorOwnerComponent.h"
#include "Components/BlockMovementSlowdownMultiplierComponent.h"
#include "Components/FallDistanceComponent.h"
#include "Components/MaxAutoStepComponent.h"
#include "Components/MobHurtTimeComponent.h"
#include "Components/MobBodyRotationComponent.h"
#include "Components/MobEffectsComponent.h"
#include "Components/RenderPositionComponent.h"
#include "Components/MoveInputComponent.h"
#include "Components/MovementInterpolatorComponent.h"
#include "Components/UserEntityIdentifierComponent.h"
#include "Components/FlagComponent.h"

#include "EntityContext/EntityIdTraits.h"
#include "EntityContext/EntityID.h"
#include "EntityContext/EntityContext.h"

class Actor { // Actor Class 1.21.50
public:
	unsigned char filler[0x3A8]; // ItemActor::ItemActor on bds line 35 ItemStack::ItemStack((ItemActor *)((char *)this + 936));
public:
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

	AttributeInstance* getAttribute(int id) {
		auto& map = getAttributesComponent()->mBaseAttributeMap.mAttributes;
		auto it = map.find(id);
		if (it != map.end()) {
			return &it->second;
		}
		return nullptr;
	}

	Vector3<int> getFeetBlockPos() {
		Vector3<int> vec = getPosition().ToInt();
		return Vector3<int>{
			static_cast<int32_t>(std::floorf(vec.x)),
				static_cast<int32_t>(std::floorf(getAABBShapeComponent()->mPosLower.y)),
				static_cast<int32_t>(std::floorf(vec.z))
		};
	}

	std::string* getNametag() {
		return Memory::CallFunc<std::string*>(SigManager::Actor_getNameTag, this);
	}

	int getSwingProgress() {
		return hat::member_at<int>(this, OffsetManager::Actor_mSwinging + 0x4);
	}

	AttributesComponent* getAttributesComponent() {
		return getComponent<AttributesComponent>();
	}

	MoveInputComponent* getMoveInputHandler() {
		return getComponent<MoveInputComponent>();
	}

	StateVectorComponent* getStateVectorComponent() {
		return getComponent<StateVectorComponent>();
	}
	
	AABBShapeComponent* getAABBShapeComponent() {
		return getComponent<AABBShapeComponent>();
	}

	JumpControlComponent* getJumpControl() {
		return getComponent<JumpControlComponent>();
	}

	int getHurtTime() {
		return getComponent<MobHurtTimeComponent>()->mHurtTime;
	}

	bool getStatusFlag(ActorFlags flag) {
		return getComponent<ActorDataFlagComponent>()->getStatusFlag(flag);
	}

	void setStatusFlag(ActorFlags flag, bool value) {
		getComponent<ActorDataFlagComponent>()->setStatusFlag(flag, value);
	}

	GameType getGameType() {
		return getComponent<ActorGameTypeComponent>()->mGameType;
	}

	void setGameType(GameType type) {
		getComponent<ActorGameTypeComponent>()->mGameType = type;
	}

	void swing() {
		Memory::CallVirtualFunc<void>(OffsetManager::Actor_swing, this);
	}

	void setSprinting(bool state) {
		getMoveInputHandler()->mSprinting = state;
	}

	void setJumping(bool state) {
		getMoveInputHandler()->setJumping(state);
	}

	float getPitch() {
		return getComponent<ActorRotationComponent>()->mRotation.x;
	}

	void setPitch(float rotation) {
		getComponent<ActorRotationComponent>()->mRotation.x = rotation;
	}

	float getHeadYaw() {
		return getComponent<ActorHeadRotationComponent>()->mRotation.x;
	}

	void setHeadYaw(float rotation) {
		getComponent<ActorHeadRotationComponent>()->mRotation.x = rotation;
	}

	float getBodyRotation() {
		return getComponent<MobBodyRotationComponent>()->mBodyRot;
	}

	void setBodyRotation(float rotation) {
		getComponent<MobBodyRotationComponent>()->mBodyRot = rotation;
	}

	AttributeInstance* getAttribute(AttributeId id) {
		return getAttribute(static_cast<int>(id));
	}

	float getAbsorption() {
		return getAttribute(AttributeId::Absorption)->mCurrentValue;
	}

	float getHealth() {
		return getAttribute(AttributeId::Health)->mCurrentValue;
	}

	int64_t getRuntimeID() {
		return getComponent<RuntimeIDComponent>()->mRuntimeID;
	}

	int64_t getUniqueID() {
		return getComponent<ActorUniqueIDComponent>()->mUniqueID;
	}

	Vector3<float> getRenderPosition() {
		return getComponent<RenderPositionComponent>()->mRenderPos;
	}

	Vector3<float>* getPos() {
		return &getStateVectorComponent()->mPosition;
	}

	Vector3<float> getPosition() {
		if (getStateVectorComponent() == nullptr) return Vector3<float>(0, 0, 0);
		return *getPos();
	}

	bool isPlayer() {
		return hasComponent<PlayerComponent>();
	}

	bool isLocalPlayer() {
		return hasComponent<LocalPlayerComponent>();
	}

	float getFallDistance() {
		return getComponent<FallDistanceComponent>()->mFallDistance;
	}

	void setFallDistance(float distance) {
		getComponent<FallDistanceComponent>()->mFallDistance = distance;
	}

	void setPlayerIsSleeping(bool state) {
		if (state) { getOrEmplaceComponent<PlayerIsSleepingFlagComponent>(); }
		else { removeComponent<PlayerIsSleepingFlagComponent>(); }
	}

	SimpleContainer* getArmorContainer() {
		return getComponent<ActorEquipmentComponent>()->mArmorContainer;
	}

	class ItemStack* getArmor(ArmorSlot slot) {
		return getArmorContainer()->getItemStack(slot);
	}

	bool isSneaking() {
		return getMoveInputHandler()->mSneaking;
	}

	void setSneaking(bool state) {
		getMoveInputHandler()->mSneaking = state;
	}

	ActorType getEntityTypeId() {
		return getComponent<ActorTypeComponent>()->mType;
	}

	bool isStuckInCollider() {
		return getComponent<DepenetrationComponent>()->isPenetrating();
	}

	bool isCollidingHorizontal() {
		return hasComponent<HorizontalCollisionFlagComponent>();
	}
	
	bool isCollidingVertical() {
		return hasComponent<VerticalCollisionFlagComponent>();
	}

	bool isOnFire() {
		return getStatusFlag(ActorFlags::Onfire);
	}

	void setOnFire(bool state) {
		if (state) { getOrEmplaceComponent<OnFireComponent>(); }
		else { removeComponent<OnFireComponent>(); }
	}

	bool isImmobile() {
		return getStatusFlag(ActorFlags::Immobile);
	}

	void setImmobile(bool state) {
		setStatusFlag(ActorFlags::Immobile, state);
	}

	bool isAlive() {
		return !hasComponent<IsDeadFlagComponent>();
	}

	bool isDead() {
		return hasComponent<IsDeadFlagComponent>();
	}

	bool isOnGround() {
		return hasComponent<OnGroundFlagComponent>();
	}

	void setIsOnGround(bool state) {
		if (state) { getOrEmplaceComponent<OnGroundFlagComponent>(); }
		else { removeComponent<OnGroundFlagComponent>(); }
	};

	void jumpFromGround() {
		getOrEmplaceComponent<JumpFromGroundRequestComponent>();
	}

	Level* getLevel() {
		return hat::member_at<Level*>(this, OffsetManager::Actor_mLevel);
	}

	EntityContext* getEntityContext()
	{
		uintptr_t address = reinterpret_cast<uintptr_t>(this);
		return reinterpret_cast<EntityContext*>((uintptr_t)this + 0x8);
	}
public:
	CLASS_FIELD(uintptr_t**, VTable, 0x0);
};