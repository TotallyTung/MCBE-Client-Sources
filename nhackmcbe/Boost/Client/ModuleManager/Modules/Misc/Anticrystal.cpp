#include "AntiCrystal.h"
int mode = 0;
bool ign = false;
AntiCrystal::AntiCrystal() : Module("AntiCrystal", "Prevents crystal damage.", Category::MISC) {
	addSlider<float>("Height", "NULL", ValueType::FLOAT_T, &value, 0.f, 1.f);
	addBoolCheck("IGN", "Bypass IGN", &ign);
}
void AntiCrystal::onSendPacket(Packet* packet, bool& shouldCancel) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (!localPlayer) return;
	auto* movepacket = (MovePlayerPacket*)packet;
	PlayerAuthInputPacket* authinput = (PlayerAuthInputPacket*)packet;
	Vec3<float> pos = *localPlayer->getPosition();
	if (packet->getId() == PacketID::PlayerAuthInput) {
		authinput->position.y = pos.y - value;
	}

	if (packet->getId() == PacketID::MovePlayerPacket) {
		movepacket->position.y = pos.y - value;
	}
}
