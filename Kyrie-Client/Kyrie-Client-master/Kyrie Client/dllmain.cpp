#include <Windows.h>
#include "Client/Client.h"
#include "ImFX/imfx.h"

DWORD WINAPI initClient(LPVOID lpParameter) {

    client->init();

    while (client->isInitialized()) {
        Sleep(1000);
    }
    Sleep(250);

    HookManager::RestoreAll();
    Sleep(50);
    ImFX::CleanupFX();

    delete client->moduleMgr;
    delete client;

    FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
    return 0;
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD  ul_reason_for_call, LPVOID lpReserved) {
    /*
    switch (ul_reason_for_call)
    {
    case DLL_PROCESS_ATTACH:
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
    case DLL_PROCESS_DETACH:
        break;
    }
    */
    if (ul_reason_for_call == DLL_PROCESS_ATTACH) {
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initClient, hModule, 0, 0);
    }
    return TRUE;
}

