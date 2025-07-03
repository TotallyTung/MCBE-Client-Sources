#pragma once
void* oPacketObserver;

class PacketObserverHook : public FuncHook {
public:
	static void PacketObserver_packetReceivedFrom(void* a1, void* a2, Packet* packet, unsigned int a4) {
		std::string name = packet->getName();
		if (name.length() > 0 && name.find("UpdateBlock") != std::string::npos) {
			UpdateBlockPacket* pkt = (UpdateBlockPacket*)packet;
			//printf("%d %d %d %u\n", pkt->x, pkt->y, pkt->z, pkt->blockRuntimeId);
		}
		if (packet->getId() == 11) {
			printf("dhua");
		}

		return CallFunc<void, void*, void*, Packet*, unsigned int>(oPacketObserver, a1, a2, packet, a4);
	}

	static PacketObserverHook& Get() {
		static PacketObserverHook instance;
		return instance;
	}

	bool Initialize() override {
		uintptr_t addr = findSig(xorstr_("40 53 48 83 EC 20 FF 41 2C 41 8B D9 01 59 28"), "PacketReceiver");
		if (not HookFunction((void*)addr, (void*)&PacketObserver_packetReceivedFrom, &oPacketObserver, xorstr_("PacketObserver_packetReceivedFrom")))
			return false;
		return true;
	}
};
