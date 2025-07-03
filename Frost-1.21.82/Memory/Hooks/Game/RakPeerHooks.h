#pragma once

void* onRunUpdateCycle;
void* onAveragePing;
void* onSendImmediate;

__int64 getAveragePingDetour(RakNet::RakPeer* _this, RakNet::AddressOrGUID a1) {
	GlobalVariables::mLastPing = Memory::CallFunc<__int64, RakNet::RakPeer*, RakNet::AddressOrGUID>(onAveragePing, _this, a1);

	return Memory::CallFunc<__int64, RakNet::RakPeer*, RakNet::AddressOrGUID>(
		onAveragePing, _this, a1);
}

__int64 RunUpdateCycleDetour(RakNet::RakPeer* _this, __int64 a1) {
	auto VTable = *(uintptr_t**)_this;

	InstanceManager::set<RakNet::RakPeer>(_this);

	static bool mHookedAveragePing = false;

	if (!mHookedAveragePing) {
		mHookedAveragePing = Memory::HookFunction((void*)VTable[44], (void*)&getAveragePingDetour, &onAveragePing, "RakNet::RakPeer::getAveragePing");
	}

	if (!InstanceManager::get<ClientInstance>()->getLocalPlayer()) {
		return Memory::CallFunc<__int64, RakNet::RakPeer*, __int64>(
			onRunUpdateCycle, _this, a1);
	}

	bool mCancelled = false;
	RunUpdateCycleEvent event{};
	event.mCancelled = &mCancelled;
	EventManager::registerEvent(&event);

	if (mCancelled) return 0;

	return Memory::CallFunc<__int64, RakNet::RakPeer*, __int64>(
		onRunUpdateCycle, _this, a1);
}

unsigned __int64 SendImmediateDetour(RakNet::RakPeer* _this, char* data, unsigned int numberOfBitsToSend, PacketPriority priority, PacketReliability reliability, unsigned __int8 orderingChannel, const RakNet::AddressOrGUID* systemIdentifier, bool broadcast, bool useCallerDataAllocation, unsigned __int64 currentTime, unsigned int receipt) {

	return Memory::CallFunc<unsigned __int64, RakNet::RakPeer*, char*, unsigned int, PacketPriority, PacketReliability, unsigned __int8, const RakNet::AddressOrGUID*, bool, bool, unsigned __int64, unsigned int>(
		onSendImmediate, _this, data, numberOfBitsToSend, priority, reliability, orderingChannel, systemIdentifier, broadcast, useCallerDataAllocation, currentTime, receipt);
}

class RakPeerHooks : public FuncHook 
{
public:
	bool Initialize() override
	{
		Memory::HookFunction(SigManager::RakNet_RakPeer_runUpdateCycle, (void*)&RunUpdateCycleDetour, &onRunUpdateCycle, "RakNet::RakPeer::runUpdateCycle");
		Memory::HookFunction(SigManager::RakNet_RakPeer_sendImmediate, (void*)&SendImmediateDetour, &onSendImmediate, "RakNet::RakPeer::sendImmediate");

		return true;
	}
};