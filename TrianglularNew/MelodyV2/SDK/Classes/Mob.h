#pragma once
#include "Actor.h"
#include <bitset>

enum ActorDamageSource;

class Mob : public Actor {
public:
	virtual void knockback(Actor*, int, float, float, float, float, float);
	virtual void spawnAnim(void);
	virtual void setSprinting(bool);
	virtual void getHurtSound(void);
	virtual void getDeathSound(void);
	virtual void getSpeed(void);
	virtual void setSpeed(float);
	virtual void hurtEffects(ActorDamageSource const&, float, bool, bool);
	virtual void aiStep(void);
	virtual void pushActors(void);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual int getItemUseDuration(void);
	virtual void getItemUseStartupProgress(void);
	virtual void getItemUseIntervalProgress(void);
	virtual void getMaxHeadXRot(void);
	virtual bool isAlliedTo(Mob*);
	virtual void doHurtTarget(Actor*, ActorDamageCause const&);
	virtual void getArmorValue(void);
	virtual void hurtArmorSlots(ActorDamageSource const&, int, std::bitset<4ul>);
	virtual void setDamagedArmor(ArmorSlot, ItemStack const&);
	virtual void sendArmorDamage(std::bitset<4ul>);
	virtual void sendArmor(std::bitset<4ul>);
	virtual void getAllHand(void);
	virtual void getAllEquipment(void);
	virtual void dropEquipmentOnDeath(ActorDamageSource const&, int);
	virtual void dropEquipmentOnDeath(void);
	virtual void clearVanishEnchantedItemsOnDeath(void);
	virtual void sendInventory(bool);
	virtual void getDamageAfterEnchantReduction(ActorDamageSource const&, float);
	virtual void createAIGoals(void);
	virtual void onBorn(Actor&, Actor&);
	virtual void setItemSlot(int, ItemStack const&);
	virtual void setTransitioningSitting(bool);
	virtual void getAttackTime(void);
	virtual void _getWalkTargetValue(Vec3<int> const&);
	virtual bool canExistWhenDisallowMob(void);
	virtual void initBodyControl(void);
	virtual void newServerAiStep(void);
	virtual void tickDeath(void);
	virtual void _getAdjustedAABBForSpawnCheck(AABB const&, Vec3<float> const&);
};