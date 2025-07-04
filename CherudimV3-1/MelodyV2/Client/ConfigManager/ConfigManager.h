#pragma once
#pragma comment(lib, "runtimeobject")
#include "../../Libs/Json.hpp"

using json = nlohmann::json;

class ConfigManager {
private:
	std::string currentConfig = "default";
	json currentConfigObj;
public:
	void saveConfig();
	void loadConfig(std::string configName);
	void createConfig(const std::string& configName);
	void deleteConfig(const std::string& configName);
	void dumpDummyData();
};