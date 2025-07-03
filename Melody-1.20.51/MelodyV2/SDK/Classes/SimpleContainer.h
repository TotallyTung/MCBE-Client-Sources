// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "ItemStack.h"

class SimpleContainer {
public:
	ItemStack* getItemStack(int a2) { // a2 aka slot
		_int64 v2; // r9

		if (a2 < 0)
			return nullptr;
		v2 = *((__int64*)this + 37);
		if (a2 >= (int)((*((__int64*)this + 0x26) - v2) / 160))
			return nullptr;
		else
			return (ItemStack*)(v2 + 160i64 * a2);
	}
};