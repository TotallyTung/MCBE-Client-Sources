#pragma once
#include "../../../Utils/MemoryUtil.h"
#include "../../Core/mce.h"

class MobEffect {
public:
	CLASS_MEMBER(mce::Color, color, 0x10);
public:
	static MobEffect* getById(uint32_t id) {
		if (id > 0x24)
			return nullptr;

		static MobEffect** effectList = (MobEffect**)(Addresses::MobEffectListPtr);
		return effectList[id];
	}
};