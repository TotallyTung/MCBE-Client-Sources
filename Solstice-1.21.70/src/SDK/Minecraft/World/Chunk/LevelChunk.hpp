#pragma once
// all of this subchunk stuff was sent to me by tozic and
// im pretty sure its pasted from nuvola lmao
#include <SDK/OffsetProvider.hpp>

#include "SubChunk.hpp"

class LevelChunk {
public:
    char                    pad_0000[120];           // this+0x0000
    int32_t                 chunkPosX;               // this+0x0078
    int32_t                 chunkPosY;               // this+0x007C
    char                    pad_0080[88];            // this+0x0080
    int8_t                  loadingStage;            // this+0x00D8
    char                    pad_00D9[71];            // this+0x00D9
    int32_t                 mLastTick;               // this+0x0120
    char                    pad_0124[20];            // this+0x0124
    std::vector<SubChunk>   subChunks;               // this+0x0138
    char                    pad_0150[3220];          // this+0x0150
    int32_t                 loadCompleted;           // this+0x0DE4
    char                    pad_0DE8[979];           // this+0x0DE8
    bool                    isLoading;               // this+0x11BB
    char                    pad_11BC[20];            // this+0x11BC

    std::vector<SubChunk>* getSubChunks() {
        return &hat::member_at<std::vector<SubChunk>>(this, OffsetProvider::LevelChunk_mSubChunks);
    }
};
