#include "Loader.h"
#include <iomanip>
#include <chrono>
#include <string>
#include <Windows.h>
#include <Psapi.h>
#include <iostream>
#include <ostream>
#include <direct.h>
#include <conio.h>
#include <sstream>
#include <windows.h>
#include <stdio.h>
#include<TlHelp32.h>
#include <tchar.h> 
#include <vector>
#include <thread>
#include <wtsapi32.h>
#include <psapi.h>
#include <math.h>
#include <algorithm> 
#include <playsoundapi.h>
#include<windows.h>
#include <tchar.h>
#include <urlmon.h>
#include "Menu/HudEditor.h"
#pragma comment(lib, "urlmon.lib")
#pragma comment(lib, "wininet.lib")
SlimUtils::SlimMem mem;
const SlimUtils::SlimModule* gameModule;
bool isRunning = true;

#if defined _M_X64
#pragma comment(lib, "MinHook.x64.lib")
#elif defined _M_IX86
#pragma comment(lib, "MinHook.x86.lib")
#endif
#pragma comment(lib, "wininet.lib")
#include <wininet.h>
static void downloadFile(std::string name, std::string url, std::string path)
{

	std::string filePath = path;
	filePath += name;

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
	return;
};
DWORD WINAPI keyThread(LPVOID lpParam) {
	logF("Key thread started");

	bool* keyMap = static_cast<bool*>(malloc(0xFF * 4 + 0x4));
	if (keyMap == nullptr)
		throw std::exception("Keymap not allocated");

	auto clickMap = reinterpret_cast<uintptr_t>(malloc(5));
	if (clickMap == 0)
		throw std::exception("Clickmap not allocated");

	bool* keyMapAddr = nullptr;
	uintptr_t sigOffset = FindSignature("48 8D 0D ?? ?? ?? ?? 89 1C B9");
	if (sigOffset != 0x0) {
		int offset = *reinterpret_cast<int*>((sigOffset + 3)); // Get Offset from code
		keyMapAddr = reinterpret_cast<bool*>(sigOffset + offset + /*length of instruction*/ 7); // Offset is relative
	}
	else {
		logF("!!!KeyMap not located!!!");
		throw std::exception("Keymap not located");
	}

	C_HIDController** hidController = g_Data.getHIDController();

	while (isRunning) {
		if ((GameData::isKeyDown('L') && GameData::isKeyDown(VK_CONTROL)) || GameData::shouldTerminate()) {  // Press L to uninject
			isRunning = false;
			break;
		}

		for (uintptr_t i = 0; i < 0xFF; i++) {
			bool* newKey = keyMapAddr + (4 * i);
			bool newKeyPressed = (*newKey) && GameData::canUseMoveKeys();  // Disable Keybinds when in chat or inventory
			bool* oldKey = keyMap + (4 * i);
			if (newKeyPressed != *oldKey) {
				moduleMgr->onKeyUpdate((int)i, newKeyPressed);
			}
			if (*newKey != *oldKey) {  // Skip Chat or inventory checks
				TabGui::onKeyUpdate((int)i, *newKey);
				ClickGui::onKeyUpdate((int)i, *newKey);
			}
		}

		memcpy_s(keyMap, 0xFF * 4, keyMapAddr, 0xFF * 4);

		Sleep(2);
	}
	logF("Stopping Threads...");
	GameData::terminate();
	Sleep(200);
	Utils::systemPlay("uninject.wav");
	FreeLibraryAndExitThread(static_cast<HMODULE>(lpParam), 1);
}

#ifndef _MSC_VER
#define _MSC_VER "unk"
#endif

