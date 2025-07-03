#include "VerticalPosition.h"

VerticalPosition::VerticalPosition() : Module("AntiCrystal", "Edit your vertical position.", Category::PACKETS) {
	slider<float>("Height", "NULL", ValueType::FLOAT_T, &value, -1.f, 0.f);
}
void VerticalPosition::onSendPacket(Packet* packet, bool& shouldCancel) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (!localPlayer) return;
	Vec3<float> pos = *localPlayer->getPosition();
	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->position.y = pos.y - value;
	}

	if (packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->position.y = pos.y - value;
	}
}

