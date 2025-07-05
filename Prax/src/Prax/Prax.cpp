//
// Created by Flash on 6/10/2023.
//
#pragma once

#include "Prax.h"

#include <gzip.h>
#include <thread>
#include <kiero.h>
#include <Psapi.h>

#include "ExceptionHandler.h"
#include <Prax/UI/ThemeManager.h>
#include "Prax/Hook/HookManager.h"
#include <Prax/Utils/Render/Image.h>
#include <Prax/Module/ModuleManager.h>
#include <Prax/Command/CommandManager.h>
#include <Prax/Utils/System/ToastManager.h>
#include <Prax/Scripting/ScriptManager.h>
#include <windows.h>
#include <cryptopp/files.h>

#include "Config/ConfigManager.h"
#include "SDK/Game/Internal/MinecraftGame.h"
#include "UI/Notifications.h"
#include "zlib.h"
#include <Prax/Hook/Hooks/D3D.h>
#include <winrt/Windows.UI.ViewManagement.h>
#include <winrt/Windows.ApplicationModel.Core.h>
#include <winrt/Windows.UI.Core.h>




HMODULE Prax::ModuleHandle;
bool Prax::Initialized;
bool Prax::ShouldUIRender = true;
Preferences* Prax::Preferences = nullptr;
std::string Prax::ClientName = "catgirl";
uint64_t Prax::InjectTime;


