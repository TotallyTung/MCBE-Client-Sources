#include "ConfigManager.h"

#include <windows.storage.h>
#include <wrl.h>
#include "../../Utils/Logger.h"

using namespace ABI::Windows::Storage;
using namespace Microsoft::WRL;
using namespace Microsoft::WRL::Wrappers;

std::wstring ConfigManager::GetRoamingFolderPath() {
	std::string ConfigFolder = (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\Gamesense\\Configs\\");
	return Utils::stringToWstring(ConfigFolder);
}

ConfigManager::ConfigManager() {
	this->roamingFolder = GetRoamingFolderPath();
}

ConfigManager::~ConfigManager() {
}

void ConfigManager::loadConfig(std::string name, bool create) {
	size_t allocSize = name.size() + roamingFolder.size() + 20;  // std::wstring::size() can be wierd so lets make sure this actually fits
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.txt", roamingFolder.c_str(), name.c_str());

	const bool configExists = std::filesystem::exists(fullPath);
	if (configExists || create) {
		if (name != currentConfig)
			saveConfig();  // Save old config

		currentConfig = name;
		if (configExists) {
			std::ifstream confFile(fullPath, std::ifstream::binary);
			json conf;
			try {
				currentConfigObj.clear();
				confFile >> currentConfigObj;
			} catch (json::parse_error& e) {
				logF("Config Load Exception!: %s", e.what());
			}
			currentConfigObj["from"] = "Gamesense";
		}

		if (configExists) {
			moduleMgr->onLoadConfig(&currentConfigObj);
			if (currentConfigObj.contains("prefix")) {
				std::string prefix = currentConfigObj["prefix"];
				cmdMgr->prefix = prefix.at(0);
			}
		}

		if (create) 
			saveConfig();

		if (g_Data.getLocalPlayer() != nullptr) {
			auto notificationsMod = moduleMgr->getModule<Notifications>();
			if (!notificationsMod->isEnabled())
			g_Data.getGuiData()->displayClientMessageF("[Gamesense] %sSuccessfully %s config %s%s%s!", GREEN, !configExists ? "created" : "loaded", GRAY, name.c_str(), GREEN);
			g_Data.addNotification("Loaded config " + name + ".", "", 3.f);
		}
	} else {
		if (g_Data.getLocalPlayer() != nullptr) {
			g_Data.addNotification("Couldn't load config " + name + ".", "", 3.f);
		}
	}

	delete[] fullPath;
}

void ConfigManager::saveConfig() {
	logF("Saving config %s", currentConfig.c_str());
	size_t allocSize = currentConfig.size() + roamingFolder.size() + 20;
	char* fullPath = new char[allocSize];
	sprintf_s(fullPath, allocSize, "%S\\%s.txt", roamingFolder.c_str(), currentConfig.c_str());

	moduleMgr->onSaveConfig(&currentConfigObj);
	std::string prefix;
	prefix.push_back(cmdMgr->prefix);
	currentConfigObj["prefix"] = prefix;

	std::ofstream o(fullPath, std::ifstream::binary);

	o << std::setw(4) << currentConfigObj << std::endl;
	o.flush();
	o.close();

	delete[] fullPath;
}

void ConfigManager::init() {
	logF("Initializing config");
	loadConfig(currentConfig, true);
}

ConfigManager* configMgr = new ConfigManager();
