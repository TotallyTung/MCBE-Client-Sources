// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "../../Utils/MemoryUtils.h"
#include <vector>
#include "Level.h"
#include "GameMode.h"
#include "Inventory.h"
#include "Dimension.h"
#include "MobEffectInstance.h"
#include "SimpleContainer.h"
#include "ActorUniqueID.h"
#include "../Components/ActorTypeComponent.h"
#include "../Components/AABBShapeComponent.h"
#include "../Components/FallDistanceComponent.h"
#include "../Components/MobEffectsComponent.h"
#include "../Components/RenderPositionComponent.h"
#include "../Components/RuntimeIDComponent.h"
#include "../Components/StateVectorComponent.h"
#include "../Components/ActorRotationComponent.h"
#include "../Components/MobBodyRotationComponent.h"
#include "../Components/ActorHeadRotationComponent.h"
#include "../Components/Flags/OnGroundFlag.h"

class HashedString;
class EntityContext;
class Block;
class ItemStack;
enum MaterialType;
class Player;


struct EntityId {
	uint32_t value;
};

class EntityRegistry;

struct EntityContext {
	EntityRegistry* registry;
	EntityId id;
	
};

class Actor {
public:
	BUILD_ACCESS(this, EntityContext*, entityContext, 0x8);
	BUILD_ACCESS(this, int16_t, hurtTime, 0x22C);
	BUILD_ACCESS(this, std::shared_ptr<Dimension>, dimension, 0x278);
	BUILD_ACCESS(this, StateVectorComponent*, stateVectorComponent, 0x2C8);
	BUILD_ACCESS(this, ActorRotationComponent*, rotationComponent, 0x2D8); // 48 8B 83 ? ? ? ? 0F 28 C7
	BUILD_ACCESS(this, AABBShapeComponent*, aabbComponent, 0x2D0);
	BUILD_ACCESS(this, bool, isDestroying, 0x1988);
protected:
	template<typename T> // ncc
	T* getComponent(uintptr_t funcAddr) {
		using func_t = T * (__cdecl*)(__int64, __int64);
		func_t func = (func_t)(funcAddr);
		__int64 a1 = *(__int64*)((__int64)this + 0x10);
		__int64 a2 = (__int64)&(*(__int64*)((__int64)this + 0x18));
		return func(a1, a2);
	}
public:

	AABB makeAABB(const Vec3<int>* pos) {
		AABB aabb;
		BlockSource* blockSource = this->dimension->blockSource;
		Block* block = blockSource->getBlock(*pos);
		BlockLegacy* blockLegacy = block->blockLegacy;

		blockLegacy->getCollisionShape(&aabb, block, blockSource, pos);
		return aabb;
	}

	SimpleContainer* getArmorContainer() {
		return (SimpleContainer*)*((uintptr_t*)this + 0x8A);
	}

	ItemStack* getArmor(int slot) {
		return this->getArmorContainer()->getItemStack(slot);
	}

	std::string* getNameTag() {
		using func_t = std::string* (__thiscall*)(Actor*);
		static func_t func = (func_t)findSig(Sigs::actor::getNameTag);
		return func(this);
	}

	class ItemStack* getOffhandSlot() {
		return (class ItemStack*)(*(__int64(__fastcall**)(uintptr_t, __int64))(**((uintptr_t**)this + 0x8B) + 40i64))(*((uintptr_t*)this + 0x8B), 1i64);
	}
	Level* getLevel() { // cai nay ko sai dc BUILD_ACCESS
		return (Level*)*((uintptr_t*)this + 0x51);
	}
	Vec3<float>* getPosPrev() {
		Vec3<float>* v1 = getPosition();
		return (Vec3<float>*)(v1 + 0xC);
	}
	Vec3<float>* getPosition() {
		return (Vec3<float>*)*((uintptr_t*)this + 0x59);
	}
	
	Vec3<float> getHumanPos() {
		Vec3<float> targetPos = this->getEyePos();

		if (this->getEntityTypeId() == 319) return targetPos.sub(Vec3<float>(0.f, 1.6f, 0.f));
		return targetPos;
	}

	void setPos(Vec3<float> pos) {
		using func_t = void* (__thiscall*)(Actor*, Vec3<float> const&, const char*);
		static func_t func = (func_t)(MemoryUtils::getBase() + 0x2648380);
		func(this, pos, nullptr);
	}
	void setonground(EntityContext* a,bool onground) {
		using func_t = void(__thiscall*)(EntityContext*, bool);
		static func_t func = (func_t)findSig(Sigs::actor::setonground);
		return func(a, onground);
	}
	Vec2<float>* getRotationPrev() {
		Vec2<float>* v1 = getRotation();
		return (Vec2<float>*)(v1 + 0x8);
	}
	Vec2<float>* getRotation() {
		return (Vec2<float>*)*((uintptr_t*)this + 0x5B);
	}
	AABB* getAABB() {
		return (AABB*)*((uintptr_t*)this + 0x5A);
	}

