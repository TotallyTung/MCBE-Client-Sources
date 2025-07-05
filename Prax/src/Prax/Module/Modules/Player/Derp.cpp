//
// Created by vastrakai on 9/17/2023.
//

#include <Prax/SDK/Game/Network/Other/PlayerAuthInputPacket.h>
#include <Prax/SDK/Game/Network/Other/MovePlayerPacket.h>
#include "Derp.h"

#include <Prax/Utils/Math.h>


void Derp::OnPacket(class LoopbackPacketSender *lbps, Packet *packet, bool *canceled) {

    // Get player
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (player == nullptr) return;
    if (player->isDestroying) return;


    float maxYaw = 179.9;
    float minYaw = -179.9;
    float minPitch = -89.9;
    float maxPitch = 89.9;
    // dangerous code 🥶



    if (Mode.SelectedIndex == (int)SCHIZO) {
        // Schizo derp is like a random derp but with random pauses


        static auto yaw = (float)(Misc::RandomNum((int) minYaw, (int) maxYaw));
        static auto pitch = (float)(Misc::RandomNum((int) minPitch, (int) maxPitch));

        // Get the current time in milliseconds
        auto currentTime = Misc::GetCurrentMs();
        static uint64_t nextResume = 0;
        // Roll a number from 1 - 10 and if it lands on 80 then we will pause for a random amount of time
        if (Misc::RandomNum(1, 10) == 5) {
            // if the last resume was less than 250ms ago then dont pause
            uint64_t timeSinceLastResume = currentTime - nextResume;
            if (timeSinceLastResume < 250) return;
            // Pause for a random amount of time
            nextResume = currentTime + Misc::RandomNum(500, 100);
        }


        if (packet->getId() == (int)PacketID::PlayerAuthInput) {
            auto *paip = (PlayerAuthInputPacket *) packet;

            paip->rotation.y = yaw;
            paip->rotation.x = pitch;
            paip->headYaw = yaw;
        }


        // If the current time is less than the next resume time then we will not derp
        if (currentTime < nextResume) return;

        // update the yaw and pitch
        yaw = (float)(Misc::RandomNum((int) minYaw, (int) maxYaw));
        pitch = (float)(Misc::RandomNum((int) minPitch, (int) maxPitch));


    } else
    if (Mode.SelectedIndex == (int)SPIN) { // Spin
        auto yaw = (float)((uint64_t)((double)Misc::GetCurrentMs() * (double)Speed.Value) % (int)(maxYaw - minYaw) + minYaw);
        if (packet->getId() == (int)PacketID::PlayerAuthInput) {
            auto *paip = (PlayerAuthInputPacket *) packet;

            paip->rotation.y = yaw;
            paip->rotation.x = 86;
            paip->headYaw = yaw;
        }
        if (packet->getId() == (int)PacketID::MovePlayer) {
            auto *mpp = (MovePlayerPacket *) packet;

            mpp->rotation.y = yaw;
            mpp->rotation.x = 86;
            mpp->headYaw = yaw;
        }
    } else if (Mode.SelectedIndex == RANDOM) { // Random
        // Use Misc::RandomNum(min, max) to generate random numbers

        if (packet->getId() == (int)PacketID::PlayerAuthInput) {
            auto *paip = (PlayerAuthInputPacket *) packet;

            float yaw = (float)(Misc::RandomNum((int) minYaw, (int) maxYaw));
            float pitch = (float)(Misc::RandomNum((int) minPitch, (int) maxPitch));
            paip->rotation.y = yaw;
            paip->rotation.x = pitch;
            paip->headYaw = yaw;
        }
        if (packet->getId() == (int)PacketID::MovePlayer) {
            auto *mpp = (MovePlayerPacket *) packet;

            float yaw = (float)(Misc::RandomNum((int) minYaw, (int) maxYaw));
            float pitch = (float)(Misc::RandomNum((int) minPitch, (int) maxPitch));
            mpp->rotation.y = yaw;
            mpp->rotation.x = pitch;
            mpp->headYaw = yaw;
        }
    } else if (Mode.SelectedIndex == HEADROLL) // HeadRoll
    {
        auto pitch = (float)((uint64_t)((double)Misc::GetCurrentMs() * (double)Speed.Value) % (int)(maxYaw - minYaw) + minYaw);
        if (packet->getId() == (int)PacketID::PlayerAuthInput) {
            auto *paip = (PlayerAuthInputPacket *) packet;

            paip->rotation.x = pitch;
        }

        if (packet->getId() == (int)PacketID::MovePlayer) {
            auto *mpp = (MovePlayerPacket *) packet;

            mpp->rotation.x = pitch;
        }
    } else if (Mode.SelectedIndex == STATIC) // Static
    {
        if (packet->getId() == (int)PacketID::PlayerAuthInput) {
            auto *paip = (PlayerAuthInputPacket *) packet;

            paip->rotation.y = 0;
            paip->rotation.x = 0;
            paip->headYaw = 0;
        }
        if (packet->getId() == (int)PacketID::MovePlayer) {
            auto *mpp = (MovePlayerPacket *) packet;

            mpp->rotation.y = 0;
            mpp->rotation.x = 0;
            mpp->headYaw = 0;
        }
    }

    if (Nod.Enabled && packet->getId() == (int)PacketID::PlayerAuthInput) {
        auto *paip = (PlayerAuthInputPacket *) packet;

        float pitch;

        // Lerp to minPitch and maxPitch
        static bool up = true; // up = true means we are going up, up = false means we are going down
        static float lastPitch = 0;
        float speed = Speed.Value;
        // Clamp speed from 0.01 to 0.99
        speed = Math::Clamp(speed, 0.01f, 0.99f);
        if (up) {
            // Lerp to maxPitch
            pitch = Math::Lerp(lastPitch, maxPitch, speed);
            if (pitch >= maxPitch - NodValue.Value) up = false;
        } else {
            // Lerp to minPitch
            pitch = Math::Lerp(lastPitch, minPitch, speed);
            if (pitch <= minPitch + NodValue.Value) up = true;
        }

        paip->rotation.x = pitch;
        lastPitch = pitch;
    }
}
