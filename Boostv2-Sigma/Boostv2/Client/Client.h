#pragma once
#include "Managers/ConfigManager/ConfigManager.h"
#include "Managers/ModuleManager/ModuleManager.h"
#include "Managers/CommandManager/CommandManager.h"
#include "Managers/HookManager/HookManager.h"
#include "Managers/NotificationManager/NotificationManager.h"

class Client {
private:
	static inline bool initialized = false;
public:
	static inline bool isInitialized() { return initialized; }

	static void DisplayClientMessage(const char* fmt, ...);
	static void init();
	static void shutdown();
};