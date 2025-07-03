// Created by Tony on 2024-10-10 10:01:54

#include "ConfigManager.h"
#include "../../Client/Client.h"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <iomanip>

void ConfigManager::saveConfig() {
    try { 
        std::string path = Utils::getClientPath() + "Configs\\";
        if (!std::filesystem::exists(path))
            std::filesystem::create_directory(path);

        if (client != nullptr && client->moduleMgr != nullptr) {
            client->moduleMgr->onSaveConfig(&currentConfigObj);
        }
        else {
            std::cerr << "Error: Client or module manager is nullptr\n";
        }

        std::string prefix = "A";
        if (client != nullptr && client->commandMgr != nullptr) {
            prefix[0] = client->commandMgr->prefix;
        }
        else {
            std::cerr << "Error: Client or command manager is nullptr\n";
        }
        currentConfigObj["prefix"] = prefix;

        std::string fullPath = path + currentConfig + ".json"; // Changed file extension to .json
        std::ofstream o(fullPath);
        if (o.is_open()) {
            o << std::setw(4) << currentConfigObj.dump(4) << std::endl; // Pretty print JSON
            o.flush();
            o.close();
        }
        else {
            std::cerr << "Error: Unable to open file for writing: " << fullPath << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in saveConfig(): " << e.what() << "\n";
    }
}

void ConfigManager::loadConfig(std::string configName) {
    try {
        std::string path = Utils::getClientPath() + "Configs\\" + configName + ".json"; // Changed file extension to .json
        if (!std::filesystem::exists(path)) {
            std::cerr << "Error: Config file does not exist: " << path << "\n";
            return;
        }

        if (strcmp(configName.c_str(), currentConfig.c_str()) != 0) {
            saveConfig();
        }

        currentConfig = configName;
        std::ifstream confFile(path);
        if (confFile.is_open()) {
            currentConfigObj = json::parse(confFile);
            confFile.close();

            if (client != nullptr && client->moduleMgr != nullptr) {
                client->moduleMgr->onLoadConfig(&currentConfigObj);
            }
            else {
                std::cerr << "Error: Client or module manager is nullptr\n";
            }

            if (currentConfigObj.contains("prefix")) {
                std::string prefix = currentConfigObj["prefix"];
                if (!prefix.empty()) {
                    client->commandMgr->prefix = prefix.at(0);
                }
            }
        }
        else {
            std::cerr << "Error: Unable to open file for reading: " << path << "\n";
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in loadConfig(): " << e.what() << "\n";
    }
}
void ConfigManager::createConfig(const std::string& configName) {
    try {
        std::string path = Utils::getClientPath() + "Configs\\";
        if (!std::filesystem::exists(path))
            std::filesystem::create_directory(path);

        std::string fullPath = path + configName + ".json";
        std::ofstream o(fullPath);
        if (o.is_open()) {
            // Initialize default configuration settings
            json defaultConfig;
            if (client != nullptr && client->moduleMgr != nullptr) {
                client->moduleMgr->onSaveConfig(&defaultConfig);
            }
            else {
                std::cerr << "Error: Client or module manager is nullptr\n";
            }

            std::string prefix = "A";
            if (client != nullptr && client->commandMgr != nullptr) {
                prefix[0] = client->commandMgr->prefix;
            }
            else {
                std::cerr << "Error: Client or command manager is nullptr\n";
            }
            defaultConfig["prefix"] = prefix;

            // Write default configuration content
            o << std::setw(4) << defaultConfig.dump(4) << std::endl; // Pretty print JSON
            o.flush();
            o.close();
            std::cout << "Config created successfully: " << fullPath << std::endl;
        }
        else {
            std::cerr << "Error: Unable to create config file: " << fullPath << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in createConfig(): " << e.what() << std::endl;
    }
}
void ConfigManager::deleteConfig(const std::string& configName) {
    try {
        std::string path = Utils::getClientPath() + "Configs\\";
        std::string fullPath = path + configName + ".json";

        if (std::filesystem::exists(fullPath)) {
            std::filesystem::remove(fullPath);
            std::cout << "Config deleted successfully: " << fullPath << std::endl;
        }
        else {
            std::cerr << "Error: Config file does not exist: " << fullPath << std::endl;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in deleteConfig(): " << e.what() << std::endl;
    }
}
void ConfigManager::dumpDummyData() {
    createConfig("dummy");
    try {
        std::string path = Utils::getClientPath() + "Configs\\";
        std::string fullPath = path + "dummy" + ".json";

        std::ofstream o(fullPath, std::ios_base::app); // Open file in append mode
        if (!o.is_open()) {
            std::cerr << "Error: Unable to open file for writing: " << fullPath << "\n";
            return;
        }

        json data; // JSON object to write
        while (true) {
            // Add your dummy data here, for example:
            data["timestamp"] = std::chrono::system_clock::now().time_since_epoch().count();
            data["value"] = rand() % 100; // Random value for demonstration

            o << std::setw(4) << data.dump() << std::endl; // Write data to file
            o.flush(); // Flush output buffer

            // Sleep for a short interval before writing next data
            //std::this_thread::sleep_for(std::chrono::seconds(1));
        }
    }
    catch (const std::exception& e) {
        std::cerr << "Exception in dummyTextDumper(): " << e.what() << "\n";
    }
}
