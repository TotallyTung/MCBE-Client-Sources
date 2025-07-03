#include "DamageUtil.h"

float DamageUtils::getExplosionDamage(const Vec3<float>& position, Actor* actor, float extrapolation, int ignoredBlockId) {
	static constexpr float explosionRadius = 12.f;
	static constexpr float armorReductionF = 0.035f;
	static constexpr float maxEpf = 25.f;
	static constexpr float maxPR = 20.f;
	Vec3<float> targetPosition = actor->getHumanPos();
	Vec3<float> predictVal = actor->stateVector->velocity.mul(extrapolation);
	Vec3<float> finalPos = targetPosition.add(predictVal);
	const float distanceToExplosion = finalPos.dist(position) / explosionRadius;
	if (distanceToExplosion > 1.f) return 0.f;
	float exposure = Game::clientInstance->getRegion()->getSeenPercent(position, actor->aabbShape->aabb);
	if (ignoredBlockId != 0) exposure = Game::clientInstance->getRegion()->getBlock((position.x, position.y, position.z))->blockLegacy->blockId == ignoredBlockId ? 1.f : Game::clientInstance->getRegion()->getSeenPercent(position, actor->aabbShape->aabb);
	else exposure = Game::clientInstance->getRegion()->getSeenPercent(position, actor->aabbShape->aabb);
	const float explosionImpact = (1 - distanceToExplosion) * exposure;
	/*
	* PEACEFUL: no damage increase
	* NORMAL: damage / 2 + 1
	* HARD: damage * 3 / 2
	*/
	float dmg = ((explosionImpact * explosionImpact * 3.5f + explosionImpact * 0.5f * 7.f) * explosionRadius + 1.f);
	int totalArmorPoints = 0;
	float epf = 0.f;
	for (int i = 0; i < 4; i++) {
		ItemStack* armor = actor->getArmor(i);
		if (armor->isValid()) {
			totalArmorPoints += armor->item->getArmorValue();
			epf += 5.f; // you cant really get their enchants in bedrock only in java so we assuming ig
		}
	}
	dmg -= dmg * totalArmorPoints * armorReductionF;
	return std::max(0.0f, dmg -= dmg * std::min(ceilf(std::min(epf, maxEpf) * 0.75), maxPR) * armorReductionF);
}
