#pragma once

#include <iostream>
#include <thread>



bool __stdcall DllMain(const HINSTANCE hinstDLL, const DWORD fdwReason, [[maybe_unused]] LPVOID lpReserved) {
    if(fdwReason == DLL_PROCESS_ATTACH) {
        DisableThreadLibraryCalls(hinstDLL);
        Prax::ModuleHandle = hinstDLL;
        CreateThread(nullptr, 0, reinterpret_cast<LPTHREAD_START_ROUTINE>(Prax::Init), nullptr, 0, nullptr);
    }
    return true;  // Successful DLL_PROCESS_ATTACH
}
