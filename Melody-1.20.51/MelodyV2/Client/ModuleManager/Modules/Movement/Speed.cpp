// Created by Tony on 2024-10-10 10:01:54

#include "Speed.h"

Speed::Speed() : Module("Speed", "Don't ever take knockback again.", Category::MOVEMENT) {
	addSlider<float>("SPED", "Chage your speed", ValueType::FLOAT_T, &speed, 2.f, 5.f);
}

void Speed::onNormalTick(Actor* actor) {
	LocalPlayer* localPlayer = mc.getLocalPlayer();





	auto velo = localPlayer->stateVectorComponent->velocity;
	velo = Vec3<float>(0, 0, 0);
	if (localPlayer->isOnGround()) {
		this->stage++;
		switch (stage) {
		case 1:
			localPlayer->stateVectorComponent->velocity.x *= this->speed / 1.2;
			localPlayer->stateVectorComponent->velocity.z *= this->speed / 1.2;
			break;
		case 2:
			localPlayer->stateVectorComponent->velocity.x *= 0.795f;
			localPlayer->stateVectorComponent->velocity.z *= 0.795f;
			break;
		case 3:
			localPlayer->stateVectorComponent->velocity.x *= 0.69f;
			localPlayer->stateVectorComponent->velocity.z *= 0.69f;
			break;
		case 4:
			localPlayer->stateVectorComponent->velocity.x *= 0.75f;
			localPlayer->stateVectorComponent->velocity.z *= 0.75f;
			break;
		case 5:
			this->stage = 0;
		}
	}
	else {
		this->stage = 1;
	}




	//mc.getLocalPlayer()->getAABB()->upper.y = mc.getLocalPlayer()->getAABB()->lower.y;
}
void Speed::onSendPacket(Packet* packet, bool& shouldCancel) {
	if (packet->getId() == PacketID::PlayerAuthInput) {
		PlayerAuthInputPacket* authPacket = (PlayerAuthInputPacket*)packet;
		if (!mc.getLocalPlayer()->isOnGround()) {
			authPacket->downVelocity = Math::lerp(mc.getLocalPlayer()->getPosition()->y, mc.getLocalPlayer()->getPosition()->y - 0.2f, 0.5f);
		}
	}

	
}