	bool isOnGround() {
		return getOnGroundFlagComponent();
	}
	
	
	ActorTypeComponent* getActorTypeComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::getActorTypeComponent));
		return getComponent<ActorTypeComponent>(funcAddr);
	}
	//   Memory::findSig     (    std::string    (    GET_SIG   (    "tryGetPrefix"   )    ) + " " +    GET_SIG   (   "Actor::getMoveInputHandler"    )   )  ;
	/*MoveInputComponent* getMoveInputHandler() { //??$try_get@UMoveInputComponent

		static uintptr_t sig = MemoryUtils::findSigdll(std::string(GET_SIG("tryGetPrefix")) + " " + GET_SIG("Actor::getMoveInputHandler"));

		return tryGet<MoveInputComponent>(sig);
	}*/
	FallDistanceComponent* getFallDistanceComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::getFallDistanceComponent));
		return getComponent<FallDistanceComponent>(funcAddr);
	}
	MobEffectsComponent* getMobEffectsComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::getMobEffectsComponent));
		return getComponent<MobEffectsComponent>(funcAddr);
	}
	RenderPositionComponent* getRenderPositionComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::getRenderPositionComponent));
		return getComponent<RenderPositionComponent>(funcAddr);
	}
	RuntimeIDComponent* getRuntimeIDComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(MemoryUtils::getBase() + 0x24EBBA7); // E8 ? ? ? ? 48 85 C0 0F 84 ? ? ? ? 48 8B 00 C7 44 24 ? ? ? ? ? C7 44 24 ? ? ? ? ? 66 C7 44 24 ? ? ? 33 C9 48 89 4C 24 ? 48 89 4C 24 ? 89 4C 24 58 48 8D 0D ? ? ? ? 48 89 4C 24 ? 48 89 44 24 ? C6 44 24 ? ? 89 7C 24 6C 48 8B 8B ? ? ? ? 48 8B 01 48 8B 80 ? ? ? ? FF 15 ? ? ? ? 4C 8B C0 48 8B 08 48 8B 41 08 48 8D 54 24 ? 49 8B C8 FF 15 ? ? ? ? 90 48
		return getComponent<RuntimeIDComponent>(funcAddr);
	}
	ActorUniqueID* getActorUniqueIDComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::getActorUniqueIDComponent));
		return getComponent<ActorUniqueID>(funcAddr);
	}
	ActorHeadRotationComponent* getActorHeadRotationComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::hook::Headrotationcomponemt));
		return getComponent<ActorHeadRotationComponent>(funcAddr);
	}
	MobBodyRotationComponent* getMobBodyRotationComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::hook::mobrotationhook));
		return getComponent<MobBodyRotationComponent>(funcAddr);
	}
	OnGroundFlag* getOnGroundFlagComponent() {
		static uintptr_t funcAddr = MemoryUtils::getFuncFromCall(findSig(Sigs::component::onGroundFlag));
		return getComponent<OnGroundFlag>(funcAddr);
	}
	uint32_t getEntityTypeId() { //e8 ? ? ? ? 3d ? ? ? ? 89 9d 
		ActorTypeComponent* component = getActorTypeComponent();
		if (component == nullptr) return 0;
		return *(uint32_t*)component;
	}

	float getFallDistance() {
		FallDistanceComponent* component = getFallDistanceComponent();
		if (component == nullptr) return 0.f;
		return *(float*)component;
	}
	Vec2<float> getInterpolatedRotation(float partialTicks) {
		return getRotationPrev()->mul((1.0f - partialTicks)).add(getRotation()->mul(partialTicks));
	}
	MobEffectInstance* getEffect(unsigned int effectId) {
		MobEffectsComponent* mobEffectsComponent = getMobEffectsComponent();
		if (!mobEffectsComponent) return nullptr;

		uintptr_t vtableStart = *reinterpret_cast<uintptr_t*>(mobEffectsComponent);
		size_t vtableSize = (vtableStart >> 7);
		if (vtableSize <= effectId) return nullptr;

		uintptr_t effectAddress = (static_cast<unsigned long long>(effectId) << 7) + vtableStart;
		if (*(uintptr_t*)effectAddress == MemoryUtils::getBase() + 0x5515A10) return nullptr;

		return reinterpret_cast<MobEffectInstance*>(effectAddress);
	}
	Vec3<float> getEyePos() {
		RenderPositionComponent* component = getRenderPositionComponent();
		if (component == nullptr) return component->pos;
		return *(Vec3<float>*)component;
	}
	
	
	uint64_t getRuntimeID() { //sub_142645130 //40 53 48 83 EC 20 8B 41 18 48 8B DA 48 8B 49 10 48 8D 54 24 30 89 44 24 30 E8 ?? ?? ?? ?? 48 85 C0 74 0F
		RuntimeIDComponent* component = getRuntimeIDComponent();
		if (component == nullptr) return 0;
		return *(uint64_t*)component;
	}
