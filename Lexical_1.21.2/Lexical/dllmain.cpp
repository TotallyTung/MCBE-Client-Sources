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

DWORD WINAPI initClient(LPVOID lpParameter) {
    setWindowTitle(L"");

    Client::init();

    setWindowTitle(L"Lexical Client");

    while (Client::isInitialized()) {
        ModuleManager::onClientTick();
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

        Client::DisplayClientMessage("%sEjected", MCTF::RED);
        break;
    }
    return TRUE;
}

