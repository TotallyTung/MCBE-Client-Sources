#include "PacketSpoofer.h"
bool invalidpos;
int invalidmode;
bool invalidyaw;
bool invalidpitch;
PacketSpoofer::PacketSpoofer() : Module("PacketSpoofer", "Spoof packets", Category::PACKETS) {
	slider<float>("Invalid-Offset", "NULL", ValueType::FLOAT_T, &value, -0.1f, 0.1f);
	enumsetting("InvalidType", "NULL", { "X>Y", "Z>X" ,"Y>X" }, &invalidmode);
	boolsetting("InvalidPos", "NULL", &invalidpos);
	boolsetting("InvalidRot", "NULL", &invalidyaw);
}
void PacketSpoofer::onSendPacket(Packet* packet, bool& shouldCancel) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (!localPlayer) return;
	Vec3<float> pos = *localPlayer->getPosition();
	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		if (invalidpos) {
			if (invalidmode == 0) {
				authPacket->position.x = pos.y - value;
			}
			if (invalidmode == 1) {
				authPacket->position.z = pos.x - value;
			}
			if (invalidmode == 2) {
				authPacket->position.y = pos.x - value;
			}
		}
		if (invalidyaw) {
			if (invalidmode == 0) {
				authPacket->rotation.x = authPacket->rotation.y - value;
			}
			if (invalidmode == 1) {
				authPacket->rotation.y = authPacket->rotation.x - value;
			}
			if (invalidmode == 2) {
				authPacket->rotation.y = authPacket->rotation.x - value;
			}
		}
	}
	if (packet->getId() == PacketID::MovePlayerPacket) {
		auto* movepacket = (MovePlayerPacket*)packet; 
		if (invalidpos) {
			if (invalidmode == 0) {
				movepacket->position.x = pos.y - value;
			}
			if (invalidmode == 1) {
				movepacket->position.z = pos.x - value;
			}
			if (invalidmode == 2) {
				movepacket->position.y = pos.x - value;
			}
		}
		if (invalidyaw) {
			if (invalidmode == 0) {
				movepacket->rotation.x = movepacket->rotation.y - value;
			}
			if (invalidmode == 1) {
				movepacket->rotation.y = movepacket->rotation.x - value;
			}
			if (invalidmode == 2) {
				movepacket->rotation.y = movepacket->rotation.x - value;
			}
		}
	}
}

