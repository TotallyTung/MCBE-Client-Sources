#pragma once

#include "../../Utils/Utils.h"
#include "DirectPlayerMovementProxy.h"
#include "../Components/AABBShapeComponent.h"
#include "../Components/RenderPositionComponent.h"
#include "../Components/StateVectorComponent.h"
#include "../Components/ActorRotationComponent.h"
#include "Level.h"

// Classes/ Structs
class DirectPlayerMovementProxy;
class EntityRegistry;

struct EntityId {
    uint32_t id;
};

struct EntityContext {
    EntityRegistry* registry;
    EntityId id;
};

class EntityLocation {
public:
	Vec3<float> posPrev;
	Vec3<float> pos;
	Vec3<float> velocity;
};

enum ActorType {

	/* Drops */

	item = 64,
	xp_orb = 69,

	/* Blocks */

	tnt = 65,
	falling_block = 66,
	moving_block = 67,

	/* Immobile and projectiles */

	armor_stand = 61,
	xp_bottle = 68,
	eye_of_ender_signal = 70,
	ender_crystal = 71,
	fireworks_rocket = 72,
	thrown_trident = 73,
	shulker_bullet = 76,
	fishing_hook = 77,
	dragon_fireball = 79,
	arrow = 80,
	snowball = 81,
	egg = 82,
	painting = 83,
	minecart = 84,
	fireball = 85,
	splash_potion = 86,
	ender_pearl = 87,
	leash_knot = 88,
	wither_skull = 89,
	boat = 90,
	wither_skull_dangerous = 91,
	lightning_bolt = 93,
	small_fireball = 94,
	area_effect_cloud = 95,
	hopper_minecart = 96,
	tnt_minecart = 97,
	chest_minecart = 98,
	command_block_minecart = 100,
	lingering_potion = 101,
	llama_spit = 102,
	evocation_fang = 103,
	ice_bomb = 106,
	balloon = 107,
	chest_boat = 218,

	/* Hostile mobs */

	zombie = 32,
	creeper = 33,
	skeleton = 34,
	spider = 35,
	zombie_pigman = 36,
	slime = 37,
	enderman = 38,
	silverfish = 39,
	cave_spider = 40,
	ghast = 41,
	magma_cube = 42,
	blaze = 43,
	zombie_villager = 44,
	witch = 45,
	stray = 46,
	husk = 47,
	wither_skeleton = 48,
	guardian = 49,
	elder_guardian = 50,
	wither = 52,
	ender_dragon = 53,
	shulker = 54,
	endermite = 55,
	vindicator = 57,
	phantom = 58,
	ravager = 59,
	evocation_illager = 104,
	vex = 105,
	drowned = 110,
	pillager = 114,
	zombie_villager_v2 = 116,
	piglin = 123,
	hoglin = 124,
	zoglin = 126,
	piglin_brute = 127,
	warden = 131,

	/* Passive and neutral mobs */

	chicken = 10,
	cow = 11,
	pig = 12,
	sheep = 13,
	wolf = 14,
	villager = 15,
	mooshroom = 16,
	squid = 17,
	rabbit = 18,
	bat = 19,
	iron_golem = 20,
	snow_golem = 21,
	ocelot = 22,
	horse = 23,
	donkey = 24,
	mule = 25,
	skeleton_horse = 26,
	zombie_horse = 27,
	polar_bear = 28,
	llama = 29,
	parrot = 30,
	dolphin = 31,
	turtle = 74,
	cat = 75,
	pufferfish = 108,
	salmon = 109,
	tropicalfish = 111,
	cod = 112,
	panda = 113,
	villager_v2 = 115,
	wandering_trader = 118,
	fox = 121,
	bee = 122,
	strider = 125,
	goat = 128,
	frog = 132,
	tadpole = 133,
	allay = 134,

	/* Other */

	player = 63,
	shield = 117,

	/* Unused */

	elder_guardian_ghost = 120,

	/* Education Edition */

	npc = 51,
	agent = 56,
	tripod_camera = 62,
	chalkboard = 78

};

