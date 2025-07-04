#include "AntiCrystal.h"

AntiCrystal::AntiCrystal() : Module("AntiCrystal", "In nukkit anarchy server kind", Category::MISC) {
	addSlider<float>("Height", "NULL", ValueType::FLOAT_T, &value, 0.f, 1.f);
}
void AntiCrystal::onSendPacket(Packet* packet, bool& shouldCancel) {
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

