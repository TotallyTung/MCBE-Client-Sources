#pragma once
#include "../../../../../../Utils/Minecraft/Intenvoru.h"
#include "../../ModuleBase/Module.h"
#include <vector>
#include <algorithm>

#define DEFAULT_REACH 3.5f
#define DEFAULT_WALL_REACH 1.5f
#define DEFAULT_DELAY 2
#define DEFAULT_CLICK_COUNT 1
#define DEFAULT_PREDICTION 0
#define DEFAULT_ROTATION_SPEED 10.0f

class Aura : public Module {
private:
    float reach = DEFAULT_REACH, wallReach = DEFAULT_WALL_REACH;
    int delayBetweenHits = DEFAULT_DELAY, clickCount = DEFAULT_CLICK_COUNT;
    float rotationSpeed = DEFAULT_ROTATION_SPEED;
    bool targetMobs = false, rotateTarget = false, swordOnlyMode = false;
    bool infinityAura = false;
    Actor* currentTarget = nullptr;
    std::vector<Actor*> targets;
    Vec2<float> currentRot{};
    int rotationprediction = DEFAULT_PREDICTION;
    std::chrono::steady_clock::time_point lastAttackTime;
    bool rotate = false;

    // Render settings
    bool rendertrue = false;
    UIColor mainColor = UIColor(0, 101, 90, 255);
    int renderSegments = 64;
    int renderSides = 12;
    float renderRotationSpeed = 0.02f;
    float renderThickness = 0.2f;
    bool renderWireframe = false;
    bool renderRotate = true;
    float renderScale = 1.0f;

    std::chrono::steady_clock::time_point renderStartTime;

public:
    Aura() : Module("Aura", "Dynamic combat module with advanced features!", Category::COMBAT) {
        // Combat settings
        registerSetting(new NumberSetting<float>("Reach", "Max attack distance.", &reach, 3.5f, 1.0f, 20.0f));
        registerSetting(new NumberSetting<float>("Wall Reach", "Distance through walls.", &wallReach, 1.5f, 0.f, 20.0f));
        registerSetting(new NumberSetting<int>("Delay", "Milliseconds between attacks.", &delayBetweenHits, 2, 0, 1000));
        registerSetting(new NumberSetting<int>("Hit Attempts", "Attacks per tick.", &clickCount, 1, 1, 20));
        registerSetting(new NumberSetting<float>("Rotation Speed", "Speed of rotation smoothing.", &rotationSpeed, 0.5f, 0.0f, 10.0f));
        registerSetting(new NumberSetting<int>("Rotation Prediction", "How many blocks rotations will predict forwards.", &rotationprediction, 0, 0, 5));
        registerSetting(new ToggleSetting("Rotation", "Rotate to target.", &rotate, false));
        registerSetting(new ToggleSetting("Target Mobs", "Attack mobs as well.", &targetMobs, false));
        registerSetting(new ToggleSetting("Sword Only", "Attack only when holding sword.", &swordOnlyMode, false));
        registerSetting(new ToggleSetting("Infinity Aura", "Spoof position to hit targets within 5 blocks", &infinityAura, false));

        // Render settings
        registerSetting(new ToggleSetting("Render", "Render targets.", &rendertrue, false));
        registerSetting(new ColorSetting("Color", "Target ring color", &mainColor, UIColor(0, 101, 90), true));
        registerSetting(new NumberSetting<int>("Render Segments", "Number of ring segments", &renderSegments, 0, 3, 128));
        registerSetting(new NumberSetting<int>("Render Sides", "Tube sides (unused if unsupported)", &renderSides, 0, 3, 64));
        registerSetting(new NumberSetting<float>("Render Rotation Speed", "Speed of ring auto-rotation", &renderRotationSpeed, 0.0f, 0.0f, 1.0f));
        registerSetting(new NumberSetting<float>("Render Thickness", "Thickness of ring tube", &renderThickness, 0.05f, 0.01f, 2.0f));
        registerSetting(new NumberSetting<float>("Render Scale", "Size multiplier for the render ring", &renderScale, 1.0f, 0.1f, 5.0f));
        registerSetting(new ToggleSetting("Render Wireframe", "Render ring as wireframe", &renderWireframe, false));
        registerSetting(new ToggleSetting("Render Rotate", "Enable spinning render ring", &renderRotate, true));
    }

    void onEnable() override {
        targets.clear();
        rotateTarget = false;
        currentTarget = nullptr;
        currentRot = Game::ourPlayer()->rotation->presentRot;
        lastAttackTime = std::chrono::steady_clock::now();
        renderStartTime = std::chrono::steady_clock::now();
    }

