#pragma once

class SubChunk {
public:
    char                                     pad_0000[32];                     // this+0x0000
    bool                                     needsInitLighting;                // this+0x0020
    bool                                     needsClientLighting;              // this+0x0021
    char                                     pad_0022[6];                      // this+0x0022
    std::unique_ptr<SubChunkStorage<Block>>* blocks;                           // this+0x0028
    char                                     pad_0030[8];                      // this+0x0030
    SubChunkBlockStorage*                    blockReadPtr;                     // this+0x0038
    char                                     pad_0040[24];                     // this+0x0040
    int8_t                                   N000027E5;                        // this+0x0058
    int8_t                                   subchunkIndex;                    // this+0x0059
    char                                     pad_005A[6];                      // this+0x005A

public:
    [[nodiscard]] Block* getBlockFor(ChunkBlockPos blockPos) {
        auto readPtr = this->blockReadPtr;
        uint16_t elementId = (blockPos.x * 0x10 + blockPos.z) * 0x10 + (blockPos.y & 0xf);
        return readPtr->getElement(elementId);
    }
};
