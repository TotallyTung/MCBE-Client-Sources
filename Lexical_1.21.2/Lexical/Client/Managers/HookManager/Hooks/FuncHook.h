#pragma once
#include <string>
#include <memory>

#include "../../ModuleManager/ModuleManager.h"
#include "../../../../Utils/Logger.h"
#include "../../../../SDK/Game.h"

class FuncHook {
public:
	const char* name;
	uintptr_t address;
	void* OriginFunc;
	void* func;
	bool enable = false;

	virtual bool enableHook();
	virtual void onHookRequest();
};
