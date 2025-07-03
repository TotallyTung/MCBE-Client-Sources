// Created by Tony on 2024-10-10 10:01:54


#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.UI.Core.h>
#include <functional>
#include <Windows.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <ostream>
#include <cstdarg>
#include <string>
#include <bitset>
#include <vector>
#include <chrono>
#include <thread>
#include <array>
#include <mutex>
#include <map>
#include <dxgi.h>
#include <dxgi1_4.h>
#include <d3d11.h>
#include <d3d12.h>
#include <d2d1_3.h>
#include "Client/Client.h"
#include "SDK/GameData.h"
#include "Utils/AudioSystem.h"
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
    AudioSystem::getInstance().play(xorstr_("Music\\inject.wav"));
    winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(L"MelodyV2");
        });
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