#pragma once

class SubChunkBlockStorage {
public:
    char                 pad_0008[1640];       // this+0x0008
    class N00002CBA*     paletteData;          // this+0x0670
    char                 pad_0678[56];         // this+0x0678
    int64_t              paletteSize;          // this+0x06B0
    char                 pad_06B8[784];        // this+0x06B8
public:
    virtual ~SubChunkBlockStorage();
    virtual bool isUniform(const Block* block);
    virtual bool isPaletteUniform(const Block* block);
    virtual Block* getElement(uint16_t index);
};
