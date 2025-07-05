//
// Created by vastrakai on 9/3/2023.
//

#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/Utils/Math.h>
#include "ShowRotations.h"



float ShowRotations::Yaw = 0;
float ShowRotations::HeadYaw = 0;
float ShowRotations::Pitch = 0;
float ShowRotations::BodyYaw = 0;
float ShowRotations::OldYaw = 0;
float ShowRotations::OldHeadYaw = 0;
float ShowRotations::OldPitch = 0;
float ShowRotations::OldBodyYaw = 0;
float ShowRotations::LerpedYaw = 0;
float ShowRotations::LerpedHeadYaw = 0;
float ShowRotations::LerpedPitch = 0;
float ShowRotations::LerpedBodyYaw = 0;

void ShowRotations::OnEnable() {
}

void ShowRotations::OnDisable() {
}

void ShowRotations::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();

    if (!LockBodyYaw.Enabled) {
        BodyYaw::updateRenderAngles(player, Yaw);
        OldBodyYaw = BodyYaw;
        BodyYaw = BodyYaw::bodyYaw;
    } else {
        OldBodyYaw = BodyYaw;
        BodyYaw = Yaw;
    }
}

// this OnPacket always gets handled last
void ShowRotations::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {
    if (packet->getId() == (int)PacketID::MovePlayer && Mode.SelectedIndex == 1) {
        auto mpp = (MovePlayerPacket*)packet;

        OldYaw = Yaw;
        OldPitch = Pitch;
        OldHeadYaw = HeadYaw;
        Yaw = mpp->rotation.y;
        Pitch = mpp->rotation.x;
        HeadYaw = mpp->headYaw;
    }

    if (packet->getId() == (int)PacketID::PlayerAuthInput && Mode.SelectedIndex == 0) {
        auto paip = (PlayerAuthInputPacket*)packet;

        OldYaw = Yaw;
        OldPitch = Pitch;
        OldBodyYaw = BodyYaw;
        Yaw = paip->rotation.y;
        Pitch = paip->rotation.x;
        HeadYaw = paip->headYaw;
    }
}

void ShowRotations::OnRender() {
    float deltaTime = ImGui::GetIO().DeltaTime;

    float yawEasing = YawEasingSpeed.Value;
    float headYawEasing = HeadYawEasingSpeed.Value;
    float pitchEasing = PitchEasingSpeed.Value;
    float bodyYawEasing = BodyYawEasingSpeed.Value;

    if (Easing.Enabled) {
        float yaw = Math::Wrap(LerpedYaw, Yaw - 180, Yaw + 180);
        float headYaw = Math::Wrap(LerpedHeadYaw, HeadYaw - 180, HeadYaw + 180);
        float pitch = LerpedPitch;
        float bodyYaw = Math::Wrap(LerpedBodyYaw, BodyYaw - 180, BodyYaw + 180);

        float preLerpedYaw = Math::Lerp(yaw, Yaw, deltaTime * yawEasing);
        float preLerpedHeadYaw = Math::Lerp(headYaw, HeadYaw, deltaTime * headYawEasing);
        float preLerpedPitch = Math::Lerp(pitch, Pitch, deltaTime * pitchEasing);
        float preLerpedBodyYaw = Math::Lerp(bodyYaw, BodyYaw, deltaTime * bodyYawEasing);

        LerpedYaw = Math::Wrap(LerpedYaw, preLerpedYaw - 180, preLerpedYaw + 180);
        LerpedHeadYaw = Math::Wrap(LerpedHeadYaw, preLerpedHeadYaw - 180, preLerpedHeadYaw + 180);
        LerpedBodyYaw = Math::Wrap(LerpedBodyYaw, preLerpedBodyYaw - 180, preLerpedBodyYaw + 180);

        LerpedYaw = Math::Lerp(preLerpedYaw, LerpedYaw, deltaTime * yawEasing);
        LerpedHeadYaw = Math::Lerp(preLerpedHeadYaw, LerpedHeadYaw, deltaTime * headYawEasing);
        LerpedPitch = Math::Lerp(preLerpedPitch, LerpedPitch, deltaTime * pitchEasing);
        LerpedBodyYaw = Math::Lerp(preLerpedBodyYaw, LerpedBodyYaw, deltaTime * bodyYawEasing);
    } else {
        LerpedYaw = Yaw;
        LerpedHeadYaw = HeadYaw;
        LerpedPitch = Pitch;
        LerpedBodyYaw = BodyYaw;
    }
}

