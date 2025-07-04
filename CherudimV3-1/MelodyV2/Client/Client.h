#pragma once
#include <string>
#include "../Utils/Utils.h"
#include "../Utils/Timerutils.h"
#include "../Utils/MemoryUtils.h"

#include "../SDK/MCTextFormat.h"
#include "../SDK/Classes/ContainerScreenControllerTickEventContainerScreenControllerTickEvent.h"

#include "CommandManager/CommandManager.h"
#include "ConfigManager/ConfigManager.h"
#include "ModuleManager/ModuleManager.h"
#include "HookManager/HooksManager.h"

class Client {
private:
	std::string clientName = "Melody";
	std::string clientVersion = "V2.0.1";
	bool initialized = false;

public:
	ConfigManager* configMgr = nullptr;
	ModuleManager* moduleMgr = nullptr;
	CommandManager* commandMgr = nullptr;
public:
	std::string getClientName();
	void setClientName(const std::string& newName);
	std::string getClientVersion();
	inline bool isInitialized() const {
		return (mc.getClientInstance() != nullptr && initialized);
	}

	void init();
	~Client();
};

extern Client* client;