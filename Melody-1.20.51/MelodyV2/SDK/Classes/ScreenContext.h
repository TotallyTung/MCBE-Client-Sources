// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "Tessellator.h"

class ScreenContext {
public:
	inline float* getColorHolder() {
		return *reinterpret_cast<float**>((uintptr_t)(this) + 0x30);
	}

	inline Tessellator* getTessellator() {
		return *reinterpret_cast<Tessellator**>((uintptr_t)(this) + 0xC0);
	}
};
