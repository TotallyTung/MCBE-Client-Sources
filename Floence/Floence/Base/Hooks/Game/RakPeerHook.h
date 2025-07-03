#pragma once

void* onRakpeer;
void* onAveragePing;

inline float RandomPing(float a, float b) {
	float random = ((float)rand()) / (float)RAND_MAX;
	float diff = b - a;
	float r = random * diff;
	return a + r;
}

__int64 RakpeerDetour(RakPeer* _this, __int64 a1) {
	if (Global::shouldLagTicks && Address::canUseKeys()) {
		return 0;
	}

	return Memory::CallFunc<__int64, RakPeer*, __int64>(
		onRakpeer, _this, a1
	);
}

__int64 getAveragePingDetour(RakPeer* _this, AddressOrGUID a1) {

	if (getModuleByName("pingholder")->isEnabled()) {
		if (!Global::RandomizePing) {
			//return Global::PingHolderValue;
		}
		else {
			//return RandomPing(Global::LowPingValue, Global::HighPingValue);
		}
	}

	return Memory::CallFunc<__int64, RakPeer*, AddressOrGUID>(
		onAveragePing, _this, a1
	);
}

class RakpeerHook : public FuncHook {
public:
	bool Initialize() override
	{
		// RakPeer_RunUpdateCycle

		//auto VTable = *(uintptr_t**)Address::getLoopback()->getNetworkSystem()->getRemoteConnectorComposite()->getRakNetConnector()->getPeer();

		std::thread([&] {
			while (!Address::getClientInstance() || !Address::getClientInstance()->getLoopbackPacketSender() || !Address::getClientInstance()->getLoopbackPacketSender()->getNetworkSystem() || !Address::getClientInstance()->getLoopbackPacketSender()->getNetworkSystem()->getRemoteConnectorComposite() || !Address::getClientInstance()->getLoopbackPacketSender()->getNetworkSystem()->getRemoteConnectorComposite()->getRakNetConnector()) {
				if (!isRunning) break;

				Sleep(0);
			}
			if (!isRunning) return;

			auto VTable = *(uintptr_t**)(Address::getClientInstance()->getLoopbackPacketSender()->getNetworkSystem()->getRemoteConnectorComposite()->getRakNetConnector()->getPeer());

			if (!Memory::HookFunction((void*)VTable[93], (void*)&RakpeerDetour, &onRakpeer, xorstr_("RunUpdateCycle"))) return;

			if (!Memory::HookFunction((void*)VTable[43], (void*)&getAveragePingDetour, &onAveragePing, xorstr_("AveragePing"))) return;
			}).detach();

		return true;
	}

	static RakpeerHook& Instance() { // a class setup function called Instance.
		static RakpeerHook instance;
		return instance;
	}
};