public:
	virtual ~Actor(void);
	virtual void getStatusFlag(enum ActorFlags);
	virtual void setStatusFlag(enum ActorFlags, bool);
	virtual void outOfWorld(void);
	virtual void reloadHardcoded(enum ActorInitializationMethod, class VariantParameterList const&);
	virtual void reloadHardcodedClient(enum ActorInitializationMethod, class VariantParameterList const&);
	virtual void initializeComponents(enum ActorInitializationMethod, class VariantParameterList const&);
	virtual void reloadComponents(enum ActorInitializationMethod, class VariantParameterList const&);
	virtual void _serverInitItemStackIds(void);
	virtual void _doInitialMove(void);
	virtual bool hasComponent(HashedString const&);
	virtual void resetUserPos(bool);
	virtual void getOwnerEntityType(void);
	virtual void remove(void);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual Vec3<float> getFiringPos(void);
	virtual float getInterpolatedBodyRot(float); //16
	virtual float getInterpolatedHeadRot(float); //17 HeadYaw
	virtual float getInterpolatedBodyYaw(float); //18
	virtual float getYawSpeedInDegreesPerSecond(void);
	virtual Vec3<float> getInterpolatedRidingOffset(float, int);
	virtual void resetInterpolated(void);
	virtual bool isFireImmune(void);
	virtual void breaksFallingBlocks(void);
	virtual void blockedByShield(enum ActorDamageSource const&, Actor&);
	virtual bool canDisableShield(void);
	virtual void teleportTo(Vec3<float> const&, bool, int, int, bool);
	virtual void lerpMotion(Vec3<float> const&);
	virtual void tryCreateAddActorPacket(void);
	virtual void normalTick(void);
	virtual void baseTick(void);
	virtual void passengerTick(void);
	virtual void startRiding(Actor&);
	virtual void addPassenger(Actor&);
	virtual void getExitTip(std::string const&, enum InputMode, enum NewInteractionModel);
	virtual void getEntityLocNameString(void);
	virtual bool isInWall(void);
	virtual bool isInvisible(void);
	virtual bool canShowNameTag(void);
	virtual bool canExistInPeaceful(void);
	virtual void getFormattedNameTag(void);
	virtual void getNameTagTextColor(void);
	virtual void getAlwaysShowNameTag(void);
	virtual void getShadowRadius(void);
	virtual void getHeadLookVector(float);
	virtual bool canInteractWithOtherEntitiesInGame(void);
	virtual void getBrightness(float, enum IConstBlockSource const&);
	virtual void interactPreventDefault(void);
	virtual void playerTouch(Player&);
	virtual bool isImmobile(void);
	virtual bool isSilentObserver(void);
	virtual bool isPickable(void);
	virtual bool isSleeping(void);
	virtual void setSleeping(bool);
	virtual void setSneaking(bool);
	virtual bool isBlocking(void);
	virtual bool isDamageBlocked(enum ActorDamageSource const&);
	virtual bool isAlive(void);
	virtual bool isOnFire(void);
	virtual bool isSurfaceMob(void);
	virtual bool isTargetable(void);
	virtual bool isLocalPlayer(void);
	virtual bool isPlayer(void);
	virtual bool canAttack(Actor*, bool);
	virtual void setTarget(Actor*);
	virtual bool isValidTarget(Actor*);
	virtual void attack(Actor&, enum ActorDamageCause const&);
	virtual void performRangedAttack(Actor&, float);
	virtual void setOwner(ActorUniqueID);
	virtual void setSitting(bool);
	virtual void onTame(void);
	virtual void onFailedTame(void);
	virtual void setStanding(bool);
	virtual bool canPowerJump(void);
	virtual bool isEnchanted(void);
	virtual void shouldRender(void);
	virtual void playAmbientSound(void);
	virtual void getAmbientSound(void);
	virtual bool isInvulnerableTo(ActorDamageSource const&);
	virtual void getBlockDamageCause(Block const&);
	virtual void doFireHurt(int);
	virtual void onLightningHit(void);
	virtual void feed(int);
	virtual void handleEntityEvent(enum ActorEvent, int);
	virtual void getPickRadius(void);
	virtual void getActorRendererId(void);
	virtual void despawn(void);
	virtual void setArmor(enum ArmorSlot, ItemStack const&);
	virtual void getArmorMaterialTypeInSlot(ArmorSlot);
	virtual void getArmorMaterialTextureTypeInSlot(ArmorSlot);
	virtual void getArmorColorInSlot(ArmorSlot, int);
	virtual void setEquippedSlot(enum EquipmentSlot, ItemStack const&);
	virtual void setCarriedItem(ItemStack const&);
	virtual ItemStack* getCarriedItem(void); // getSelectedItem
	virtual void setOffhandSlot(ItemStack*);
	virtual void getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save(class CompoundTag&);
	virtual void load(class CompoundTag const&, enum DataLoadHelper&);
	virtual void queryEntityRenderer(void);
	virtual void getSourceUniqueID(void);
	virtual bool canFreeze(void);
	virtual void getLiquidAABB(MaterialType);
	virtual void handleInsidePortal(enum BlockPos const&);
	virtual void getPortalCooldown(void);
	virtual void getPortalWaitTime(void);
	virtual bool canChangeDimensionsUsingPortal(void);
	virtual void changeDimension(enum AutomaticID); // AutomaticID<Dimension, int>
	virtual void changeDimension(enum ChangeDimensionPacket const&);
	virtual void getControllingPlayer(void);
	virtual void checkFallDamage(float, bool, bool);
	virtual void causeFallDamageToActor(float, float, ActorDamageSource);
	virtual void onSynchedDataUpdate(int);
	virtual bool canAddPassenger(Actor&);
	virtual bool canPickupItem(ItemStack const&);
	virtual bool canBePulledIntoVehicle(void);
	virtual void inCaravan(void);
	virtual void sendMotionPacketIfNeeded(void);
	virtual bool canSynchronizeNewEntity(void);
	virtual void startSwimming(void);
	virtual void stopSwimming(void);
	virtual void buildDebugInfo(std::string&);
	virtual void getCommandPermissionLevel(void);
	virtual int32_t getDeathTime(void);
	virtual bool canBeAffected(enum uint);
	virtual bool canBeAffectedByArrow(MobEffectInstance const&);
	virtual void onEffectAdded(MobEffectInstance&);
	virtual void onEffectUpdated(MobEffectInstance&);
	virtual void onEffectRemoved(MobEffectInstance&);
	virtual bool canObstructSpawningAndBlockPlacement(void);
	virtual void getAnimationComponent(void);
	virtual void openContainerComponent(Player&);
	virtual void swing(void);
	virtual void useItem(ItemStackBase&, enum ItemUseMethod, bool);
	virtual void getDebugText(std::vector<std::string>&);
	virtual void getMapDecorationRotation(void);
	virtual void getPassengerYRotation(Actor const&);
	virtual void add(ItemStack&);
	virtual void drop(ItemStack const&, bool);
	virtual void getInteraction(Player&, enum ActorInteraction&, Vec3<float> const&);
	virtual bool canDestroyBlock(Block const&);
	virtual void setAuxValue(int);
	virtual void startSpinAttack(void);
	virtual void stopSpinAttack(void);
	virtual void renderDebugServerState(class Options const&);
	virtual void getDeletionDelayTimeSeconds(void);
	virtual void kill(void);
	virtual void die(enum ActorDamageSource const&);
	virtual void shouldDropDeathLoot(void);
	virtual void applySnapshot(EntityContext const&, EntityContext const&);
	virtual void getNextStep(float);
	virtual void onPush(Actor&);
	virtual void getLastDeathPos(void);
	virtual void getLastDeathDimension(void);
	virtual bool hasDiedBefore(void);
	virtual void doEnterWaterSplashEffect(void);
	virtual void doExitWaterSplashEffect(void);
	virtual void doWaterSplashEffect(void);
	virtual void _shouldProvideFeedbackOnHandContainerItemSet(enum HandSlot, ItemStack const&);
	virtual void _shouldProvideFeedbackOnArmorSet(ArmorSlot, ItemStack const&);
	virtual void updateEntitySpecificMolangVariables(enum RenderParams&);
	virtual void shouldTryMakeStepSound(void);
	virtual bool canMakeStepSound(void);
	virtual void _hurt(ActorDamageSource const&, float, bool, bool);
	virtual void readAdditionalSaveData(CompoundTag const&, DataLoadHelper&);
	virtual void addAdditionalSaveData(CompoundTag&);
	virtual void _playStepSound(enum BlockPos const&, Block const&);
	virtual void _makeFlySound(void);
	virtual void _doAutoAttackOnTouch(Actor&);
};