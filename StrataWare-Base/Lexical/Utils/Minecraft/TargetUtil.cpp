#include "TargetUtil.h"
#include "../../SDK/Game.h"
#include "../../Client/Client.h"

bool TargetUtil::isTargetValid(Actor* target, bool isMob, float rangeCheck) {
	LocalPlayer* localPlayer = Game::ourPlayer();
	if (localPlayer == nullptr)
		return false;

	if (target == nullptr)
		return false;

	if (target == localPlayer)
		return false;

	if (!target->isAlive())
		return false;

	ActorTypeComponent* actorTypeComponent = target->getActorTypeComponent();
	if (actorTypeComponent == nullptr)
		return false;
	uint32_t entityId = actorTypeComponent->id;

	if (!isMob) {
		if (entityId != 319)
			return false;
		if (target->getNameTag()->length() <= 1 || target->getNameTag()->find("game selector") != std::string::npos) return false;
		if (target->aabbShape->width >= 0.70f && target->aabbShape->width <= 0.80f && target->aabbShape->height >= 2.0f && target->aabbShape->height <= 2.2f) return false;
		if (target->getNameTag()->size() == 0)
			return false;
	}
	else {
		static std::vector<uint32_t> bannedEntityId = {
			64,					// entity.item.name
			69,					// entity.xp_orb.name
			4194372,			// entity.xp_bottle.name
			4194390,			// entity.splash_potion.name
			4194405,			// entity.lingering_potion.name
			95,					// entity.area_effect_cloud.name
			12582992,			// entity.arrow.name
			4194385,			// entity.snowball.name
			4194447,			// entity.wind_charge_projectile.name
			4194393,			// entity.wither_skull.name
			4194395,			// entity.wither_skull_dangerous.name
			72,					// entity.fireworks_rocket.name
			4194391,			// entity.ender_pearl.name
			77,					// entity.fishing_hook.name
			12582985,			// entity.thrown_trident.name
			66,					// entity.falling_block.name
			65,					// entity.tnt.name
		};

		for (uint32_t i : bannedEntityId) {
			if (entityId == i)
				return false;
		}
	}

	if ((localPlayer->getPos().dist(target->getPos())) > rangeCheck)
		return false;

	return true;
}
bool TargetUtil::sortByDist(Actor* a1, Actor* a2) {
	Vec3<float> lpPos = Game::ourPlayer()->getPos();
	return ((a1->getPos().dist(lpPos)) < (a2->getPos().dist(lpPos)));
}