    void onDisable() override {
        targets.clear();
        rotateTarget = false;
        currentTarget = nullptr;
    }

    void onNormalTick(LocalPlayer* player) override {
        if (swordOnlyMode && !isSwordEquipped()) return;
        targets.clear();
        for (auto& entity : player->level->getRuntimeActorList()) {
            if (!TargetUtil::isTargetValid(entity, targetMobs, reach)) continue;
            targets.push_back(entity);
        }
        if (targets.empty()) {
            rotateTarget = false;
            return;
        }
        currentTarget = targets[0];
        rotateTarget = true;
        auto now = std::chrono::steady_clock::now();
        auto timeSinceLastAttack = std::chrono::duration_cast<std::chrono::milliseconds>(now - lastAttackTime).count();
        if (timeSinceLastAttack >= delayBetweenHits) {
            Game::ourPlayer()->gameMode->attackv2(currentTarget, clickCount);
            Game::ourPlayer()->swing();
            lastAttackTime = now;
        }
    }

    void onUpdateRotation(LocalPlayer* player) override {
        if (!rotateTarget || !currentTarget || !rotate) return;
        float deltaTime = 1.0f / 20.0f;
        float t = deltaTime * rotationSpeed;
        Vec3<float> targetEyePos = currentTarget->getEyePos();
        const auto& rot = currentTarget->rotation->presentRot;
        float yaw = rot.y;
        float pitch = rot.x;
        Vec3<float> lookDir = {
            -cosf(pitch * 0.017453292f) * sinf(yaw * 0.017453292f),
            -sinf(pitch * 0.017453292f),
            cosf(pitch * 0.017453292f) * cosf(yaw * 0.017453292f)
        };
        float pred = static_cast<float>(rotationprediction);
        Vec3<float> predictedPos = {
            targetEyePos.x + lookDir.x * pred,
            targetEyePos.y + lookDir.y * pred,
            targetEyePos.z + lookDir.z * pred
        };
        Vec2<float> targetRot = player->getHumanPos().CalcAngle(predictedPos).normAngles();
        currentRot = Math::smoothRotation(currentRot, targetRot, t);
        player->rotation->presentRot = currentRot;
        if (auto* headRot = player->getActorHeadRotationComponent())
            headRot->headYaw = currentRot.y;
    }

    void onLevelRender() override {
        if (!rendertrue || targets.empty()) return;

        static float autoRotation = 0.0f;
        static float verticalTime = 0.0f;

        if (renderRotate)
            autoRotation += renderRotationSpeed;

        verticalTime += 0.05f;

        for (auto& target : targets) {
            if (!target || !target->isAlive()) continue;

            AABB aabb = target->aabbShape->aabb;
            Vec3<float> center = {
                (aabb.lower.x + aabb.upper.x) * 0.5f,
                0.0f,
                (aabb.lower.z + aabb.upper.z) * 0.5f
            };

            Vec3<float> size = {
                aabb.upper.x - aabb.lower.x,
                aabb.upper.y - aabb.lower.y,
                aabb.upper.z - aabb.lower.z
            };
            float height = size.y;
            float middleY = (aabb.lower.y + aabb.upper.y) * 0.5f;
            float amplitude = height * 0.5f;
            float yOffset = std::sin(verticalTime) * amplitude;

            center.y = middleY + yOffset;

            float outerRadius = std::max(size.x, size.z) * 0.5f * renderScale;
            float innerRadius = outerRadius - renderThickness;
            Vec3<float> rot = { 0.f, autoRotation, 0.f };

            MCR::drawRing3d(
                center,
                innerRadius,
                outerRadius,
                mainColor,
                renderWireframe ? UIColor(255, 255, 255, 255) : UIColor(0, 0, 0, 0),
                1.0f,
                rot,
                renderSegments
            );
        }
    }

    bool onSendPacket(Packet* packet) override {
        if (infinityAura && packet->getId() == PacketID::PlayerAuthInput && !targets.empty()) {
            auto* spoofed = new PlayerAuthInputPacket(*static_cast<PlayerAuthInputPacket*>(packet));
            spoofed->position = targets[0]->getHumanPos();
            spoofed->mInputData |= InputData::PerformItemInteraction;
            spoofed->mInputData |= InputData::PerformBlockActions;
            spoofed->mInputData |= InputData::PerformItemStackRequest;
        }
        return false;
    }

private:
    bool isSwordEquipped() {
        int selectedItem = InventoryUtils::getSelectedItemId();
        return selectedItem == 314 || selectedItem == 318 || selectedItem == 313 || selectedItem == 329 || selectedItem == 322 || selectedItem == 622;
    }
};