class Actor { //actor and mob vtable
public:
	virtual int getStatusFlag(int ActorFlags);
	virtual void setStatusFlag(int ActorFlags, bool);
	virtual int getLastHurtByMob(void);
	virtual void setLastHurtByMob(Actor*);
	virtual int getLastHurtByPlayer(void);
	virtual void setLastHurtByPlayer(Actor*);
	virtual int getLastHurtMob(void);
	virtual void setLastHurtMob(Actor*);
	virtual void outOfWorld(void);
	virtual void reloadHardcoded(unsigned __int64 ActorInitializationMethod, unsigned __int64 VariantParameterList);
	virtual void reloadHardcodedClient(unsigned __int64 ActorInitializationMethod, unsigned __int64 VariantParameterList);
	virtual void initializeComponents(unsigned __int64 ActorInitializationMethod, unsigned __int64 VariantParameterList);
	virtual void reloadComponents(unsigned __int64 ActorInitializationMethod, unsigned __int64 VariantParameterList);
	virtual void _serverInitItemStackIds(void);
	virtual void _doInitialMove(void);
	virtual void hasComponent(__int64 const&);
	virtual ~Actor();
	virtual void resetUserPos(bool);
	virtual int getOwnerEntityType(void);
	virtual void remove(void);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual int getPredictedMovementValues(void);
	virtual Vec3<float>* getPosition(void);
	virtual Vec3<float>* getPosPrev(void);
	virtual float getPosExtrapolated(float);
	virtual int getFiringPos(void);
	virtual float getInterpolatedRidingPosition(float);
	virtual float getInterpolatedBodyRot(float);
	virtual float getInterpolatedHeadRot(float);
	virtual float getInterpolatedBodyYaw(float);
	virtual int getYawSpeedInDegreesPerSecond(void);
	virtual float getInterpolatedWalkAnimSpeed(float);
	virtual float getInterpolatedRidingOffset(float, int);
	virtual void resetInterpolated(void);
	virtual bool isFireImmune(void);
	virtual void breaksFallingBlocks(void);
	virtual void blockedByShield(unsigned __int64 actorDamageSource, Actor&);
	virtual void canDisableShield(void);
	virtual void teleportTo(Vec3<float> const&, bool, int, int, bool);
	virtual void tryTeleportTo(Vec3<float> const&, bool, bool, int, int);
	virtual void chorusFruitTeleport(Vec3<float> const&);
	virtual void lerpMotion(Vec3<float> const&);
	virtual void tryCreateAddActorPacket(void);
	virtual void normalTick(void);
	virtual void baseTick(void);
	virtual void passengerTick(void);
	virtual void positionPassenger(Actor&, float);
	virtual bool startRiding(Actor&);
	virtual void addPassenger(Actor&);
	virtual int getExitTip(std::string const&, int InputMode, int NewInteractionModel);
	virtual int getEntityLocNameString(void);
	virtual void intersects(Vec3<float> const&, Vec3<float> const&);
	virtual bool isInWall(void);
	virtual bool isInvisible(void);
	virtual void canShowNameTag(void);
	virtual void canExistInPeaceful(void);
	virtual void setNameTagVisible(bool);
	virtual std::string getNameTag(void);
	virtual int getNameTagAsHash(void);
	virtual std::string getFormattedNameTag(void);
	virtual void filterFormattedNameTag(unsigned __int64 UIProfanityContext);
	virtual void setNameTag(std::string const&);
	virtual int getAlwaysShowNameTag(void);
	virtual void setScoreTag(std::string const&);
	virtual int getScoreTag(void);
	virtual bool isInWater(void);
	virtual bool isUnderLiquid(int MaterialType);
	virtual bool isOverWater(void);
	virtual int getShadowHeightOffs(void);
	virtual int getShadowRadius(void);
	virtual float getHeadLookVector(float);
	virtual void canSeeInvisible(void);
	virtual void canSee(Actor const&);
	virtual void canSee(Vec3<float> const&);
	virtual void canInteractWithOtherEntitiesInGame(void);
	virtual bool isSkyLit(float);
	virtual float getBrightness(float, class BlockSource*);
	virtual void interactPreventDefault(void);
	virtual void playerTouch(Actor&);
	virtual bool isImmobile(void);
	virtual bool isSilent(void);
	virtual bool isSilentObserver(void);
	virtual bool isPickable(void);
	virtual bool isFishable(void);
	virtual bool isSleeping(void);
	virtual bool setSleeping(bool);
	virtual bool isShootable(void);
	virtual void setSneaking(bool);
	virtual bool isBlocking(void);
	virtual bool isDamageBlocked(unsigned __int64 actorDamageSource);
	virtual bool isAlive(void);
	virtual bool isOnFire(void);
	virtual bool isOnHotBlock(void);
	virtual bool isCreativeModeAllowed(void);
	virtual bool isSurfaceMob(void);
	virtual bool isTargetable(void);
	virtual bool isLocalPlayer(void);
	virtual bool isRemotePlayer(void);
	virtual bool isPlayer(void);
	virtual bool isAffectedByWaterBottle(void);
	virtual void canAttack(Actor*, bool);
	virtual void setTarget(Actor*);
	virtual bool isValidTarget(Actor*);
	virtual void attack(Actor&, unsigned __int64 actorDamageCause);
	virtual void performRangedAttack(Actor&, float);
	virtual int getEquipmentCount(void);
	virtual void setOwner(unsigned int64_t);
	virtual void setSitting(bool);
	virtual void onTame(void);
	virtual void onFailedTame(void);
	virtual int getInventorySize(void);
	virtual int getEquipSlots(void);
	virtual int getChestSlots(void);
	virtual void setStanding(bool);
	virtual void canPowerJump(void);
	virtual void setCanPowerJump(bool);
	virtual bool isEnchanted(void);
	virtual void shouldRender(void);
	virtual void playAmbientSound(void);
	virtual int getAmbientSound(void);
	virtual bool isInvulnerableTo(unsigned __int64 actorDamageSource);
	virtual int getBlockDamageCause(Block const&);
	virtual void animateHurt(void);
	virtual void doFireHurt(int);
	virtual void onLightningHit(void);
	virtual void onBounceStarted(__int64 const&, Block const&);
	virtual void feed(int);
	virtual void handleEntityEvent(unsigned __int64 ActorEvent, int);
	virtual int getPickRadius(void);
	virtual int getActorRendererId(void);
	virtual void spawnAtLocation(int, int);
	virtual void spawnAtLocation(int, int, float);
	virtual void spawnAtLocation(__int64 const&, float);
	virtual void despawn(void);
	virtual void killed(Actor&);
	virtual void awardKillScore(Actor&, int);
	virtual void setArmor(__int64, __int64 const&);
	virtual int getArmor(__int64);
	virtual int getAllArmor(void);
	virtual int something();
	virtual int getArmorMaterialTextureTypeInSlot(__int64);
	virtual int getArmorColorInSlot(__int64, int);
	virtual int getEquippedSlot(int EquipmentSlot);
	virtual void setEquippedSlot(int EquipmentSlot, __int64 const&);
	virtual void setCarriedItem(__int64 const&);
	virtual int getCarriedItem(void);
	virtual void setOffhandSlot(__int64 const&);
	virtual int getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save(__int64&);
	virtual void saveWithoutId(__int64&);
	virtual void load();
	virtual void loadLinks();
	virtual int getEntityTypeId(void);
	virtual void queryEntityRenderer(void);
	virtual int getSourceUniqueID(void);
	virtual void thawFreezeEffect(void);
	virtual void canFreeze(void);
	virtual bool isWearingLeatherArmor(void);
	virtual int getLiquidAABB(int MaterialType);
	virtual void handleInsidePortal(__int64 const&);
	virtual int getPortalCooldown(void);
	virtual int getPortalWaitTime(void);
	virtual void canChangeDimensionsUsingPortal(void);
	virtual void changeDimension();
	virtual void changeDimension(__int64 const&);
	virtual int getControllingPlayer(void);
	virtual void checkFallDamage(float, bool);
	virtual void causeFallDamage(float, float, unsigned __int64 ActorDamageSource);
	virtual void handleFallDistanceOnServer(float, float, bool);
	virtual void playSynchronizedSound(unsigned __int64 LevelSoundEvent, Vec3<float> const&, Block const&, bool);
	virtual void playSynchronizedSound(unsigned __int64 LevelSoundEvent, Vec3<float> const&, int, bool);
	virtual void onSynchedFlagUpdate(int, long, long);
	virtual void onSynchedDataUpdate(int);
	virtual void canAddPassenger(Actor&);
	virtual void canPickupItem(__int64 const&);
	virtual void canBePulledIntoVehicle(void);
	virtual void inCaravan(void);
	virtual bool isLeashableType(void);
	virtual void tickLeash(void);
	virtual void sendMotionPacketIfNeeded(void);
	virtual void canSynchronizeNewEntity(void);
	virtual void startSwimming(void);
	virtual void stopSwimming(void);
	virtual void buildDebugInfo(std::string&);
	virtual int getCommandPermissionLevel(void);
	virtual bool isClientSide(void);
	virtual int getMutableAttribute(__int64 const&);
	virtual int getAttribute(__int64 const&);
	virtual int getDeathTime(void);
	virtual void heal(int);
	virtual bool isInvertedHealAndHarm(void);
	virtual void canBeAffected(unsigned int);
	virtual void canBeAffectedByArrow(__int64 const&);
	virtual void onEffectAdded(__int64&);
	virtual void onEffectUpdated(__int64&);
	virtual void onEffectRemoved(__int64&);
	virtual void canObstructSpawningAndBlockPlacement(void);
	virtual int getAnimationComponent(void);
	virtual void openContainerComponent(Actor&);
	virtual void swing(void);
	virtual void useItem(__int64&, unsigned __int64 ItemUseMethod, bool);
	virtual void hasOutputSignal();
	virtual int getOutputSignal(void);
	virtual int getDebugText(std::vector<std::string>&);
	virtual int getMapDecorationRotation(void);
	virtual int getPassengerYRotation(Actor const&);
	virtual void add(__int64&);
	virtual void drop(__int64 const&, bool);
	virtual int getInteraction(Actor&, __int64&, Vec3<float> const&);
	virtual void canDestroyBlock(Block const&);
	virtual void setAuxValue(int);
	virtual void setSize(float, float);
	virtual void onOrphan(void);
	virtual void wobble(void);
	virtual void wasHurt(void);
	virtual void startSpinAttack(void);
	virtual void stopSpinAttack(void);
	virtual void setDamageNearbyMobs(bool);
	virtual void renderDebugServerState(__int64 const&);
	virtual void reloadLootTable(void);
	virtual void reloadLootTable(__int64 const&);
	virtual int getDeletionDelayTimeSeconds(void);
	virtual void kill(void);
	virtual void die(unsigned __int64 actorDamageSource);
	virtual void shouldDropDeathLoot(void);
	virtual void shouldTick(void);
	virtual void applySnapshot(EntityContext const&, EntityContext const&);
	virtual float getNextStep(float);
	virtual int getLootTable(void);
	virtual void onPush(Actor&);
	virtual int getLastDeathPos(void);
	virtual int getLastDeathDimension(void);
	virtual void hasDiedBefore(void);
	virtual void doWaterSplashEffect(void);
	virtual void _shouldProvideFeedbackOnHandContainerItemSet(__int64, __int64 const&);
	virtual void _shouldProvideFeedbackOnArmorSet(__int64, __int64 const&);
	virtual void updateEntitySpecificMolangVariables(__int64&);
	virtual void shouldTryMakeStepSound(void);
	virtual void canMakeStepSound(void);
	virtual void _hurt(unsigned __int64 actorDamageSource, float, bool, bool);
	virtual void markHurt(void);
	virtual void _getAnimationComponent();
	virtual void readAdditionalSaveData();
	virtual void addAdditionalSaveData2(__int64&);
	virtual void _playStepSound(__int64 const&, Block const&);
	virtual void _playFlySound(__int64 const&, Block const&);
	virtual void _makeFlySound(void);
	virtual void pushOutOfBlocks(Vec3<float> const&);
	virtual void spawnTrailBubbles(void);
	virtual void updateInsideBlock(void);
	virtual void _removePassenger();
	virtual void _onSizeUpdated(void);
	virtual void _doAutoAttackOnTouch(Actor&);
	// Mob vtable start
	virtual void knockback(Actor*, int, float, float, float, float, float);
	virtual void spawnAnim(void);
	virtual void setSprinting(bool);
	virtual int getHurtSound(void);
	virtual int getDeathSound(void);
	virtual int getSpeed(void);
	virtual void setSpeed(float);
	virtual void hurtEffects(unsigned __int64 actorDamageSource, float, bool, bool);
	virtual int getMeleeWeaponDamageBonus(Actor);
	virtual int getMeleeKnockbackBonus(void);
	virtual void aiStep(void);
	virtual void aiStep(__int64&);
	virtual void pushActors(void);
	virtual void lookAt(Actor*, float, float);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual float getAttackAnim(float);
	virtual int getItemUseDuration(void);
	virtual int getItemUseStartupProgress(void);
	virtual int getItemUseIntervalProgress(void);
	virtual int getItemUseIntervalAxis(void);
	virtual int getTimeAlongSwing(void);
	virtual void ate(void);
	virtual int getMaxHeadXRot(void);
	virtual bool isAlliedTo(Actor);
	virtual void doHurtTarget(Actor*, unsigned __int64 actorDamageCause);
	virtual void canBeControlledByPassenger(void);
	virtual void leaveCaravan(void);
	virtual void joinCaravan(Actor);
	virtual void hasCaravanTail(void);
	virtual int getCaravanHead(void);
	virtual int getArmorValue(void);
	virtual int getArmorCoverPercentage(void);
	virtual int getToughnessValue(void);
	virtual void hurtArmorSlots();
	virtual void setDamagedArmor(__int64, __int64 const&);
	virtual void sendArmorDamage(__int64);
	virtual void sendArmor(__int64);
	virtual void containerChanged(int);
	virtual void updateEquipment(void);
	virtual void clearEquipment(void);
	virtual int getAllArmorID(void);
	virtual int getAllHand(void);
	virtual int getAllEquipment(void);
	virtual int getArmorTypeHash(void);
	virtual void dropEquipmentOnDeath(unsigned __int64 actorDamageSource, int);
	virtual void dropEquipmentOnDeath(void);
	virtual void clearVanishEnchantedItemsOnDeath(void);
	virtual void sendInventory(bool);
	virtual float getDamageAfterEnchantReduction(unsigned __int64 actorDamageSource, float);
	virtual float getDamageAfterArmorReduction(unsigned __int64 actorDamageSource, float);
	virtual float getDamageAfterResistanceEffect(unsigned __int64 actorDamageSource, float);
	virtual void createAIGoals(void);
	virtual void onBorn(Actor&, Actor&);
	virtual void setItemSlot(int EquipmentSlot, __int64 const&);
	virtual void setTransitioningSitting(bool);
	virtual void attackAnimation(Actor*, float);
	virtual int getAttackTime(void);
	virtual void _getWalkTargetValue(Vec3<int> const&);
	virtual void canExistWhenDisallowMob(void);
	virtual void ascendBlockByJumping(void);
	virtual void descendBlockByCrouching(void);
	virtual void dropContainer(void);
	virtual void initBodyControl(void);
	virtual void newServerAiStep(void);
	virtual void _serverAiMobStep(void);
	virtual void dropBags(void);
	virtual void tickDeath(void);
	virtual void updateGliding(void);
	virtual void _allowAscendingScaffolding(void);
	virtual void _getAdjustedAABBForSpawnCheck(AABB const&, Vec3<float> const&);
public:
    DirectPlayerMovementProxy* getMovementProxy() {
        using MovementProxyFunction = void(__fastcall*)(Actor*, std::shared_ptr<void>*);
        static MovementProxyFunction originalFunction = (MovementProxyFunction)Utils::findSig("40 53 48 83 ec ? 48 8b da 48 8b d1 48 89 5c 24 ? 48 8b 41");
        std::shared_ptr<void> movementProxyPointer;
        originalFunction(this, &movementProxyPointer);
        return (DirectPlayerMovementProxy*)movementProxyPointer.get();
    }
	StateVectorComponent* getStateVectorComponent() {
		using getStateVectorComponent = StateVectorComponent * (__cdecl*)(void*, EntityId*);
		static auto func = reinterpret_cast<getStateVectorComponent>(Utils::findSig("40 53 48 83 EC 20 48 8B DA BA 91 3C C9 0E"));
		auto registryBase = *reinterpret_cast<void**>(this->entityContext.registry);
		auto id = this->entityContext.id;
		return func(registryBase, &id);
	}
	RenderPositionComponent* getRenderPositionComponent() {
		using getRenderPositionComponent = RenderPositionComponent * (__cdecl*)(void*, EntityId*);
		static auto func = reinterpret_cast<getRenderPositionComponent>(Utils::findSig("40 53 48 83 EC ? 48 8B DA BA 6E F3 E8 D4"));
		auto registryBase = *reinterpret_cast<void**>(this->entityContext.registry);
		auto id = this->entityContext.id;
		return func(registryBase, &id);
	}
	AABBShapeComponent* getAABBShapeComponent() {
		using getAABBShapeComponent = AABBShapeComponent * (__cdecl*)(void*, EntityId*);
		static auto func = reinterpret_cast<getAABBShapeComponent>(Utils::findSig("40 53 48 83 EC ? 48 8B DA BA F2 C9 10 1B"));
		auto registryBase = *reinterpret_cast<void**>(this->entityContext.registry);
		auto id = this->entityContext.id;
		return func(registryBase, &id);
	}
public:
    BUILD_ACCESS(struct EntityContext, entityContext, 0x8);
	BUILD_ACCESS(class EntityLocation*, location, 0x2A0);
	BUILD_ACCESS(Level*, level, 0x260);
};