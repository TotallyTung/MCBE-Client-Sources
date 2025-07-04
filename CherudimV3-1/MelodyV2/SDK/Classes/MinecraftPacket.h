#pragma once
#include "../../Utils/MemoryUtils.h"
#include "Packet.h"

#include <memory>

namespace MinecraftPackets
{
	static std::shared_ptr<Packet> __cdecl createPacket(PacketID id){
		using func_t = std::shared_ptr<Packet>(__cdecl*)(PacketID);
		static func_t func = (func_t)findSig(Sigs::packet::MinecraftPacket);
		return func(id);
	}
}