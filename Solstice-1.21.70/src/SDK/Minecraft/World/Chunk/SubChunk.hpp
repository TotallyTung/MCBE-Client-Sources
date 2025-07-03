#pragma once
#include "SubChunkStorage.hpp"

class SubChunk {
public:
    char                                     pad_0000[32];                     // this+0x0000
    bool                                     needsInitLighting;                // this+0x0020
    bool                                     needsClientLighting;              // this+0x0021
    char                                     pad_0022[6];                      // this+0x0022
    std::unique_ptr<SubChunkStorage<Block>>* blocks;                            // this+0x0028
    char                                     pad_0030[8];                      // this+0x0030
    class SubChunkBlockStorage*              blockReadPtr;                     // this+0x0038
    char                                     pad_0040[24];                     // this+0x0040
    int8_t                                   N000027E5;                        // this+0x0058
    int8_t                                   subchunkIndex;                    // this+0x0059
    char                                     pad_005A[6];                      // this+0x005A
};
