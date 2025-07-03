#include "DamageUtil.h"
/* fixed */
float DamageUtils::getExplosionDamage(const Vec3<float>& position, Actor* actor, float extrapolation, int ignoredBlockId) {
	static constexpr float explosionRadius = 12.f;
	static constexpr float armorReductionF = 0.035f;
	static constexpr float maxEpf = 25.f;
	static constexpr float maxPR = 20.f;

	Vec3<float> targetPosition = actor->getHumanPos();
	Vec3<float> predictVal = actor->stateVector->velocity.mul(extrapolation);

	Vec3<float> finalPos{
		targetPosition.x + predictVal.x,
		targetPosition.y + predictVal.y,
		targetPosition.z + predictVal.z
	};

	const float distanceToExplosion = finalPos.dist(position) / explosionRadius;
	if (distanceToExplosion > 1.f) return 0.f;

	float exposure = Game::clientInstance->getRegion()->getSeenPercent(position, actor->aabbShape->aabb);

	if (ignoredBlockId != 0) {
		auto* block = Game::clientInstance->getRegion()->getBlock({
			static_cast<int>(position.x),
			static_cast<int>(position.y),
			static_cast<int>(position.z)
			});
		if (block->blockLegacy->blockId == ignoredBlockId) {
			exposure = 1.f;
		}
		else {
			exposure = Game::clientInstance->getRegion()->getSeenPercent(position, actor->aabbShape->aabb);
		}
	}

	const float explosionImpact = (1 - distanceToExplosion) * exposure;

	float dmg = ((explosionImpact * explosionImpact * 3.5f + explosionImpact * 0.5f * 7.f) * explosionRadius + 1.f);

	int totalArmorPoints = 0;
	float epf = 0.f;

	for (int i = 0; i < 4; i++) {
		ItemStack* armor = actor->getArmor(i);
		if (armor->isValid()) {
			totalArmorPoints += armor->item->getArmorValue();
			epf += 5.f;
		}
	}

	dmg -= dmg * totalArmorPoints * armorReductionF;

	float pr = std::min(std::ceil(std::min(epf, maxEpf) * 0.75f), maxPR);
	dmg -= dmg * pr * armorReductionF;

	return std::max(0.0f, dmg);
}
