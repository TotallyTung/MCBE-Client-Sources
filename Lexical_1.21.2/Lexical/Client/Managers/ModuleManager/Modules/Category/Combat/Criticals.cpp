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

	LocalPlayer* localPlayer = Game::getLocalPlayer();
	if (localPlayer == nullptr) {
		offsetY = 0.0f;
		return;
	}

	if (mode == 0) {
		if (packet->getId() == PacketID::PlayerAuthInput) {
			PlayerAuthInputPacket* paip = static_cast<PlayerAuthInputPacket*>(packet);
			paip->position.y -= offsetY;
			offsetY += 0.012f;
			if (offsetY >= 0.24f) {
				offsetY = 0.0f;
			}
		}
	}
}