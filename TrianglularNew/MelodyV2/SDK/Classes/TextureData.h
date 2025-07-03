#pragma once
#include "PathStruct.h"

class TextureData {
private:
	char pad_0x0[0x18]; // 0x0
public:
	PathStruct* ptrToPath; // 0x18
};
class ResourceLocation {
private:
    uint64_t type;        //0x0000
    TextHolder filePath;  //0x0008
public:
    ResourceLocation(std::string filePath, bool external) {
        memset(this, 0, sizeof(ResourceLocation));
        this->filePath.setText(filePath);
        if (external)
            this->type = 2;
    };
};