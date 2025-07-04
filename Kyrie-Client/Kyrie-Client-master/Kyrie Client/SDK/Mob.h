#pragma once
#include "Actor.h"

class Mob : public Actor {
public:
	virtual void knockback(Actor*, int, float, float, float, float, float);
	virtual __int64 spawnAnim(void);
	virtual __int64 setSprinting(bool);
	virtual __int64 getHurtSound(void);
	virtual __int64 getDeathSound(void);
	virtual float getSpeed(void);
	virtual void setSpeed(float);
	virtual void hurtEffects(ActorDamageSource const&, float, bool, bool);
	virtual float getMeleeWeaponDamageBonus(Mob*);
	virtual float getMeleeKnockbackBonus(void);
	virtual __int64 aiStep(void);
	virtual void pushActors(void);
	virtual void lookAt(Actor*, float, float);
	virtual void checkSpawnRules(bool);
	virtual void checkSpawnObstruction(void);
	virtual __int64 getAttackAnim(float);
	virtual __int64 getItemUseDuration(void);
	virtual float getItemUseStartupProgress(void);
	virtual float getItemUseIntervalProgress(void);
	virtual float getItemUseIntervalAxis(void);
	virtual float getTimeAlongSwing(void);
	virtual __int64 ate(void);
	virtual float getMaxHeadXRot(void);
	virtual bool isAlliedTo(Mob*);
	virtual void doHurtTarget(Actor*, ActorDamageCause const&);
	virtual bool canBeControlledByPassenger(void);
	virtual __int64 leaveCaravan(void);
	virtual __int64 joinCaravan(Mob*);
	virtual bool hasCaravanTail(void);
	virtual __int64 getCaravanHead(void);
	virtual float getArmorValue(void);
	virtual float getArmorCoverPercentage(void);
	virtual float getToughnessValue(void);

};
