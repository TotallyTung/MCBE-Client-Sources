#include "dllmain.h"
#include <mmsystem.h>
#pragma comment(lib, "winmm.lib")

static inline float DayTimeValue = 1;

static inline void addBootLog(std::string str) {
	for (auto c : str) {
		Game::RenderInfo::bootLogLOL += c;
		Sleep(10);
	}
}

void Init(HANDLE module) {
#ifdef _DEBUG
	Console::CreateConsole(Game::ClientName.c_str());
#endif
	DirectXHook::Initialize();
	initChars();
	Game::Initializing = true;
	Sleep(500);
	addBootLog("Vape Bedrock Version v4.13\n\n");
	Sleep(200);
	InitializeMods();
	InitGameData();
	InitHooks();
	Sleep(500);
	Game::Initializing = false;
	SplashScreen::startMs = getCurrentMs();
	SplashScreen::showSplash = true;
}

void ejectThread(HMODULE module)
{
	while (Game::isRunning)
		Sleep(0);

	std::cout << "Ejecting client..." << std::endl;

	Sleep(50);
	MH_DisableHook(MH_ALL_HOOKS);
	//	MH_RemoveHook(MH_ALL_HOOKS);
	MH_Uninitialize();
#ifdef _DEBUG
	Console::CloseConsole();
#endif
	FreeLibraryAndExitThread(module, 1);
}


//<-- ENTRY POINT
BOOL APIENTRY DllMain(HMODULE a1, DWORD a2, LPVOID a3)
{
	if (a2 == 1) { //Inject
		Utils::setDebugPath("Vape");
		//std::string url = "https://breeze.rip/assets/logos/logo.png";
		//std::string url
		//URLDownloadToFileA(nullptr, url.c_str(), path.c_str(), 0, NULL);
		Utils::toastNotification(Game::ClientName, "Injecting...");
		Utils::toastNotification("Authentication", "Logging in...");
		Utils::toastNotification("Authentication", "Verifying Account...");
		Utils::toastNotification("Authentication", "Succefully Logged in...");
		//std::string test = "Injected skittle wtf??";
		//ChatUtil::sendMessage(test);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)Init, a1, 0, 0);
		CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ejectThread, a1, 0, 0);
	}
	else if (a2 == 0) {//Eject
		Utils::toastNotification(Game::ClientName, "What?! plz no eject!!");
	}
	return TRUE;
}
//-->