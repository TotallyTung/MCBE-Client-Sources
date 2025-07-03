#include "Client.h"

#include "../Utils/FileUtil.h"
#include "../Utils/NetworkUtil.h"
#include "../Utils/Logger.h"

void AddFont(const std::string& fontPath) {
	std::wstring temp = std::wstring(fontPath.begin(), fontPath.end());
	AddFontResourceW(temp.c_str());
}
#include <Windows.h>
#include <string>

std::string WStringToString(const std::wstring& wstr) {
    if (wstr.empty()) return std::string();
    int sizeNeeded = WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), nullptr, 0, nullptr, nullptr);
    std::string result(sizeNeeded, 0);
    WideCharToMultiByte(CP_UTF8, 0, wstr.data(), (int)wstr.size(), &result[0], sizeNeeded, nullptr, nullptr);
    return result;
}

void Client::DisplayClientMessage(const char* fmt, ...) {
    LocalPlayer* localPlayer = Game::ourPlayer();
    if (localPlayer == nullptr)
        return;

    va_list arg;
    va_start(arg, fmt);
    char message[300];
    vsprintf_s(message, 300, fmt, arg);
    va_end(arg);

    static std::wstring headerMessage;
    if (headerMessage.empty()) {
        // Use wide characters directly in header format
        headerMessage = L"[" + std::wstring(L"花の星") + L"] ";
    }

    // Convert the char message into std::wstring
    std::wstring messageStr = headerMessage + std::wstring(message, message + strlen(message));

    // Convert the full wide string to narrow string (std::wstring to std::string)
    std::string finalMessageStr = WStringToString(messageStr);

    // Display the final message
    localPlayer->displayClientMessage(finalMessageStr);
}

void Client::init() {
	std::string ClientPath = FileUtil::getClientPath();
	if (!FileUtil::doesFilePathExist(ClientPath)) {
		FileUtil::createPath(ClientPath);
	}
	Logger::init();
	ConfigManager::init();
	Addresses::init();
	MCTF::initMCTF();
	ModuleManager::init();
	CommandManager::init();
	HookManager::init();
	if (Game::clientInstance == nullptr)
		return;
	initialized = true;
	Beep(50, 255);
	if (!ConfigManager::doesConfigExist("default"))
		ConfigManager::createNewConfig("default");
	ConfigManager::loadConfig("default");
}

void Client::shutdown() {
	ConfigManager::saveConfig();
	initialized = false;
}