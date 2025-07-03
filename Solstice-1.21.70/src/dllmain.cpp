#pragma once

#include <iostream>
#include <Solstice.hpp>
#include <thread>
#include <Windows.h>
#include <Utils/Logger.hpp>
#include <winrt/base.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>

bool __stdcall DllMain(const HMODULE hModule, const DWORD fdwReason, [[maybe_unused]] LPVOID lpReserved) {
    if(fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hModule);
        winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, [hModule]()
        {
            Solstice::init(hModule);
        });
    }
    return true;  // Successful DLL_PROCESS_ATTACH
}
