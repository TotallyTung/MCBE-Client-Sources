#pragma once
#include "../../Module.h"
#include "../../../../../Utils/Math.h"

#include <vector>

enum class CardinalDirection {
    North,
    South,
    East,
    West
};

class BedAura : public Module {
private:
    // Settings
    float targetRange = 4.0f;
    float minDamage = 7.0f;
    float maxSelfDamage = 7.0f;
    bool targetMobs = false;
    bool render = true;
    UIColor sideColor = UIColor(15, 255, 211, 75);
    UIColor lineColor = UIColor(15, 255, 211, 255);

    // Internal variables
    CardinalDirection bestDirection;
    Actor* target;
    Vec3<int> bestPlacePos;

    // Helper functions
    bool canPlaceBed(const Vec3<int>& pos);
    void findBestPlacement();
    float calculateBedDamage(Actor* actor, const Vec3<float>& bedPos);
    Actor* getClosestTarget(float range);

    // Damage calculation helper functions
    float computeDistancePercentage(const Vec3<float>& bedPos, Actor* target, float explosionRadius);
    float computeImpact(const Vec3<float>& bedPos, Actor* target, float distPercent);
    float calculateBaseDamage(float impact, float explosionRadius);
    std::pair<int, float> getArmorDetails(Actor* target);
    float applyArmorReduction(float damage, int armorPoints, float epf);

public:
    BedAura();
    virtual void onEnable() override;
    virtual void onDisable() override;
    virtual void onNormalTick(Actor* actor) override;
    virtual void onRender(MinecraftUIRenderContext* renderCtx) override;
    virtual std::string getModName() override;
};