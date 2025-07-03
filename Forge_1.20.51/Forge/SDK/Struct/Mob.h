#pragma once

class Mob : public Actor {
public:
	virtual void knockback(Actor*, int, float, float, float, float, float);
	virtual void spawnAnim(void);
	virtual void setSprinting(bool);
	virtual void getHurtSound(void);
	virtual void getDeathSound(void);
	virtual void getSpeed(void);
	virtual void setSpeed(float);
	virtual void hurtEffects(float, bool, bool);
	virtual void aiStep(void);
	virtual void pushActors(void);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual int getItemUseDuration(void);
	virtual void getItemUseStartupProgress(void);
	virtual void getItemUseIntervalProgress(void);
	virtual void getMaxHeadXRot(void);
	virtual bool isAlliedTo(Mob*);
	virtual void doHurtTarget(Actor*);
	virtual void getArmorValue(void);
	virtual void hurtArmorSlots(int);
	virtual void setDamagedArmor();
	virtual void sendArmorDamage();
	virtual void sendArmor();
	virtual void getAllHand(void);
	virtual void getAllEquipment(void);
	virtual void dropEquipmentOnDeath(int);
	virtual void dropEquipmentOnDeath(void);
	virtual void clearVanishEnchantedItemsOnDeath(void);
	virtual void sendInventory(bool);
	virtual void getDamageAfterEnchantReduction(float);
	virtual void createAIGoals(void);
	virtual void onBorn(Actor&, Actor&);
	virtual void setItemSlot(int);
	virtual void setTransitioningSitting(bool);
	virtual void getAttackTime(void);
	virtual void _getWalkTargetValue(Vector3<int> const&);
	virtual bool canExistWhenDisallowMob(void);
	virtual void initBodyControl(void);
	virtual void newServerAiStep(void);
	virtual void tickDeath(void);
	virtual void _getAdjustedAABBForSpawnCheck(AABB<float> const&, Vector3<float> const&);
};
