#pragma once
#include "Packet/Packet.h"
#include "Packet/InventoryTransactionPacket.h"
#include "Packet/PlayerAuthInputPacket.h"
#include "Packet/TextPacket.h"
#include "Packet/MovePlayerPacket.h"
#include "Packet/DisconnectPacket.h"
#include "Packet/MobEquipmentPacket.h"

class MinecraftPackets {
public:
	static std::shared_ptr<Packet> createPacket(PacketID id) {
		using func_t = std::shared_ptr<Packet>(__cdecl*)(PacketID);
		static func_t Func = reinterpret_cast<func_t>(Addresses::MinecraftPackets_createPacket);
		return Func(id);
	}
};