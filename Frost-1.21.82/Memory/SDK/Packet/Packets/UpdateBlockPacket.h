#pragma once

class UpdateBlockPacket : public Packet {
public:
    enum class BlockLayer : unsigned int {
        Standard = 0,
        Extra    = 1,
        Count    = 2,
    };

    static const bool          SHARE_WITH_HANDLER = true; // constant
    Vector3<int>               mPos;                      // this+0x30
    BlockLayer                 mLayer;                    // this+0x3C
    BlockUpdateFlag            mUpdateFlags;              // this+0x40
    unsigned                   mRuntimeId;                // this+0x44
};