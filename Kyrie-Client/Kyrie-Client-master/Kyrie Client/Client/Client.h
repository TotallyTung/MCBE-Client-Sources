#pragma once
#include "../Utils/Utils.h"
#include "../Memory/Memory.h"
#include "../Memory/Hooks/HookManager.h"
#include "ModuleManager/ModuleManager.h"

class Client {
private:
	std::string clientName = "Kyrie";
	bool initialized = false;
public:
	ModuleManager* moduleMgr = nullptr;
public:
	inline std::string getClientName() { return this->clientName; }
	inline bool isInitialized() { return this->initialized; }
public:
	void init();
	void unject();
};

extern Client* client;