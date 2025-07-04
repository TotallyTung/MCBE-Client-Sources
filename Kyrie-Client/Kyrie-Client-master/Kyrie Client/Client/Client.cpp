#include "Client.h"

#include <direct.h>
#include <filesystem>
#include <MinHook.h>



void Client::init() {
	//Create Client folder;
	_mkdir(Utils::getClientFolderPath().c_str());
	Sleep(25);
	//Create Configs folder;
	_mkdir(Utils::getConfigsFolderPath().c_str());
	Sleep(25);

	{
		std::string file = Utils::getClientFolderPath() + "\\Logs.txt";
		auto f = std::filesystem::path(file);
		if (std::filesystem::exists(f)) std::filesystem::remove(f);
	}

	if (MH_Initialize() == MH_OK) { Utils::DebugF("Successfully Initialized MinHook!"); }
	else {
		Utils::DebugF("Failed to Initialized MinHook!");
		return;
	}

	moduleMgr = new ModuleManager(this);
	HookManager::initAll();

	this->initialized = true;
}

void Client::unject() {
	this->initialized = false;
}

Client* client = new Client();