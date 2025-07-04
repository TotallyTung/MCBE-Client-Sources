#include "AntiImmobile.h"

AntiImmobile::AntiImmobile() : Module("AntiImmobile", "Prevents AFK kick", Category::MISC) {
}

void AntiImmobile::onSendPacket(Packet* packet) {
    if ((packet->getName() == "PlayerAuthInputPacket" || packet->getName() == "MovePlayerPacket")) {
        static bool toggle = false;
        auto* paip = (PlayerAuthInputPacket*)packet;
        auto* mpp = (MovePlayerPacket*)packet;
        if (toggle) {
            paip->position.y -= 0.01f;
        }
        else {
            paip->position.y -= 0.001f;
        }
        toggle = !toggle;
    }
}