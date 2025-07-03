#include "BedAura.h"
#include "../../../../Client.h"

BedAura::BedAura() : Module("BedAura", "Calculates and renders optimal bed placements.", Category::COMBAT) {
    addSlider<float>("Target Range", "The range at which targets can be selected.", ValueType::FLOAT_T, &targetRange, 0.f, 10.f);
    addSlider<float>("Min Damage", "The minimum damage to inflict on your target.", ValueType::FLOAT_T, &minDamage, 0.f, 20.f);
    addSlider<float>("Max Self Damage", "The maximum damage to inflict on yourself.", ValueType::FLOAT_T, &maxSelfDamage, 0.f, 20.f);
    addBoolCheck("Target Mobs", "Allows targeting mobs in addition to players.", &targetMobs);
    addBoolCheck("Render", "Renders the block where it would place a bed.", &render);
    addColorPicker("Side Color", "The side color for positions to be placed.", &sideColor);
    addColorPicker("Line Color", "The line color for positions to be placed.", &lineColor);
}

void BedAura::onEnable() {
    bestPlacePos = Vec3<int>(0, 0, 0);
    target = nullptr;
}

void BedAura::onDisable() {
    bestPlacePos = Vec3<int>(0, 0, 0);
    target = nullptr;
}

void BedAura::onNormalTick(Actor* actor) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer || localPlayer->dimension->id == 0) {
        setEnabled(false);
        return;
    }

    target = getClosestTarget(targetRange);
    if (!target) {
        bestPlacePos = Vec3<int>(0, 0, 0);
        return;
    }

    findBestPlacement();
}

void BedAura::findBestPlacement() {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    Vec3<int> targetPos = target->stateVectorComponent->pos.toInt();
    float bestDamage = 0.f;
    bestPlacePos = Vec3<int>(0, 0, 0);

    for (int y = -1; y <= 1; y++) {
        for (int x = -2; x <= 2; x++) {
            for (int z = -2; z <= 2; z++) {
                Vec3<int> bedPos = targetPos.add(Vec3<int>(x, y, z));
                if (!canPlaceBed(bedPos)) continue;

                for (CardinalDirection dir : {CardinalDirection::North, CardinalDirection::South, CardinalDirection::East, CardinalDirection::West}) {
                    Vec3<int> bedHead = bedPos;
                    switch (dir) {
                    case CardinalDirection::North: bedHead = bedPos.add(Vec3<int>(0, 0, -1)); break;
                    case CardinalDirection::South: bedHead = bedPos.add(Vec3<int>(0, 0, 1)); break;
                    case CardinalDirection::East: bedHead = bedPos.add(Vec3<int>(1, 0, 0)); break;
                    case CardinalDirection::West: bedHead = bedPos.add(Vec3<int>(-1, 0, 0)); break;
                    }

                    if (!canPlaceBed(bedHead)) continue;

                    float targetDamage = calculateBedDamage(target, bedPos.toFloat());
                    float selfDamage = calculateBedDamage(localPlayer, bedPos.toFloat());

                    if (targetDamage >= minDamage && selfDamage < maxSelfDamage && targetDamage > bestDamage) {
                        bestDamage = targetDamage;
                        bestPlacePos = bedPos;
                        bestDirection = dir;
                    }
                }
            }
        }
    }
}

bool BedAura::canPlaceBed(const Vec3<int>& pos) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    BlockSource* region = localPlayer->dimension->blockSource;
    Block* block = region->getBlock(pos);

    if (block == nullptr || block->blockLegacy == nullptr) return false;

    Vec3<float> eyePos = localPlayer->getEyePos();
    Vec3<float> blockPos = Vec3<float>(pos.x + 0.5f, pos.y + 0.5f, pos.z + 0.5f);
    if (eyePos.dist(blockPos) > 5.0f) return false;

    return true;
}

float BedAura::calculateBedDamage(Actor* target, const Vec3<float>& bedPos) {
    constexpr float explosionRadius = 10.f;
    const float distPercent = computeDistancePercentage(bedPos, target, explosionRadius);
    if (distPercent > 1.0f) return 0.0f;

    const float impact = computeImpact(bedPos, target, distPercent);
    float damage = calculateBaseDamage(impact, explosionRadius);

    const auto [armorPoints, epf] = getArmorDetails(target);
    damage = applyArmorReduction(damage, armorPoints, epf);

    return std::max(0.0f, damage);
}

