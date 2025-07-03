#include <Windows.h>
#include "Client/Client.h"
#include "Renderer/D2D.h"
#include "Renderer/MCR.h"
#include "winrt/Windows.UI.Core.h"
#include "winrt/windows.system.h"
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include "winrt/windows.applicationmodel.core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.Foundation.h"

void setWindowTitle(const std::wstring& title) {
    winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().DispatcherQueue().TryEnqueue([title]() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(title);
        });
}

std::wstring getCompileTime() {
    std::wstring compileTime = L"Compiled on " + std::wstring(__DATE__, __DATE__ + strlen(__DATE__)) +
        L" at " + std::wstring(__TIME__, __TIME__ + strlen(__TIME__));
    return compileTime;
}

DWORD WINAPI initClient(LPVOID lpParameter) {
    setWindowTitle(L"");
    Client::init();
    std::wstring compileTime = getCompileTime();
#ifdef _DEBUG
    setWindowTitle(L"strataware v0.0.1 - 1.21.2.2 - debug - " + compileTime);
#else
    setWindowTitle(L"strataware v0.0.1 - 1.21.2.2 - release - " + compileTime);
#endif
    while (Client::isInitialized()) {
        ModuleManager::onPlayerTick();
        Sleep(50);
    }
    Sleep(25);
    FreeLibraryAndExitThread((HMODULE)lpParameter, 1);
}

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call, LPVOID lpReserved) {
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH:
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)initClient, hModule, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        HookManager::shutdown();
        ModuleManager::shutdown();
        CommandManager::shutdown();
        D2D::Clean();
        break;
    }
    return TRUE;
}
