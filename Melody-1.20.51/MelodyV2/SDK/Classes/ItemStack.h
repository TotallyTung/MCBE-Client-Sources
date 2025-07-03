// Created by Tony on 2024-10-10 10:01:54

#pragma once
#include "ItemStackBase.h"

class ItemStack : public ItemStackBase {
public:
	uint8_t stackNetId; //0x0088
	ItemStack()
	{
		memset(this, 0, sizeof(ItemStack));
	}
	bool isValid() {
		return (this->getItemPtr() != nullptr);
	}
};