float BedAura::computeDistancePercentage(const Vec3<float>& bedPos, Actor* target, float explosionRadius) {
    const Vec3<float> predictedPos = target->getHumanPos().add(target->stateVectorComponent->velocity);
    return std::clamp(predictedPos.distanceTo(bedPos) / explosionRadius, 0.0f, 1.0f);
}

float BedAura::computeImpact(const Vec3<float>& bedPos, Actor* target, float distPercent) {
    return (1.0f - distPercent) * mc.getLocalPlayer()->dimension->blockSource->getSeenPercent(bedPos, *target->getAABB());
}

float BedAura::calculateBaseDamage(float impact, float explosionRadius) {
    return ((impact * impact * 3.5f + impact * 0.5f * 7.0f) * explosionRadius + 1.0f);
}

std::pair<int, float> BedAura::getArmorDetails(Actor* target) {
    int armorPoints = 0;
    float epf = 0.0f;

    for (int i = 0; i < 4; i++) {
        auto* armor = target->getArmor(i);
        if (armor->isValid()) {
            armorPoints += armor->getItemPtr()->getArmorValue();
            epf += 5.0f;
        }
    }

    return { armorPoints, epf };
}

float BedAura::applyArmorReduction(float damage, int armorPoints, float epf) {
    constexpr float armorReductionFactor = 0.035f;
    constexpr float maxEpf = 25.0f;
    constexpr float epfFactor = 0.75f;
    constexpr float maxEpfCap = 20.0f;

    damage -= damage * armorPoints * armorReductionFactor;
    damage -= damage * std::min(ceilf(std::min(epf, maxEpf) * epfFactor), maxEpfCap) * armorReductionFactor;

    return damage;
}

Actor* BedAura::getClosestTarget(float range) {
    LocalPlayer* localPlayer = mc.getLocalPlayer();
    if (!localPlayer) return nullptr;

    Player* closest = nullptr;
    float closestDistSq = range * range;

    auto* level = localPlayer->getLevel();
    if (!level) return nullptr;

    for (Actor* actor : level->getRuntimeActorList()) {
        if (!actor->isPlayer() || actor == localPlayer) continue;

        Player* player = reinterpret_cast<Player*>(actor);

        if (!TargetUtils::isTargetValid(player, targetMobs)) continue;

        Vec3<float> playerPos = player->stateVectorComponent->pos;
        Vec3<float> localPos = localPlayer->stateVectorComponent->pos;

        float distSq = playerPos.distanceTo(localPos);

        if (distSq < closestDistSq) {
            closestDistSq = distSq;
            closest = player;
        }
    }

    return closest;
}

void BedAura::onRender(MinecraftUIRenderContext* renderCtx) {
    if (!render || bestPlacePos == Vec3<int>(0, 0, 0)) return;

    Vec3<float> pos = bestPlacePos.toFloat();
    AABB bb;

    switch (bestDirection) {
    case CardinalDirection::North:
        bb = AABB(pos, pos.add(Vec3<float>(1, 0.5625f, 2)));
        break;
    case CardinalDirection::South:
        bb = AABB(pos.sub(Vec3<float>(0, 0, 1)), pos.add(Vec3<float>(1, 0.5625f, 1)));
        break;
    case CardinalDirection::East:
        bb = AABB(pos.sub(Vec3<float>(1, 0, 0)), pos.add(Vec3<float>(1, 0.5625f, 1)));
        break;
    case CardinalDirection::West:
        bb = AABB(pos, pos.add(Vec3<float>(2, 0.5625f, 1)));
        break;
    }

    RenderUtils::drawBox(bb, sideColor, lineColor, 1.5f, true, true);
}

std::string BedAura::getModName() {
    if (target) {
        return "BedAura [" + std::string(target->getNameTag()->c_str()) + "]";
    }
    else {
        return "BedAura";
    }
}