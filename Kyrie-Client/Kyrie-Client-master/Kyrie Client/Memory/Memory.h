#pragma once
#include <Windows.h>
#include <memory>

class Memory {
public:
	static uintptr_t getBase();
	static void CreateHook(const char* name, uintptr_t address, void* func, void* OriginFunc);
};
