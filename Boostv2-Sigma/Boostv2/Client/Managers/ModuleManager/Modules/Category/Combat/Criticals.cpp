#include "Criticals.h"


Criticals::Criticals() : Module("Criticals", "Each hit is a critical hit", Category::COMBAT) {
	registerSetting(new EnumSetting("Mode", "Mode", { "MovePacket", "FakeFall", "Velocity", "SlowDown" }, &mode, 3));
	registerSetting(new SliderSetting<float>("val", "NULL", &val, 0.f, -20.2f, 20.f));
}


std::string Criticals::getModeText() {
	switch (mode) {
	case 0: {
		return "MovePacket";
		break;
	}
	case 1: {
		return "FakeFall";
		break;
	}
	case 2: {
		return "Velocity";
		break;
	}
	case 3: {
		return "SlowDown";
		break;
	}
	}
	return "NULL";
}
void Criticals::onNormalTick(LocalPlayer* localPlayer) {
	static int a = 0;
	++a;
	if (a == 2) {
		a = 0;
		return;
	}

	Vec3 localPlayerPos = Game.getLocalPlayer()->getPos();
	if (mode == 0) {
		std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::MovePlayer);
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet.get());
		movePacket->mOnGround = false;
		movePacket->mPos = { localPlayerPos.x, localPlayerPos.y + 1.2f, localPlayerPos.z };
		Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(movePacket);
		movePacket->mPos = { localPlayerPos.x, localPlayerPos.y + 0.4f, localPlayerPos.z };
		Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(movePacket);
		/*
		C_PlayerActionPacket actionPacket;
		actionPacket.action = 8; //��Ծ
		actionPacket.entityRuntimeId = Game.getLocalPlayer()->entityRuntimeId;
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

		C_MovePlayerPacket movePacket;
		movePacket.onGround = false;
		movePacket.Position = *Game.getLocalPlayer()->getPos();
		movePacket.Position.y += 1.2f;
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		Sleep(0.05);
		movePacket.Position.y -= 0.3f;
		Game.getClientInstance()->loopbackPacketSender->sendToServer(&movePacket);
		//Game.getLocalPlayer()->setPos(Game.getLocalPlayer()->getPos()->add(0, 0.9, 0));
		*/
	}
	if (mode == 2) {
		Game.getLocalPlayer()->stateVector->velocity.y = -0.0f;
	}
	if (mode == 3) {
		std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::MovePlayer);
		auto* movePacket = reinterpret_cast<MovePlayerPacket*>(packet.get());
		movePacket->mOnGround = false;
		movePacket->mPos = { localPlayerPos.x, localPlayerPos.y + 1.2f, localPlayerPos.z };
		Game.getClientInstance()->getLocalPlayer()->level->getPacketSender()->sendToServer(movePacket);
		Game.getLocalPlayer()->setPos(localPlayerPos.add2(0.f, val, 0.f));
	}
}

void Criticals::onSendPacket(Packet* packet) {
	LocalPlayer* player = Game.getLocalPlayer();
	if (Game.getLocalPlayer() == nullptr)
		return;
	if (mode == 1) {
		if (packet->getId() == PacketID::PlayerAuthInput) {
			PlayerAuthInputPacket* authPacket = reinterpret_cast<PlayerAuthInputPacket*>(packet);
			if (tick == 0) {
				authPacket->position.y += 0.239f;
				++tick;
				return;
			}
			if (tick == 1) {
				authPacket->position.y += 0.238f;
				++tick;
				return;
			}
			if (tick == 2) {
				authPacket->position.y += 0.237f;
				++tick;
				return;
			}
			if (tick == 3) {
				authPacket->position.y += 0.236f;
				++tick;
				return;
			}
			if (tick == 4) {
				authPacket->position.y += 0.235f;
				++tick;
				return;
			}
			if (tick == 5) {
				authPacket->position.y += 0.234f;
				++tick;
				return;
			}
			if (tick == 6) {
				authPacket->position.y += 0.233f;
				++tick;
				return;
			}
			if (tick == 7) {
				authPacket->position.y += 0.232f;
				++tick;
				return;
			}
			if (tick == 8) {
				authPacket->position.y += 0.231f;
				++tick;
				return;
			}
			if (tick == 9) {
				authPacket->position.y += 0.230f;
				tick = 0;
				return;
			}

			/*
			if (fakeFall == 0) {
				C_PlayerActionPacket actionPacket;
				actionPacket.action = 8; //��Ծ
				actionPacket.entityRuntimeId = Game.getLocalPlayer()->entityRuntimeId;
				Game.getClientInstance()->loopbackPacketSender->sendToServer(&actionPacket);

				authPacket->pos.y += 1.25f;

				++fakeFall;
				return;
			}
			if (fakeFall == 1) {

				authPacket->pos.y += 1.17676f;

				++fakeFall;
				return;
			}
			if (fakeFall == 2) {
				authPacket->pos.y += 1.024424f;

				++fakeFall;
				return;
			}
			if (fakeFall == 3) {
				authPacket->pos.y += 0.796736f;

				++fakeFall;
				return;
			}
			if (fakeFall == 4) {
				authPacket->pos.y += 0.495201f;

				++fakeFall;
				return;
			}
			if (fakeFall == 5) {
				authPacket->pos.y += 0.121297f;

				fakeFall = 0;
				return;
			}
			*/
		}
	}
}

void Criticals::onEnable() {
	if (Game.getLocalPlayer() != nullptr)
		Game.getLocalPlayer()->setPos((Game.getLocalPlayer()->getPos()).add2(0, 0.4f, 0));
}