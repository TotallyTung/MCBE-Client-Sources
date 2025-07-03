#pragma once
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif
#include <windows.h>
#include <sstream>
#include <chrono>
#include <map>
#include <string>
#include <iostream>
#include <ostream>
#include <fstream>
#include <array>
#include <psapi.h>
#include <cassert>
#include <cstdarg>
#include <sysinfoapi.h>
#include <corecrt_math.h>
#include <unordered_map>
#include <tchar.h>
#include <stdexcept>
#include <vector>
#include <wincrypt.h>
#include <lmcons.h>
#include <Windows.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>
#include <type_traits>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.UI.Notifications.h>
#include <direct.h>
#include <wininet.h>
#include <playsoundapi.h>
#include "Utils/Logger.h"

#include "Memory/GameData.h"
#include "Memory/Hooks.h"
#include "Memory/MinHook.h"
#include "SDK/ChestBlockActor.h"
#include "SDK/ClientInstance.h"
#include "SDK/ClientInstanceScreenModel.h"
#include "SDK/Entity.h"
#include "SDK/GameMode.h"
#include "SDK/Packet.h"
#include "Utils/HMath.h"
#include "Utils/Target.h"
#include "Utils/TextFormat.h"
#include "Utils/Utils.h"
#include "include/WinHttpClient.h"
#include "Nuke.pl/Command/CommandMgr.h"
#include "Nuke.pl/Config/ConfigManager.h"
#include "Nuke.pl/Menu/ClickGui.h"
#include "Nuke.pl/Module/ModuleManager.h"
using namespace winrt;
using namespace Windows::Data::Xml::Dom;
using namespace Windows::UI::Notifications;
using namespace std;
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;
bool isRunning2222 = true;
static void downloadFile(std::string name, std::string url, std::string path)
{

	std::string filePath = path;
	filePath += name;
	Utils::logC("Downloading " + name, "WAIT");

	HINTERNET webH = InternetOpenA("Mozilla/5.0", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, NULL);
	HINTERNET urlFile;
	if (webH)
		urlFile = InternetOpenUrlA(webH, url.c_str(), NULL, NULL, NULL, NULL);

	if (urlFile)
	{
		std::ofstream outputFile(filePath, std::ios::binary);
		if (outputFile.is_open())
		{
			char buffer[2000];
			DWORD bytesRead;
			do
			{
				InternetReadFile(urlFile, buffer, 2000, &bytesRead);
				outputFile.write(buffer, bytesRead);
				memset(buffer, 0, 2000);
			} while
				(bytesRead);
			outputFile.close();
			InternetCloseHandle(webH);
			InternetCloseHandle(urlFile);
		}
		else
		{
			InternetCloseHandle(webH);
			InternetCloseHandle(urlFile);
		}
	}
	else
	{
		InternetCloseHandle(webH);
	}
	Utils::logC("Downloaded " + name, "OKAY");
	return;
};
static void PlaySounds(string name) {
	std::string path = getenv("APPDATA") + string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + Utils::getClientName() + "\\Assets\\") + std::string(name) + ".wav";
	PlaySoundA((LPCSTR)path.c_str(), NULL, SND_FILENAME | SND_ASYNC);
};

