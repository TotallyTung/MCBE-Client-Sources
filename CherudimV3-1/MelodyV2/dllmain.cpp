#include <Windows.h>
#include "Client/Client.h"
#include "SDK/GameData.h"
#pragma comment(lib, "libhat.lib")

DWORD WINAPI setUpClient(LPVOID lpParameter) {

    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* f;

    SetConsoleTitleA("Melo Console");

    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONOUT$", "w", stdout);

    printf("Test Message *******************");

    client->init();

    while (true) {
        if ((mc.isKeyDown(VK_CONTROL) && mc.isKeyDown('L')) || mc.isKeyDown(VK_END)) break;
        Sleep(25);
    }
    delete client;
    FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
    ExitThread(0);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(nullptr, 0, (LPTHREAD_START_ROUTINE)setUpClient, hModule, 0, nullptr);
    }
    return TRUE;
}