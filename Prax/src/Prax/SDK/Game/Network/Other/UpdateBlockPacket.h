//
// Created by vastrakai on 10/30/2023.
//

#pragma once

#include "Prax/SDK/Game/Network/Network.h"

enum class BlockUpdateFlag : unsigned char {
    None                = 0,
    Neighbors           = 1 << 0,
    Network             = 1 << 1,
    NoGraphic           = 1 << 2,
    Priority            = 1 << 3,
    ForceNoticeListener = 1 << 4,
    All                 = Neighbors | Network, // default value in BDS
    AllPriority         = All | Priority,
};

class UpdateBlockPacket : public Packet {
public:
/*    BlockPos Pos;
    uint32_t Layer;
    uint8_t UpdateFlags;
    uint64_t BlockRuntimeId;*/
    static const PacketID ID = PacketID::UpdateBlock;

    enum class BlockLayer : unsigned int {
        Standard = 0,
        Extra    = 1,
        Count    = 2,
    };

    static const bool          SHARE_WITH_HANDLER = true; // constant
    BlockPos                   Pos;                      // this+0x30
    BlockLayer                 Layer;                    // this+0x3C
    BlockUpdateFlag            UpdateFlags;              // this+0x40
    unsigned int               BlockRuntimeId;                // this+0x44

    std::string toString() const {
        // Return fields as string (use magic_enum if needed)
        return "Layer: " + std::to_string((int)Layer) + " | UpdateFlags: " + std::to_string((int)UpdateFlags) + " | BlockRuntimeId: " + std::to_string(BlockRuntimeId);
    }

};