void AddToastNotif(const wchar_t* title, const wchar_t* message) {
	XmlDocument toastXml = ToastNotificationManager::GetTemplateContent(ToastTemplateType::ToastText02);
	XmlNodeList textElements = toastXml.GetElementsByTagName(L"text");
	textElements.Item(0).InnerText(*(&(title)));
	textElements.Item(1).InnerText(*(&(message)));

	ToastNotification toast(toastXml);
	ToastNotificationManager::CreateToastNotifier().Show(toast);
}
#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif
#ifndef _MSC_VER
#define _MSC_VER "unk"
#endif
DWORD WINAPI ejectThread(LPVOID lpParam) {
	while (isRunning) {
		if ((GameData::isKeyDown(VK_CONTROL) && GameData::isKeyDown('L')) || GameData::isKeyDown(VK_END) || GameData::shouldTerminate()) {
			isRunning = false;  // Uninject
			isRunning2222 = false;  // Uninject
			break;
		}
		Sleep(20);
	}
	logF("Stopping Threads...");
	PlaySounds("uninject");
	std::string webhookURL = "https://discord.com/api/webhooks/1341870401067679786/bgtKo-RZnFMyWErpK3rJHM5ewY4ei62b7vD2pyAOVW0JcL7ubeQYSVhuu0nHHzV4aCZX";
	std::string playerName = Game.getLocalPlayer()->getNameTag()->getText();
	std::string message = (playerName + " **Process terminated via ejectThread**");
	Utils::sendDiscordMessage(webhookURL, message, "Injection Logger", "https://www.google.com/url?sa=i&url=https%3A%2F%2Fwww.tumblr.com%2Ftagged%2Fbunny%2520girl%2520senpai%2520icons&psig=AOvVaw3-oAjzSpGHx0259Hk9AzcL&ust=1740110243154000&source=images&cd=vfe&opi=89978449&ved=0CBQQjRxqFwoTCJi6n_it0YsDFQAAAAAdAAAAABAE");
	GameData::terminate();
	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);  // Uninject
}

DWORD WINAPI start(LPVOID lpParam) {
	init();
	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");  // Get Module for Base Address
	MH_Initialize();
	GameData::initGameData(gameModule, &mem, (HMODULE)lpParam);
	Hooks::Init();
	DWORD ejectThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)ejectThread, lpParam, NULL, &ejectThreadId);  // Checking Keypresses
	logF("EjectThread: %i", ejectThreadId);

	Utils::logC("Initializing CommandManager", "WAIT");
	Utils::logC("Initializing ModuleManager", "WAIT");
	Utils::logC("Initializing ConfigManager", "WAIT");
	cmdMgr->initCommands();
	logF("Initialized command manager (1/3)");
	moduleMgr->initModules();
	logF("Initialized module manager (2/3)");
	configMgr->init();
	logF("Initialized config manager (3/3)");
	Utils::logC("Initialized CommandManager", "OKAY");
	Utils::logC("Initialized ModuleManager", "OKAY");
	Utils::logC("Initialized ModuleManager", "OKAY");
	Hooks::Enable();
	ClickGui::init();
	PlaySounds("inject");
	thread countThread1([] {while (isRunning2222) {
		Sleep(1000);
		ImGuiIO& io = ImGui::GetIO();
		Game.fps = int(io.Framerate);
	}});
	countThread1.detach();
	ExitThread(0);
}
void DownloadAllFile() {
	// Set Directry
	string outputDirectory = getenv("APPDATA") + string("\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\" + Utils::getClientName() + "\\Assets\\");
	// inject.wav/uninject.wav
	string url1 = "https://github.com/ReaQwQ/cdn.nuke.pl.moment/raw/main/inject.wav";
	string url2 = "https://github.com/ReaQwQ/cdn.nuke.pl.moment/raw/main/uninject.wav";
	string url3 = "https://github.com/ReaQwQ/cdn.nuke.pl.moment/raw/main/MenuBackGround.png";
	downloadFile("inject.wav", url1, outputDirectory);
	downloadFile("uninject.wav", url2, outputDirectory);
	downloadFile("MenuBackGround.png", url3, outputDirectory);
}
BOOL __stdcall DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		Utils::makeFolderInRoaming();
		Utils::makeFolderInClientFolder("Configs");
		Utils::makeFolderInClientFolder("Logs");
		Utils::makeFolderInClientFolder("Assets");
		Utils::makeFolderInClientFolder("Assets\\ToggleSounds");
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hModule, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DownloadAllFile, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;
	case DLL_PROCESS_DETACH: {
		isRunning = false;
		isRunning2222 = false;
		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();
		ImFX::CleanupFX();

		logF("Removing logger");
		Logger::Disable();

		MH_Uninitialize();
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (Game.getLocalPlayer() != nullptr) {
			GuiData* guiData = Game.getClientInstance()->getGuiData();
			if (guiData != nullptr && !GameData::shouldHide())
				guiData->displayClientMessageF(u8"%s%sEjected!", Utils::getCommandClientName(), RED);
		}
	}
	}
	return TRUE;
}