void Prax::Init() {
    // ReSharper disable once CppDFAUnusedValue
    uint64_t startTime = Misc::GetCurrentMs();

    Logger::Init();
    ExceptionHandler::Init();
    InjectTime = Misc::GetCurrentMs();

    Logger::Write(xorstr_("Init"), xorstr_("Initializing..."));
    Logger::Write(xorstr_("Init"), xorstr_("Loading file system..."));
    FileSystem::Init();

    auto w = winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(L"Catgirl Client");
    });

    Logger::Write(xorstr_("Init"), xorstr_("Loading settings..."));
    Preferences = PrefManager::Load();

    if (Preferences->ClientName != "") {
        ClientName = Preferences->ClientName;
    }


    // Check the current process module and make sure more than 120 modules are loaded
    // While there is less than 120 modules loaded, just wait

    Logger::Write(xorstr_("Init"), xorstr_("Loading modules..."));

    while (Misc::GetModuleCount() < 140) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));
    }
    Logger::Write(xorstr_("Init"), xorstr_("Modules loaded."));

    Logger::Write(xorstr_("Init"), xorstr_("Searching for sigs..."));
    SigManager::Init();

    ClientInstance* ci = MinecraftGame::GetInstance()->getPrimaryClientInstance();

    if (!ci) {
        Logger::Write(xorstr_("Init"), xorstr_("ClientInstance is null!"), Logger::LogType::Error);
    }

    Logger::Write(xorstr_("Init"), xorstr_("ClientInstance found at ") + String::ToHex((uintptr_t)ci), Logger::LogType::Debug);

    Minecraft::ClientInstance = ci;


    Logger::Write(xorstr_("Init"), xorstr_("Initializing MinHook..."));
    if (MH_Initialize() == MH_OK) {
        Logger::Write(xorstr_("Init"), xorstr_("MinHook Initialized"));


        Logger::Write(xorstr_("Init"), xorstr_("Initializing hooks..."));
        HookManager::Init();


        Logger::Write(xorstr_("Init"), xorstr_("Initializing modules..."));
        ModuleManager::Init();

        Logger::Write(xorstr_("Init"), xorstr_("Initializing commands..."));
        CommandManager::Init();

    } else {
        Logger::Write(xorstr_("Init"), xorstr_("MinHook failed to initialize!"), Logger::LogType::Error);
    }

    Logger::Write(xorstr_("Init"), xorstr_("Initializing notifications..."));
    ToastManager::Init();


    Logger::Write(xorstr_("Init"), xorstr_("Initialized Catgirl!"));
    uint64_t endTime = Misc::GetCurrentMs();
    uint64_t timeTaken = endTime - startTime;
    Logger::Write(xorstr_("Init"), xorstr_("Initialization took ") + std::to_string(timeTaken) + xorstr_("ms"));

    ThemeManager::Init();

    Minecraft::ClientInstance->playUI("beacon.activate", 1, 1.0f);

    Initialized = true;

    //Logger::Write(xorstr_("Init"), xorstr_("Waiting for LP to initialize..."));

    while (!Minecraft::ClientInstance->ThePlayer()) {
        std::this_thread::sleep_for(std::chrono::milliseconds(1));

        if (!Initialized) // If eject is wanted or auth is not running
        {
            goto label;
        }
    }

    if (Minecraft::ClientInstance->ThePlayer()) {
        Minecraft::ClientInstance->ThePlayer()->addChatMessage(xorstr_("&aInjected!&r"), true);

        for (auto hook : HookManager::Hooks){
            if (!hook->InitOnConstruct) {
                Logger::Write(xorstr_("Init"), xorstr_("Initializing ") + hook->Name + xorstr_("..."), Logger::Debug);
                hook->Init();
            }
        }
    }

    Sleep(100);

    if (Minecraft::ClientInstance->ThePlayer() && Preferences->DefaultConfig == xorstr_("")) {
    } else {
        // Check if the config exists. If it doesn't, tell the user.
        std::string path = FileSystem::ConfigDirectory + "\\" + Preferences->DefaultConfig + ".json";
        if (!std::filesystem::exists(path)) {
            Logger::Write(xorstr_("Init"), xorstr_("No default config is selected."));
            Notifications::Notify(Prax::ClientName, "Your default config was not found!!", NotificationType::Error, 10.f);
            Preferences->DefaultConfig = xorstr_("");
        } else
        {
            ConfigManager::Load(Preferences->DefaultConfig);
        }

        /*auto irc = ModuleManager::GetModuleByName(xorstr_("IRC"));
        if (irc && !irc->Enabled) irc->Toggle();*/
    }

    // ReSharper disable once CppDFAConstantConditions
    // ReSharper disable once CppDFAEndlessLoop
    while (Initialized) {
        std::this_thread::sleep_for(std::chrono::milliseconds(25));
    }

    label:

    // ReSharper disable once CppDFAUnreachableCode

    Logger::Write(xorstr_("Shutdown"), xorstr_("Shutting down..."));

    winrt::Windows::ApplicationModel::Core::CoreApplication::MainView().CoreWindow().Dispatcher().RunAsync(winrt::Windows::UI::Core::CoreDispatcherPriority::Normal, []() {
        winrt::Windows::UI::ViewManagement::ApplicationView::GetForCurrentView().Title(L"");
    });


    Logger::Write(xorstr_("Shutdown"), xorstr_("Saving settings..."));
    PrefManager::Save(Preferences);

    if (Minecraft::ClientInstance->ThePlayer())
        Minecraft::ClientInstance->ThePlayer()->addChatMessage(xorstr_("&cEjected!&r"), true);
    Minecraft::ClientInstance->playUI("beacon.deactivate", 1, 1.0f);

    // Disable all modules
    Logger::Write(xorstr_("Shutdown"), xorstr_("Disabling modules..."));
    ConfigManager::Loading = true; // prevent any toggle sounds
    ModuleManager::Shutdown();

    // Unload all hooks
    Logger::Write(xorstr_("Shutdown"), xorstr_("Unloading all hooks..."));
    HookManager::Shutdown();

    Image::Shutdown();

    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    MH_Uninitialize();
    kiero::shutdown();


    Sleep(1000);
    Logger::Shutdown();

    FreeLibraryAndExitThread(ModuleHandle, 1);
}

void Prax::Shutdown()
{
    // this is called when the process is closed
    Logger::Write(xorstr_("Shutdown"), xorstr_("Shutting down..."));

    Logger::Write(xorstr_("Shutdown"), xorstr_("Saving settings..."));
    PrefManager::Save(Preferences);

}
