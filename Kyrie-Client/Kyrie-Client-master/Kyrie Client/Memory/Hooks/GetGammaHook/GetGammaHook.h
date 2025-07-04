#pragma once
#include <stdint.h>

class GetGammaHook {
private:
	static float getGamma(uintptr_t* a1);
public:
	static void init();
};