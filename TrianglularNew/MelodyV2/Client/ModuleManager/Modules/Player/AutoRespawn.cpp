#include "AutoRespawn.h"

AutoRespawn::AutoRespawn() : Module("AutoRespawn", "Respawn after dead", Category::PLAYER) {
}
void AutoRespawn::onSendPacket(Packet* packet, bool& shouldCancel) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();
	if (localPlayer->getDeathTime() == 1) {
		if(packet->getName() == "RespawnPacket")
		{
			auto* movepacket = (RespawnPacket*)packet;
			movepacket->respawnState = RespawnState::ClientReadyToSpawn;
		}
	
		
	
	}
}