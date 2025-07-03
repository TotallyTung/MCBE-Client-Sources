#include "NoFall.h"
int mode = 0;
NoFall::NoFall() : Module("NoFall", "Each hit is a critical hit", Category::WORLD) {
	registerSetting(new EnumSetting("Mode", "The Mode", { "Lifeboat" }, &mode, 0));
}

std::string NoFall::getModeText() {
	switch (mode) {
	case 0: {
		return "Lifeboat";
		break;
	}
	}
	return "NULL";
}

void NoFall::onSendPacket(Packet* packet) {
	if (packet->getId() == PacketID::PlayerAuthInput) {
        PlayerAuthInputPacket* paip = reinterpret_cast<PlayerAuthInputPacket*>(packet);
        if (mode == 0) {
            if (!Game.getLocalPlayer()->hasComponent<SetMovingFlagRequestComponent>()) return;

            static bool alt = false;
            alt = !alt;

            static float lastY = 0;
            if (alt) lastY = paip->position.y;
            else
            {
                paip->position.y = lastY + 0.1f;
                paip->mPosDelta = { 0, 0, 0 };
                paip->removeMovingInput();
            }
        }
	}
}
