#pragma once
#include <string>
#include "../../../Libs/json.hpp"

using json = nlohmann::json;

class ConfigManager {
private:
	static inline std::string configsPath;
	static inline std::string configFormat = ".txt";
	static inline json currentConfigObj;
public:
	static inline std::string currentConfig = "NULL";

	static void init();
	static bool doesConfigExist(const std::string& name);
	static void createNewConfig(const std::string& name);
	static void loadConfig(const std::string& name);
	static void saveConfig();
};