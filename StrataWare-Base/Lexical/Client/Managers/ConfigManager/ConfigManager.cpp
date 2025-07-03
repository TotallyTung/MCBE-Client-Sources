#include "ConfigManager.h"
#include "../../Client.h"
#include "../../../Utils/FileUtil.h"

#include "../ModuleManager/ModuleManager.h"
#include "../NotificationManager/NotificationManager.h"
#include <windows.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

void downloadFile(const std::string& url, const std::string& outputPath) {
	HINTERNET hInternet = InternetOpenA("ConfigDownloader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (hInternet) {
		HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
		if (hFile) {
			char buffer[4096];
			DWORD bytesRead;
			HANDLE hOutputFile = CreateFileA(outputPath.c_str(), GENERIC_WRITE, 0, NULL, CREATE_ALWAYS, FILE_ATTRIBUTE_NORMAL, NULL);
			if (hOutputFile != INVALID_HANDLE_VALUE) {
				while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead != 0) {
					DWORD bytesWritten;
					WriteFile(hOutputFile, buffer, bytesRead, &bytesWritten, NULL);
				}
				CloseHandle(hOutputFile);
			}
			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInternet);
	}
}

void ConfigManager::init() {
	configsPath = FileUtil::getClientPath() + "Configs\\";
	if (!FileUtil::doesFilePathExist(configsPath))
		FileUtil::createPath(configsPath);
}

bool ConfigManager::doesConfigExist(const std::string& name) {
	std::string path = configsPath + name + configFormat;
	return FileUtil::doesFilePathExist(path);
}

void ConfigManager::createNewConfig(const std::string& name) {
	if (doesConfigExist(name)) {
		Client::DisplayClientMessage("%sFailed to create config %s%s%s. Config already existed!", MCTF::RED, MCTF::GRAY, name.c_str(), MCTF::RED);
		return;
	}
	if (currentConfig != "NULL") {
		saveConfig();
	}
	/* This is how we download images. Im sure the drawimage in D2D can be optimized, im just lazy. */
	currentConfig = name;
	saveConfig();
	std::string downloadUrlName = "https://raw.githubusercontent.com/93281/tickware.sound/228c3c1b12f8d24fb183395843e7d247cea12d5c/Senko.png";
	std::string outputPathName = configsPath + "Name.png";
	downloadFile(downloadUrlName, outputPathName);
	std::string downloadUrlWatermark = "https://raw.githubusercontent.com/93281/tickware.sound/12c5493223b0f9dfce15b479aac901029624e2e4/Watermark.png";
	std::string outputPathWatermark = configsPath + "Watermark.png";
	downloadFile(downloadUrlWatermark, outputPathWatermark);
	Client::DisplayClientMessage("Successfully created config %s%s%s!", MCTF::GRAY, name.c_str(), MCTF::WHITE);
}

void ConfigManager::loadConfig(const std::string& name) {
	if (!doesConfigExist(name)) {
		Client::DisplayClientMessage("%sFailed to load config %s%s%s. Config doesn't exist!", MCTF::RED, MCTF::GRAY, name.c_str(), MCTF::RED);
		return;
	}

	/*
	if (currentConfig == name) {
		return;
	}
	*/

	/*
	if (currentConfig != "NULL") {
		saveConfig();
	}
	*/

	currentConfig = name;

	std::string configPath = configsPath + name + configFormat;

	std::ifstream confFile(configPath.c_str(), std::ifstream::binary);
	currentConfigObj.clear();
	confFile >> currentConfigObj;
	ModuleManager::onLoadConfig((void*)&currentConfigObj);

	Client::DisplayClientMessage("Successfully loaded config %s%s%s!", MCTF::GRAY, name.c_str(), MCTF::WHITE);
}

void ConfigManager::saveConfig() {
	if (currentConfig == "NULL")
		return;

	std::string configPath = configsPath + currentConfig + configFormat;

	ModuleManager::onSaveConfig((void*)&currentConfigObj);

	std::ofstream o(configPath, std::ifstream::binary);
	o << std::setw(4) << currentConfigObj << std::endl;
	o.flush();
	o.close();
}