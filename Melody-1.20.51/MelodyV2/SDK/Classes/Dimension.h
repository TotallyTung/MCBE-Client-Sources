// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include <string>
#include "BlockSource.h"

class Dimension {
public:
	BUILD_ACCESS(this, BlockSource*, blockSource, 0xD0);
	BUILD_ACCESS(this, std::string, name, 0x140);
	BUILD_ACCESS(this, uint32_t, id, 0x160);
};