//
// Created by vastrakai on 9/22/2023.
//

#include "Trail.h"
#include <Prax/Utils/Math.h>
#include <Prax/Utils/Render/ColorUtil.h>
#include <Prax/SDK/Game/Render/LevelRenderer.h>
#include <Prax/Module/Modules/Combat/Antibot.h>
#include <Prax/Utils/Render/Render.h>

#include "HUD.h"


struct TrailSegment {
    Actor* Entity;
    uint64_t Time;
    Vector3 Position;
};

auto segments = std::vector<TrailSegment>();

int ticks = 0;

void Trail::OnRender() {
    auto corrected = Render::Transform.modelView;
    // toRemove
    std::vector<TrailSegment> toRemove = std::vector<TrailSegment>();

    std::map<Actor*, int> entityColorIndex = std::map<Actor*, int>();



    for (auto &segment : segments) {
        Vector3 position = segment.Position;

        // new code
        float progress = 1.f - ((float) (Misc::GetCurrentMs() - segment.Time) / (FadeTime.Value * 1000));

        if (progress > 1.f || progress < 0.f) {
            toRemove.push_back(segment);
            entityColorIndex[segment.Entity]++;
            continue;
        }

        float animation = progress;

        Vector2 screenPos = { 0,0 };
        Vector3 cameraPos = Render::Transform.origin;

        if (!corrected.OWorldToScreen(Render::Transform.origin, position, screenPos, Math::Fov, Minecraft::ClientInstance->guiData->resolution)) {
            entityColorIndex[segment.Entity]++;
            continue;
        }


        float scaledSize = Size.Value * animation;  // Calculate the scaled size

        if (animation >= ScaleInTime.Value) {
            // Scale up to the current animation (starts from 1), actual animation until the scaleInTime
            float scale = abs(1.f - (animation - ScaleInTime.Value) / (1.f - ScaleInTime.Value));
            scaledSize *= scale;
        } else {
            scaledSize = Size.Value * animation;
        }


        // scale the size down based how large the distance is from the camera
        float distance = cameraPos.Distance(position);
        float scale = abs((distance / 2.5f));
        if (scale < 1.f) scale = 1.f;
        scaledSize /= scale;

        auto color = ImColor(1.f, 1.f, 1.f, 1.f * animation);
        if (RGB.Enabled){
            color = HUD::GetColor(entityColorIndex[segment.Entity] / 10);
        }

        ImGui::GetBackgroundDrawList()->AddShadowCircle(screenPos, scaledSize + 1, ImColor(color.Value.x, color.Value.y, color.Value.z, 1.f * animation), 50 * animation, ImVec2(0, 0));
        ImGui::GetBackgroundDrawList()->AddCircleFilled(screenPos, scaledSize + 1, ImColor(color.Value.x, color.Value.y, color.Value.z, 1.f * animation));
        entityColorIndex[segment.Entity]++;
    }

    for (const TrailSegment &segment : toRemove) {
        auto removeIt = std::remove_if(segments.begin(), segments.end(), [&](const TrailSegment &s) {
            return s.Entity == segment.Entity && s.Time == segment.Time && s.Position == segment.Position;
        });
        segments.erase(removeIt, segments.end());
    }
}


void Trail::OnTick() {
    uint64_t now = Misc::GetCurrentMs();
    ticks++;
    if (ticks >= (int)Delay.Value) {
        ticks = 0;
        auto player = Minecraft::ClientInstance->ThePlayer();
        for (auto actor : player->level->getRuntimeActorList()) {
            if (!actor->isPlayer()) continue;
            if (!RenderOnSelf.Enabled && actor == player) continue;
            if (!RenderOnOthers.Enabled && actor != player) continue;

            Vector3 pos = *actor->getPos() - Vector3(0.f, 1.62f, 0.f);

            if (!segments.empty()) {
                // Get the last trail segment for this actor
                auto lastSegment = std::find_if(segments.rbegin(), segments.rend(), [&](const TrailSegment &segment) {
                    return segment.Entity == actor;
                });

                if (lastSegment != segments.rend()) {
                    // If the distance between the last segment and the current position is less than the minimum distance, don't add a new segment
                    if (lastSegment->Position.Distance(pos) < 0.001f) continue;
                }
            }

            //segments.push_back({ actor, now, pos });
            // add a segment to the start of the vector
            segments.insert(segments.begin(), { actor, now, pos });
        }
    }
}

void Trail::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::ChangeDimension) {
        segments.clear();
    }
}

void Trail::OnEnable() {

}

void Trail::OnDisable() {
    // Clear the positions
    segments.clear();
}
