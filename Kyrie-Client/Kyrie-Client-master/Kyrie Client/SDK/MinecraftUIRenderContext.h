#pragma once
#include "ClientInstance.h"
#include <memory>

class MinecraftUIRenderContext {
private:
	uintptr_t** VTable;
public:
	ClientInstance* clientInstance;
};