#pragma once
#include "PacketID.h"

class PacketHandlerDispatcherInstance {
public:
	uintptr_t** vTable;

	//virtual void IPacketHandlerDispatcher() {};
	//virtual void handle(::NetworkIdentifier const&, ::NetEventCallback&, ::std::shared_ptr<::Packet>&) const = 0;
};

class Packet {
public:
	PacketPriority                   mPriority;         // this+0x8
    NetworkPeer::Reliability         mReliability;      // this+0xC
    SubClientId                      mClientSubId;      // this+0x10
    //bool                             mIsHandled;        // this+0x11
    //NetworkPeer::PacketRecvTimepoint mReceiveTimepoint; // this+0x18
	char pad_0018[8]; //0x0018
	PacketHandlerDispatcherInstance* mHandler;          // this+0x20
    Compressibility                  mCompressible;     // this+0x28
	char                             pad_002C[4];       // this+0x2C

	virtual void packetConstructor(void) {};
	virtual PacketID getId(void) {};
	virtual std::string getName(void) {};
	virtual void write(class BinaryStream&) {};
	virtual void read(class ReadOnlyBinaryStream&) {};
	virtual void readExtended(class ReadOnlyBinaryStream&) {};
	virtual void disallowBatching(void) {};
	// virtual bool isValid(void) {};
	// virtual void _read(class ReadOnlyBinaryStream& stream) {};
};
static_assert(sizeof(Packet) == 0x30);

class MinecraftPackets {
public:
	static std::shared_ptr<Packet> createPacket(int packetId) {
		return Memory::CallFastcall<std::shared_ptr<Packet>>(SigManager::MinecraftPackets_createPacket, packetId);
	}

	static std::shared_ptr<Packet> createPacket(PacketID packetId) {
		return createPacket(static_cast<int>(packetId));
	}

	template <typename T>
	static std::shared_ptr<T> createPacket(PacketID id) {
		return std::reinterpret_pointer_cast<T>(createPacket(id));
	}
};

#include "Packets/ActorLink.h"
#include "Packets/NetworkItemStackDescriptor.h"
#include "Packets/SyncedAttribute.h"

#include "Packets/ActorEventPacket.h"
#include "Packets/ActorPickRequestPacket.h"
#include "Packets/AddActorPacket.h"
#include "Packets/AddBehaviorTreePacket.h"
#include "Packets/AddItemActorPacket.h"
#include "Packets/AddPaintingPacket.h"
#include "Packets/AddPlayerPacket.h"
#include "Packets/AnimatePacket.h"
#include "Packets/BookEditPacket.h"
#include "Packets/CameraInstructionPacket.h"
#include "Packets/CommandRequestPacket.h"
#include "Packets/ContainerClosePacket.h"
#include "Packets/ContainerOpenPacket.h"
#include "Packets/DisconnectPacket.h"
#include "Packets/EmotePacket.h"
#include "Packets/InteractPacket.h"
#include "Packets/InventoryContentPacket.h"
#include "Packets/InventoryTransactionPacket.h"
#include "Packets/LevelEventPacket.h"
#include "Packets/MobEffectPacket.h"
#include "Packets/MobEquipmentPacket.h"
#include "Packets/ModalFormRequestPacket.h"
#include "Packets/ModalFormResponsePacket.h"
#include "Packets/MovePlayerPacket.h"
#include "Packets/NetworkStackLatencyPacket.h"
#include "Packets/PlayerActionPacket.h"
#include "Packets/PlayerAuthInputPacket.h"
#include "Packets/PlayerSkinPacket.h"
#include "Packets/PlaySoundPacket.h"
#include "Packets/RemoveActorPacket.h"
#include "Packets/SetActorMotionPacket.h"
#include "Packets/SetHealthPacket.h"
#include "Packets/SetPlayerGameTypePacket.h"
#include "Packets/SetTimePacket.h"
#include "Packets/TextPacket.h"
#include "Packets/UpdateBlockPacket.h"