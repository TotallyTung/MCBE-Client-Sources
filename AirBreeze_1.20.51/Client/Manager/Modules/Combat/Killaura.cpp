#include "Killaura.h"
#include "../../Manager.h"
#include "../../Client.h"
#include "../../../Include/imgui/imgui.h"
#include <cmath>
#include <chrono>

static bool isInitialized = false;
static float targetDistance = 0.0f;
static Actor* targetPlayer = nullptr;
static std::chrono::high_resolution_clock::time_point lastRideTime = std::chrono::high_resolution_clock::now();
static std::chrono::high_resolution_clock::time_point lastAttackTime = std::chrono::high_resolution_clock::now();


// Function to calculate the distance between two Vec3<float> positions
float calculateDistance(const Vec3<float>& pos1, const Vec3<float>& pos2) {
    float dx = pos1.x - pos2.x;
    float dy = pos1.y - pos2.y;
    float dz = pos1.z - pos2.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}

void Killaura::onBaseTick(Actor* actor, Level* level, GameMode* gm) {
    auto instance = Minecraft::getClientInstance();
    auto localPlayer = instance->getLocalPlayer();

    if (!localPlayer) return;

    // Control riding action every half a second
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto timeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastRideTime).count();

    if (timeDifference > 500) {  // Ride every 500 milliseconds (half a second)
        if (targetPlayer) {
            localPlayer->setSneaking(true);  // Sneak instead of unride
            lastRideTime = currentTime;
        }
        else {
            auto list = localPlayer->level->getRuntimeActorList();

            for (Actor* player : list) {
                if (player != localPlayer && player->isPlayer() && player->isAlive()) {
                    Vec3<float> localPos = *localPlayer->getPosition();
                    Vec3<float> playerPos = *player->getPosition();

                    float playerDistance = calculateDistance(localPos, playerPos);

                    if (playerDistance > maxTargetDistance && playerDistance <= 50.0f) {
                        // Start riding the target player
                        //localPlayer->startRiding(*player);
                        targetPlayer = player;
                        lastRideTime = currentTime;
                        return;
                    }
                }
            }
        }
    }

    // Control attacking with CPS limit
    auto attackTimeDifference = std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastAttackTime).count();
    if (attackTimeDifference > cpsDelay) {  // Control CPS limit
        if (targetPlayer) {
            if (targetPlayer->isAlive()) {
                if (localPlayer->isAlive()) {
                    Vec3<float> localPos = *localPlayer->getPosition();
                    Vec3<float> targetPos = *targetPlayer->getPosition();

                    targetDistance = calculateDistance(localPos, targetPos);

                    if (targetDistance > maxTargetDistance) {
                        // If within range, attack the target player
                        gm->attack(*targetPlayer);
                    }
                }
            }
            else {
                targetPlayer = nullptr;
            }

            lastAttackTime = currentTime;  // Update the last attack time
        }
    }
}

void Killaura::onImGuiRender() {
    if (!isInitialized) {
        isInitialized = true;
    }



    /*ImGui::Begin("Killaura Debug");

    if (targetPlayer) {
        ImGui::Text("Target Player: %s", targetPlayer->getFormattedNameTag().c_str());
    }

    ImGui::Text("Target Distance: %.2f", targetDistance);

    ImGui::End();*/
}