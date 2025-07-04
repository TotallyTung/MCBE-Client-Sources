#pragma once
#include "../FuncHook.h"

int lastSlot = 0;

class SendPacketHook : public FuncHook {
private:
	using func_t = void(__thiscall*)(PacketSender*, Packet*);
	static inline func_t oFunc;

	static void SendPacketCallback(PacketSender* _this, Packet* packet) {
		static NoPacket* noPacketMod = ModuleManager::getModule<NoPacket>();

		if (noPacketMod->isEnabled() && Game::getLocalPlayer() != nullptr)
			return;

		ModuleManager::onSendPacket(packet);

		oFunc(_this, packet);
	}
public:
	SendPacketHook() {
		OriginFunc = (void*)&oFunc;
		func = (void*)&SendPacketCallback;
	}
};