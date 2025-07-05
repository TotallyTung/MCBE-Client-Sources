//
// Created by vastrakai on 4/27/2024.
//

#include "DamageEffects.h"

#include <Prax/SDK/Game/Entity/Components/ActorTypeComponent.h>
#include <Prax/SDK/Game/Network/Other/ActorEventPacket.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/Render.h>

#include "HUD.h"
#include "Prax/SDK/Game/Entity/Components/ActorOwnerComponent.h"

std::vector<DamageEffects::Particle> DamageEffects::Particles;

#define DRAW_LIST ImGui::GetBackgroundDrawList()

 constexpr float gravity = -0.0784000015258789f;


void DamageEffects::OnRender()
{
    float delta = ImGui::GetIO().DeltaTime;

    auto corrected = Render::Transform.modelView;

    /*static uint64_t lastBlockUpdate = 0;
    // if the last update was more than 50ms ago
    if (Misc::GetCurrentMs() - lastBlockUpdate > 50) {
        lastBlockUpdate = Misc::GetCurrentMs();
        // update the blocks nearby
        for (auto& p : Particles) {
            p.BlocksNearby.clear();
            std::vector<BlockPosPair> blockz = Misc::GetBlocksWithinDistance2(&p.Position, 3, false);
            p.BlocksNearby.clear();
            p.BlocksNearby.reserve(blockz.size());
            for (auto& bp : blockz) {
                p.BlocksNearby.push_back(bp.GetAABB());
            }
        }
    }*/

    for (int i = 0; i < Particles.size(); i++) {
        auto& p = Particles[i];
        p.Position.x += p.Velocity.x * delta;
        p.Position.y += p.Velocity.y * delta;
        p.Position.z += p.Velocity.z * delta;

        // since we're adjusting by delta, multiply gravity to make it more effective
        p.Velocity.y += Math::Lerp(gravity, gravity * 2, delta);

        if (p.Position.y <= p.FloorY) {

            p.Position.y = p.FloorY;
            p.Velocity.x = Math::Lerp(p.Velocity.x, 0, delta);
            p.Velocity.z = Math::Lerp(p.Velocity.z, 0, delta);
        } else
        {
            /*p.Velocity.x *= 0.99f; // slowly decrease velocity as well as alpha
            p.Velocity.z *= 0.99f;*/
            p.Velocity.x = Math::Lerp(p.Velocity.x, 0, delta * 5);
            p.Velocity.z = Math::Lerp(p.Velocity.z, 0, delta * 5);
        }

        // slowly decrease velocity as well as alpha
        uint64_t elapsed = Misc::GetCurrentMs() - p.StartTime;
        uint64_t maxLife = p.MaxLife * 1000;
        float alpha = 1.0f - static_cast<float>(elapsed) / static_cast<float>(maxLife);
        p.Color.Value.w = alpha;
        if (ThemedColors.Enabled) {
            ImColor balls = HUD::GetColor(0, elapsed);
            p.Color.Value.x = balls.Value.x;
            p.Color.Value.y = balls.Value.y;
            p.Color.Value.z = balls.Value.z;
        }

        if (elapsed >= maxLife) {
            Particles.erase(Particles.begin() + i);
            i--;
            continue;
        }



        Vector2 screenPos;
        if (corrected.OWorldToScreen(Render::Transform.origin, p.Position, screenPos, Math::Fov, Minecraft::ClientInstance->guiData->resolution)) {
            if (screenPos.x < 0 || screenPos.y < 0 || screenPos.x > Minecraft::ClientInstance->guiData->resolution.x || screenPos.y > Minecraft::ClientInstance->guiData->resolution.y) continue;
            // draw the particle and account for distance n stuff
            Vector3 originVec = Render::Transform.origin;
            float distance = p.Position.Distance(originVec);
            float size = ParticleSize.Value;

            float scale = abs((distance / 2.5f));
            if (scale < 1.f) scale = 1.f;
            size /= scale;

            DRAW_LIST->AddShadowCircle(ImVec2(screenPos.x, screenPos.y), size, p.Color, 10.f, ImVec2(1.0f, 1.0f), 0.0f);
            DRAW_LIST->AddCircleFilled(ImVec2(screenPos.x, screenPos.y), size, p.Color);
        }
    }
}

void DamageEffects::OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel)
{
    if (packet->getId() == (int)PacketID::EntityEvent) {
        auto eep = std::reinterpret_pointer_cast<ActorEventPacket>(packet);

        auto player = Minecraft::ClientInstance->ThePlayer();
        if (eep->ActorRuntimeID == player->GetRuntimeID()) return; // ignore self

        Actor* actor = nullptr;

        // Find the actor with this runtimeId
        for (auto&& [_, moduleOwner, type, ridc] : player->entityCtx.registry->view<ActorOwnerComponent, ActorTypeComponent, RuntimeIDComponent>().each())
        {
            if (ridc.runtimeID == eep->ActorRuntimeID) {
                actor = moduleOwner.actor;
                if (actor == nullptr) return;
            }
        }

        if (actor == nullptr) return;


        if (eep->Event == ActorEvent::HURT) {
            float floorY = actor->GetGroundY() + 1.f;

            for (int i = 0; i < static_cast<int>(ParticleCount.Value); i++) {
                Particle p;
                AABBShapeComponent* aabb = actor->GetAABBShapeComponent();
                Vector3 hitboxMin = Vector3(aabb->AAX, aabb->AAY, aabb->AAZ);
                Vector3 hitboxMax = Vector3(aabb->BBX, aabb->BBY, aabb->BBZ);
                Vector3 randomPos = Vector3(Math::RandomFloat(hitboxMin.x, hitboxMax.x), Math::RandomFloat(hitboxMin.y, hitboxMax.y), Math::RandomFloat(hitboxMin.z, hitboxMax.z));



                p.Position = randomPos;
                p.Velocity = Vector3(Math::RandomFloat(-3.f, 3.f), 0.f, Math::RandomFloat(-3.f, 3.f));
                p.StartTime = Misc::GetCurrentMs();
                p.MaxLife = Math::RandomFloat(MinLife.Value, MaxLife.Value);
                p.Color = rgb(255, 0, 0, 255);
                p.FloorY = floorY;
                Particles.push_back(p);
            }
        }


    }
}
