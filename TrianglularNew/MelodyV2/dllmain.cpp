#include <Windows.h>
#include "Client/Client.h"
#include "SDK/GameData.h"
#include "Client/SplashScreen.h"
#pragma comment(lib, "libhat.lib")
static int getCurrentMs1() {
    using namespace std::chrono;
    return duration_cast<milliseconds>(steady_clock::now().time_since_epoch()).count();
}
DWORD WINAPI setUpClient(LPVOID lpParameter) {



    client->init();

    while (true) {
        if ((mc.isKeyDown(VK_CONTROL) && mc.isKeyDown('L')) || mc.isKeyDown(VK_END)) break;
        Sleep(32);
    }
    delete client;
    FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
    ExitThread(0);
    SplashScreen::startMs = getCurrentMs1();
    SplashScreen::showSplash = true;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)setUpClient, hModule, 0, nullptr);
    }
    return TRUE;
}