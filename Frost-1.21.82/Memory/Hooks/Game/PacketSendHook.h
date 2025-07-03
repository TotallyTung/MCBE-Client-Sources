#pragma once
#include "../../../Client/Modules/Visual/ServerRotations.hpp"

void* onSendToServer;

void SendToServerDetour(LoopbackPacketSender* _this, Packet* packet) {
	PacketID mID = packet->getId();

	bool mCancelled = false;
	PacketEvent event{ _this, packet };
	event.mCancelled = &mCancelled;
	EventManager::registerEvent(&event);

	if (mID == PacketID::PlayerAuthInput) {
		auto* pkt = reinterpret_cast<PlayerAuthInputPacket*>(packet);
		if (pkt) {
			getModuleByType<ServerRotations>()->mBodyYaw = pkt->mRotation.y;
			getModuleByType<ServerRotations>()->mHeadYaw = pkt->mYHeadYaw;
			getModuleByType<ServerRotations>()->mPitch = pkt->mRotation.x;
		}
	}

	if (mID == PacketID::MovePlayer) {
		auto* pkt = reinterpret_cast<MovePlayerPacket*>(packet);
		if (pkt) {
			getModuleByType<ServerRotations>()->mBodyYaw = pkt->mRot.y;
			getModuleByType<ServerRotations>()->mHeadYaw = pkt->mYHeadRot;
			getModuleByType<ServerRotations>()->mPitch = pkt->mRot.x;
		}
	}

	if (mID == PacketID::Text) {
		auto* pkt = reinterpret_cast<TextPacket*>(packet);
		std::string* mMsg = &pkt->mMessage;

		if (mMsg->c_str()[0] == *".")
		{
			mCommandMgr.sendCommand(mMsg->c_str());
			return;
		}
	}

	if (mCancelled) return;

	Memory::CallFunc<void*, LoopbackPacketSender*, Packet*>(
		onSendToServer, _this, packet);
}

class PacketSendHook : public FuncHook 
{
public:
	bool Initialize() override 
	{
		auto LoopbackVTable = *(uintptr_t**)InstanceManager::get<ClientInstance>()->getLoopbackPacketSender();

		Memory::HookFunction((void*)LoopbackVTable[2], (void*)&SendToServerDetour, &onSendToServer, "LoopbackPacketSender::send");

		return true;
	}
};
