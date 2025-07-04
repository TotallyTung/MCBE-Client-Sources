#pragma once
#include "FilePath.h"

class TexturePtr {
private:
	char pad_0x0[0x18];
public:
	FilePath* filePath;
};