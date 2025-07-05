//
// Created by vastrakai on 4/19/2024.
//

#include <Prax/Utils/Math.h>
#include <Prax/SDK/Game/Network/Other/PlayerActionPacket.h>
#include "LevelTransitions.h"
#include <Prax/Utils/Render/Render.h>
#include <Prax/SDK/Game/Internal/MinecraftGame.h>

uint64_t LevelTransitions::LastDimensionChange = 0;
float LevelTransitions::AnimPerc = 0.f;
bool LevelTransitions::IsTransitioning = false;

#define DRAW_LIST ImGui::GetBackgroundDrawList()

void LevelTransitions::OnRender() {
    AnimPerc = Math::Lerp(AnimPerc, 1.f, ImGui::GetIO().DeltaTime * TransitionSpeed.Value);

    if (IsTransitioning) AnimPerc = 0.f;
    float alpha = 1.f - AnimPerc;

    static bool lastTransitioning = false;

    if (lastTransitioning != IsTransitioning && !IsTransitioning && !Minecraft::ClientInstance->mcGame->canUseKeys()) {
        // Set AnimPerc to 0 and dont set lastTransitioning to IsTransitioning
        AnimPerc = 0.f;
    } else lastTransitioning = IsTransitioning;

    if (LastDimensionChange == 0) return;

    if (Misc::GetCurrentMs() - LastDimensionChange > 5000) {
        LastDimensionChange = 0;
        return;
    }

    auto size = ImGui::GetIO().DisplaySize;

    DRAW_LIST->AddRectFilled({ 0, 0 }, size, IM_COL32(0, 0, 0, 255 * alpha));
    if (IsTransitioning) {
        // Draw small text in the bottom left saying "Loading..."
        float guiScalar = 1.f - Minecraft::ClientInstance->guiData->scalingMultiplier;
        float fontSize = 50.f * guiScalar;

        Vector2 pos = { 10.f * guiScalar, size.y - 10.f * guiScalar - fontSize };

        Render::RenderText("Loading...", pos, fontSize, ImColor(255, 255, 255, (int)(255.f * alpha)), true);
    }
}

void LevelTransitions::OnPacket(LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int) PacketID::PlayerAction) {
        auto playerActionPacket = (PlayerActionPacket *) packet;
        if (playerActionPacket->mAction == PlayerAction::ChangeDimensionAck) {
            LastDimensionChange = Misc::GetCurrentMs();
            AnimPerc = 0.f;
            IsTransitioning = false;
        }
    }
}

void LevelTransitions::OnPacketReceive(std::shared_ptr<Packet> packet, bool *cancel) {
    if (packet->getId() == (int)PacketID::ChangeDimension) {
        LastDimensionChange = Misc::GetCurrentMs();
        AnimPerc = 0.f;
        IsTransitioning = true;
    }
}
