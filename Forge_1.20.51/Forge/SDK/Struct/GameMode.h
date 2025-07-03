#pragma once
//class AABB;
class MobEffectInstance;
class CompoundTag;
class ItemStack;
class Block; // Today I want to learn how to find any sigs. That is hard for me>< <ye>
class InputMode;
class Entity;

class MobEntityList {
public:
private:
	char pad_0x0000[0x38];  //0x0000
public:
	uintptr_t firstEntity;  //0x0038
	uintptr_t lastEntity;   //0x0040
	uintptr_t endAddr;      //0x0048

	inline size_t getListSize() {
		return (lastEntity - firstEntity) / sizeof(uintptr_t);
	}

	inline Entity* get(size_t idx) {
		if (idx >= getListSize())
			return 0;
		return *reinterpret_cast<Entity**>(firstEntity + (idx * sizeof(uintptr_t)));
	}

	Entity* operator[](size_t idx) {
		return get(idx);
	}
};  //Size=0x0050

class Actor {
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
	virtual bool hasComponent();
	virtual void resetUserPos(bool);
	virtual void getOwnerEntityType(void);
	virtual void remove(void);
	virtual bool isRuntimePredictedMovementEnabled(void);
	virtual Vector3<float> getFiringPos(void);
	virtual float getInterpolatedBodyRot(float); //16
	virtual float getInterpolatedHeadRot(float); //17 HeadYaw
	virtual float getInterpolatedBodyYaw(float); //18
	virtual float getYawSpeedInDegreesPerSecond(void);
	virtual Vector3<float> getInterpolatedRidingOffset(float, int);
	virtual void resetInterpolated(void);
	virtual bool isFireImmune(void);
	virtual void breaksFallingBlocks(void);
	virtual void blockedByShield(enum ActorDamageSource const&, Actor&);
	virtual bool canDisableShield(void);
	virtual void teleportTo(Vector3<float> const&, bool, int, int, bool);
	virtual void lerpMotion(Vector3<float> const&);
	virtual void tryCreateAddActorPacket(void);
	virtual void normalTick(void);
	virtual void baseTick(void); //30
	virtual void passengerTick(void);
	virtual void startRiding(Actor&);
	virtual void addPassenger(Actor&);
	virtual void getExitTip(std::string const&, InputMode, enum NewInteractionModel);
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
	virtual void playerTouch();
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
	virtual void attack(Actor&, void*);
	virtual void performRangedAttack(Actor&, float);
	virtual void setOwner(void);
	virtual void setSitting(bool);
	virtual void onTame(void);
	virtual void onFailedTame(void);
	virtual void setStanding(bool);
	virtual bool canPowerJump(void);
	virtual bool isEnchanted(void);
	virtual void shouldRender(void);
	virtual void playAmbientSound(void);
	virtual void getAmbientSound(void);
	virtual bool isInvulnerableTo(void);
	virtual void getBlockDamageCause(void*);
	virtual void doFireHurt(int);
	virtual void onLightningHit(void);
	virtual void feed(int);
	virtual void handleEntityEvent(void*, int);
	virtual void getPickRadius(void);
	virtual void getActorRendererId(void);
	virtual void despawn(void);
	virtual void setArmor(enum ArmorSlot, void*); //ItemStack
	virtual void getArmorMaterialTypeInSlot(ArmorSlot);
	virtual void getArmorMaterialTextureTypeInSlot(ArmorSlot);
	virtual void getArmorColorInSlot(ArmorSlot, int);
	virtual void setEquippedSlot(enum EquipmentSlot, void*);
	virtual void setCarriedItem(void*);
	virtual void* getCarriedItem(void); // getSelectedItem
	virtual void setOffhandSlot(void*);
	virtual void getEquippedTotem(void);
	virtual void consumeTotem(void);
	virtual void save();
	virtual void load();
	virtual void queryEntityRenderer(void);
	virtual void getSourceUniqueID(void);
	virtual bool canFreeze(void);
	virtual void getLiquidAABB(void*);
	virtual void handleInsidePortal();
	virtual void getPortalCooldown(void);
	virtual void getPortalWaitTime(void);
	virtual bool canChangeDimensionsUsingPortal(void);
	virtual void changeDimension(); // AutomaticID<Dimension, int>
	virtual void changeDimension(void*, void*);
	virtual void getControllingPlayer(void);
	virtual void checkFallDamage(float, bool, bool);
	virtual void causeFallDamageToActor(float, float);
	virtual void onSynchedDataUpdate(int);
	virtual bool canAddPassenger(Actor&);
	virtual bool canPickupItem(void*);
	virtual bool canBePulledIntoVehicle(void);
	virtual void inCaravan(void);
	virtual void sendMotionPacketIfNeeded(void);
	virtual bool canSynchronizeNewEntity(void);
	virtual void startSwimming(void);
	virtual void stopSwimming(void);
	virtual void buildDebugInfo(std::string&);
	virtual void getCommandPermissionLevel(void);
	virtual void getDeathTime(void);
	virtual bool canBeAffected(enum uint);
	virtual bool canBeAffectedByArrow(void*);
	virtual void onEffectAdded(void*);
	virtual void onEffectUpdated(void*);
	virtual void onEffectRemoved(void*);
	virtual bool canObstructSpawningAndBlockPlacement(void);
	virtual void getAnimationComponent(void);
	virtual void openContainerComponent();
	virtual void swing(void);
	virtual void useItem(void*, bool);
	virtual void getDebugText(std::vector<std::string>&);
	virtual void getMapDecorationRotation(void);
	virtual void getPassengerYRotation(Actor const&);
	virtual void add(void*);
	virtual void drop(void*);
	virtual void getInteraction(Vector3<float> const&);
	virtual bool canDestroyBlock(Block const&);
	virtual void setAuxValue(int);
	virtual void startSpinAttack(void);
	virtual void stopSpinAttack(void);
	virtual void renderDebugServerState();
	virtual void getDeletionDelayTimeSeconds(void);
	virtual void kill(void);
	virtual void die();
	virtual void shouldDropDeathLoot(void);
	virtual void applySnapshot();
	virtual void getNextStep(float);
	virtual void onPush(Actor&);
	virtual void getLastDeathPos(void);
	virtual void getLastDeathDimension(void);
	virtual bool hasDiedBefore(void);
	virtual void doEnterWaterSplashEffect(void);
	virtual void doExitWaterSplashEffect(void);
	virtual void doWaterSplashEffect(void);
	virtual void _shouldProvideFeedbackOnHandContainerItemSet(void*);
	virtual void _shouldProvideFeedbackOnArmorSet(void*);
	virtual void updateEntitySpecificMolangVariables();
	virtual void shouldTryMakeStepSound(void);
	virtual bool canMakeStepSound(void);
	virtual void _hurt(float, bool, bool);
	virtual void readAdditionalSaveData();
	virtual void addAdditionalSaveData();
	virtual void _playStepSound();
	virtual void _makeFlySound(void);
	virtual void _doAutoAttackOnTouch(Actor&);
public:
	Vector3<float>* getPos() {
		Vector3<float>* playerpos = (Vector3<float>*)((__int64*)this + 0x59);
		return playerpos;
	}

