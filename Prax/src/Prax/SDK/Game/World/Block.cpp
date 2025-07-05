//
// Created by vastrakai on 5/5/2024.
//

#include "Block.h"

#include <Prax/Hook/Hooks/PacketHandler.h>
#include <Prax/SDK/Game/Minecraft.h>
#include <Prax/SDK/Game/Network/Other/UpdateBlockPacket.h>


uintptr_t** BlockSource::vTable()
{
    if (this == nullptr) {
        return 0x0;
    }
    return *reinterpret_cast<uintptr_t***>(reinterpret_cast<__int64>(this) + 0x0);
}

Block* BlockSource::_getBlock(BlockPos const& pos)
{
    static uintptr_t** vTable = nullptr;
    if (!vTable) {
        vTable = *reinterpret_cast<uintptr_t***>(reinterpret_cast<__int64>(this) + 0x0);
    }
    return ((Block*(__thiscall*)(BlockSource*, BlockPos const&))vTable[2])(this, pos);
}

// this is beyond homeless plz dont @ me i just need it to work :sob:
void BlockSource::clearBlock(BlockPos pos)
{
    std::shared_ptr<UpdateBlockPacket> p = MinecraftPackets::CreatePacket<UpdateBlockPacket>();
    p->Pos = pos;
    p->Layer = UpdateBlockPacket::BlockLayer::Standard;
    p->UpdateFlags = BlockUpdateFlag::Priority;
    p->BlockRuntimeId = 3690217760;
    PacketHandler::HandlePacket(p);
}
