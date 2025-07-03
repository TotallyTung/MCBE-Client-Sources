#include "XPosition.h"

XPosition::XPosition() : Module("X-Position", "Edit your X position.", Category::PACKETS) {
	slider<float>("Offset", "NULL", ValueType::FLOAT_T, &value, -5.f, 5.f);
}
void XPosition::onSendPacket(Packet* packet, bool& shouldCancel) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (!localPlayer) return;
	Vec3<float> pos = *localPlayer->getPosition();
	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		authPacket->position.x = pos.x - value;
	}

	if (packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet;
		movepacket->position.x = pos.x - value;
	}
}