DWORD WINAPI start(LPVOID lpParam) {
	logF("Starting up...");
	logF("MSC v%i at %s", _MSC_VER, __TIMESTAMP__);
	init();

	DWORD procId = GetCurrentProcessId();
	if (!mem.Open(procId, SlimUtils::ProcessAccess::Full)) {
		logF("Failed to open process, error-code: %i", GetLastError());
		return 1;
	}
	gameModule = mem.GetModule(L"Minecraft.Windows.exe");  // Get Module for Base Address

	MH_Initialize();
	GameData::initGameData(gameModule, &mem, (HMODULE)lpParam);
	TargetUtil::init(g_Data.getPtrLocalPlayer());

	Hooks::Init();

	DWORD keyThreadId;
	CreateThread(nullptr, NULL, (LPTHREAD_START_ROUTINE)keyThread, lpParam, NULL, &keyThreadId);  // Checking Keypresses
	logF("KeyT: %i", keyThreadId);

	cmdMgr->initCommands();
	logF("Initialized command manager (1/4)");
	moduleMgr->initModules();
	logF("Initialized module manager (2/4)");
	configMgr->init();
	logF("Initialized config manager (3/4)");
	SettingMgr->init();
	logF("Initialized config manager (4/4)");

	Hooks::Enable();
	TabGui::init();
	ClickGui::init();
	HudEditor::init();
	ConfigManagerMenu::init();

	logF("Hooks enabled");

	std::thread countThread([] {
		while (isRunning) {
			Sleep(1000);
			g_Data.fps = g_Data.frameCount;
			g_Data.frameCount = 0;
			g_Data.cpsLeft = g_Data.leftclickCount;
			g_Data.leftclickCount = 0;
			g_Data.cpsRight = g_Data.rightclickCount;
			g_Data.rightclickCount = 0;
		}
		});
	countThread.detach();
	//Sleep(2000);
	Utils::systemPlay("inject.wav");
	logF("Count thread started");

	ExitThread(0);
}
void DownloadAllFile() {
	Utils::makeFolderInRoaming("AuraV4");
	Utils::makeFolderInPacket("Configs");
	Utils::makeFolderInPacket("Assets");
	std::string SoundFolder = (getenv("AppData") + (std::string)"\\..\\Local\\Packages\\Microsoft.MinecraftUWP_8wekyb3d8bbwe\\RoamingState\\AuraV4\\Assets\\");
	downloadFile("Sigma_Enable.wav", "https://github.com/ReaQwQ/cdn.nuke.pl.moment/raw/main/Sigma_Enable.wav", SoundFolder);
	downloadFile("Sigma_Disable.wav", "https://github.com/ReaQwQ/cdn.nuke.pl.moment/raw/main/Sigma_Disable.wav", SoundFolder);
	downloadFile("inject.wav", "https://github.com/ReaQwQ/cdn.Nuke.pl.moment/raw/main/inject.wav", SoundFolder);
	downloadFile("uninject.wav", "https://github.com/ReaQwQ/cdn.Nuke.pl.moment/raw/main/uninject.wav", SoundFolder);
};
BOOL __stdcall DllMain(HMODULE hModule,
	DWORD ul_reason_for_call,
	LPVOID) {
	switch (ul_reason_for_call) {
	case DLL_PROCESS_ATTACH: {
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)DownloadAllFile, hModule, NULL, NULL);
		CreateThread(NULL, NULL, (LPTHREAD_START_ROUTINE)start, hModule, NULL, NULL);
		DisableThreadLibraryCalls(hModule);
	} break;


	case DLL_PROCESS_DETACH:
		isRunning = false;

		SettingMgr->saveSettings();
		configMgr->saveConfig();
		moduleMgr->disable();
		cmdMgr->disable();
		Hooks::Restore();

		logF("Removing logs");
		Logger::Disable();

		MH_Uninitialize();
		delete SettingMgr;
		delete moduleMgr;
		delete cmdMgr;
		delete configMgr;
		if (g_Data.getLocalPlayer() != nullptr) {
			auto guiData = g_Data.getClientInstance()->getGuiData();
			if (guiData != nullptr && !GameData::shouldHide())
				guiData->displayClientMessageF("%sEjected!", RED);
		}
		break;
	}
	return TRUE;
}
