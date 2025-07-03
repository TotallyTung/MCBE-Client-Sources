#pragma once

void* onSetActorMotionPacketDispatcher;
void* onLevelEventPacketDispatcher;
void* onUpdateBlockPacketDispatcher;

void SetActorMotionPacketDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet) 
{
	PacketID mID = packet.get()->getId();
	bool mCancelled = false;

	auto pkt = std::reinterpret_pointer_cast<SetActorMotionPacket>(packet);

	if (pkt->mRuntimeId == InstanceManager::get<ClientInstance>()->getLocalPlayer()->getRuntimeID()) {
		ActorSetMotionEvent event{ pkt->mMotion };
		event.mCancelled = &mCancelled;
		EventManager::registerEvent(&event);
	}

	if (mCancelled) return;

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>( // CallFunc to call the original.
		onSetActorMotionPacketDispatcher, a1, networkIdentifier, netEventCallback, packet);
}

void LevelEventPacketDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet)
{
	PacketID mID = packet.get()->getId();
	bool mCancelled = false;

	auto mPkt = reinterpret_cast<LevelEventPacket*>(packet.get());

	LevelEventPacketEvent event{ mPkt };
	event.mCancelled = &mCancelled;
	EventManager::registerEvent(&event);

	if (mCancelled) return;

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>(
		onLevelEventPacketDispatcher, a1, networkIdentifier, netEventCallback, packet);
}

void UpdateBlockPacketDetour(const float* a1, const float* networkIdentifier, const float* netEventCallback, const std::shared_ptr<Packet>& packet)
{
	PacketID mID = packet.get()->getId();
	bool mCancelled = false;

	auto mPkt = reinterpret_cast<UpdateBlockPacket*>(packet.get());

	UpdateBlockEvent event{ mPkt };
	event.mCancelled = &mCancelled;
	EventManager::registerEvent(&event);

	if (mCancelled) return;

	Memory::CallFunc<void*, const float*, const float*, const float*, const std::shared_ptr<Packet>&>(
		onUpdateBlockPacketDispatcher, a1, networkIdentifier, netEventCallback, packet);
}

class PacketReceiveHook : public FuncHook 
{
public:
	bool Initialize() override 
	{
		std::shared_ptr<Packet> mSetActorMotionPacket = MinecraftPackets::createPacket(PacketID::SetActorMotion);
		std::shared_ptr<Packet> mLevelEventPacket = MinecraftPackets::createPacket(PacketID::LevelEvent);
		std::shared_ptr<Packet> mUpdateBlockPacket = MinecraftPackets::createPacket(PacketID::UpdateBlock);

		Memory::HookFunction((void*)mSetActorMotionPacket->mHandler->vTable[1], (void*)&SetActorMotionPacketDetour, &onSetActorMotionPacketDispatcher, "SetActorMotionPacket::mHandler::handle");
		Memory::HookFunction((void*)mLevelEventPacket->mHandler->vTable[1], (void*)&LevelEventPacketDetour, &onLevelEventPacketDispatcher, "LevelEventPacket::mHandler::handle");
		Memory::HookFunction((void*)mUpdateBlockPacket->mHandler->vTable[1], (void*)&UpdateBlockPacketDetour, &onUpdateBlockPacketDispatcher, "UpdateBlockPacket::mHandler::handle");

		return true;
	}
};
