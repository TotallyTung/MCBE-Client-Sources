#pragma once

#include <stdint.h>

struct MobHurtTimeComponent
{
public:
	int32_t hurtTime; //0x0000
}; //Size: 0x0004

static_assert(sizeof(MobHurtTimeComponent) == 0x4);