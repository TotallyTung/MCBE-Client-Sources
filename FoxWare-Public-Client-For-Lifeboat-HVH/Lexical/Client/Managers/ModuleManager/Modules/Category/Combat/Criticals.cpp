#include "Criticals.h"

Criticals::Criticals() : Module("Criticals", "Each hit is a critical hit", Category::COMBAT) {
	registerSetting(new EnumSetting("Mode", "NULL", { "Nukkit" }, &mode, 0));
}

std::string Criticals::getModeText() {
	switch (mode) {
	case 0: {
		return "Nukkit";
		break;
	}
	}
	return "NULL";
}

void Criticals::onSendPacket(Packet* packet) {
    static float offsetY = 0.0f;
    static bool descending = true;
    LocalPlayer* localPlayer = Game::getLocalPlayer();
    if (localPlayer == nullptr) {
        offsetY = 0.0f;
        descending = true;
        return;
    }
    if (mode == 0 && packet->getId() == PacketID::PlayerAuthInput) {
        PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
        paip->position.y += offsetY;
        if (descending) {
            offsetY -= 0.05f;
            if (offsetY <= -0.20f) {
                descending = false;
            }
        }
        else {
            offsetY += 0.05f;
            if (offsetY >= 0.0f) {
                offsetY = 0.0f;
                descending = true;
            }
        }
    }
}