	Vector2<float>* getRotation() {
		__int64* v2 = (__int64*)*((__int64*)this + 0x5B);
		if (v2 == nullptr)
			return nullptr;
		return (Vector2<float>*)v2;
	}

	void DisplayClientMessage(const char* msg)
	{
		uintptr_t DisplayClientMessageAddr = findSig(xorstr_("48 89 5C 24 08 48 89 74 24 10 57 48 83 EC 40 48 8B FA 48 8B 89"), xorstr_("DisplayClientMessage"));
		TextHolder holder = TextHolder(msg);
		CallFunc<void*, Actor*, TextHolder*>(DisplayClientMessageAddr, this, &holder);
	}
public:
	BUILD_ACCESS(this, StateVectorComponent*, stateVector, 0x2C8);
};

class GameMode {
private:
	virtual void Constructor();
public:
	virtual void startDestroyBlock(Vector3<int> const&, unsigned char, bool&);
	virtual void destroyBlock(Vector3<int> const&, unsigned char);
	virtual void continueDestroyBlock(Vector3<int> const&, unsigned char, Vector3<float> const&, bool&);
	virtual void stopDestroyBlock(Vector3<int> const&);
	virtual void startBuildBlock(Vector3<int> const&, unsigned char);
	virtual void buildBlock(Vector3<int> const&, unsigned char, bool);
	virtual void continueBuildBlock(Vector3<int> const&, unsigned char);
	virtual void stopBuildBlock(void);
private:
	virtual void tick(void);
	virtual int getPickRange(InputMode const&, bool);
	virtual void useItem(ItemStack&);
	virtual void useItemOn(ItemStack&, Vector3<int> const&, unsigned char, Vector3<float> const&, Block const*);
	virtual void interact(Actor&, Vector3<float> const&);
public:
	virtual void attack(Actor&);
private:
	virtual void releaseUsingItem(void);
	virtual void setTrialMode(bool);
	virtual bool isInTrialMode(void);
	virtual void registerUpsellScreenCallback();
public:
	Actor* player;
	void baseUseItem(ItemStack& stack) {
		using baseUseItem = void(*)(GameMode*, ItemStack&);
		static baseUseItem baseUseItemFunc = reinterpret_cast<baseUseItem>(findSig("48 89 5C 24 10 48 89 74 24 18 55 57 41 56 48 8D 6C 24 90 48 81 EC 70 01 00 00 48 8B 05 ?? ?? ?? ?? 48 33 C4 48 89 45 60 48 8B F2 48 8B F9 45 33 F6 4C 89 75 08 48 8D 4D 08 E8 ?? ?? ?? ?? 90 48"));
		return baseUseItemFunc(this, stack);
	}
};
