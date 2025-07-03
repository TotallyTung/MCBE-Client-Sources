#pragma once

// SubChunk: 48 2B ? ? ? ? ? 48 8B ? 48 C1 FF ? 48 0F

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
    std::vector<SubChunk>   mSubChunks;              // this+0x0138
    char                    pad_0150[3220];          // this+0x0150
    int32_t                 loadCompleted;           // this+0x0DE4
    char                    pad_0DE8[979];           // this+0x0DE8
    bool                    isLoading;               // this+0x11BB
    char                    pad_11BC[20];            // this+0x11BC
public:
    std::vector<SubChunk>* getSubChunks() {
        return &hat::member_at<std::vector<SubChunk>>(this, 0x158); // 1.21.50
    }

    int getSize() {
        return (mSubChunks.end() - mSubChunks.begin()) / 0x60;
    }
};