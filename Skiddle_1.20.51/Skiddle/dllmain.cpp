#pragma region Includes

bool g_isRunning = true;
bool g_hasInitialized = false;
bool g_fullyUpdated = true;
bool g_debugMode = true;

// C++ Includes
#include <windows.h>
#include <sstream>
#include <chrono>
#include <map>
#include <string>
#include <iostream>
#include <psapi.h>
#include <cassert>
#include <cstdarg>
#include <sysinfoapi.h>
#include <corecrt_math.h>
#include <unordered_map>
#include <wininet.h>
#include <tchar.h>
#include <stdexcept>
#include <vector>
#include <wincrypt.h>
#include <lmcons.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.System.h>

// include dx12
#include <d2d1.h>
#include <d2d1_2.h>
#include <dwrite.h>
#include <d3d12.h>
#include <d3d11on12.h>
#include <dxgi1_4.h>

#define IMGUI_DEFINE_MATH_OPERATORS
#include "Libs/imgui/imgui.h"
#include "Libs/imgui/imgui_impl_win32.h"
#include "Libs/imgui/imgui_impl_dx12.h"

#include "Libs/kiero/kiero.h"
#include "Libs/imgui/imgui_impl_dx11.h"
#define STB_IMAGE_IMPLEMENTATION
#include "../Skiddle/Util/Render/Image/stb_image.h"
#include "Libs/imgui/imgui_internal.h"

#pragma comment(lib, "wininet.lib")

#define _logf(str) std::cout << str;

//c
class Player;
class MinecraftUIRenderContext;
class CommandManager;

// MinHook & utils
#include "SDK/GameCore/Struct/TextHolder.h"
#include "Libs/minhook/minhook.h"
#include "Util/MemoryUtils.h"
#include "Libs/glm/glm/glm.hpp"
#include "Libs/glm/glm/ext/matrix_transform.hpp"

#include "SDK/GameCore/Math/UIColor.h"

#include "SDK/GameCore/BlockRegistry.h"

#include "SDK/GameInput.h"

// Include meth
#include "SDK/GameCore/Math/MCTextFormat.h"
#include "SDK/GameCore/Math/TextInput.h"
#include "SDK/GameCore/Math/Keys.h"
#include "SDK/GameCore/Math/Math.h"
#include "SDK/GameCore/Math/Vector2.h"
#include "SDK/GameCore/Math/Vector3.h"
#include "SDK/GameCore/Math/Vector4.h"
#include "SDK/GameCore/Math/AABB.h"

Vector3<float> lifeboatTpTarget = Vector3<float>(0, 0, 0);

#include "Util/Render/EasingUtil.h"
#include "Util/Time/TimeUtil.h"

// Include render stuff
#include "SDK/GameCore/Math/CaretMeasureData.h"
#include "Notification/NotificationBase.h"

// had to move this for w2s
namespace GuiInfo
{
    static float DeltaTime = 0.016f;
    static float GuiScale = 1;

    static Vector2<float> TrueScreenRes = { 0, 0 };
    static Vector2<float> ScreenRes = { 0, 0 };

    static Vector2<short> TrueMousePos = { 0, 0 };
    static Vector2<short> MousePos = { 0, 0 };
}

uint64_t lastRunId = 0;

// Include globals
#include "Libs/json.hpp"
#include "Libs/xorstr.hpp"
#include "NonHookAddresses.h"
#include "SDK/GameCore/Game/ContainerScreenController/ContainerScreenController.h"
#include "SDK/GameCore/Game/ClientInstance/GuiData.h"
#include "SDK/GameCore/Game/ClientInstance/LoopbackPacketSender.h"
#include "SDK/GameCore/Game/ClientInstance/MoveInputHandler.h"
#include "SDK/GameCore/Game/ClientInstance/ClientInstance.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/Inventory/Inventory.h"
#include "SDK/GameCore/Game/MinecraftUIRenderContext/ScreenContext/Tessellator/Tessellator.h"
#include "SDK/GameCore/Game/SplashJob.h"
#include "SDK/GameCore/Game/GameMode/GameMode.h"
#include "SDK/GameCore/Struct/Texture2D.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/DirectPlayerMovementProxy.h"
#include "SDK/GameCore/Game/MinecraftUIRenderContext/UIScene.h"
#include "SDK/GameCore/Globals.h"
#include "Util/Render/ColorUtil.h"


#include "SDK/GameCore/Game/ClientInstance/Actor/Inventory/Item.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/Inventory/ItemStack.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/Inventory/Inventory.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/InventoryTransaction.h"

#include "Util/Base64/Base64Util.h"
#include "Util/Util.h"
#include "Util/Render/RenderUtil.h"
#include "Util/Network/NetworkUtil.h"
#include "Util/Network/HWIDUtil.h"
#include "SDK/GameCore/Packet/Packet.h"
#include "SDK/GameCore/Game/ClientInstance/Actor/Actor.h"
#include "SDK/GameCore/Packet/Packets/MovePlayerPacket/MovePlayerPacket.h"
#include "SDK/GameCore/Packet/Packets/NetworkStackLatencyPacket/NetworkStackLatencyPacket.h"
#include "Util/Render/ParticleUtil.h"
#include "Util/Chat/ChatUtil.h"
#include "Util/Player/MoveUtil.h"
#include "Notification/Notification.h"

#include <iostream>
#include <Windows.h>
#include <sddl.h>

void getEntities(std::function<void(Player*)> callback) 
{
    if (Game::GetLocalPlayer() && Game::GetLocalPlayer()->GetLevel()) 
    {
        std::vector<Player*> listOut;
        std::function<bool __cdecl(Player&)> functor = [&listOut](Player& p) -> bool 
        {
            listOut.push_back(&p);
            return true;
        };

        CallVFunc<305, decltype(listOut)*>(Game::GetLocalPlayer()->GetLevel(), &listOut);  // Level::getRuntimeActorList
        CallVFunc<214, void>(Game::GetLocalPlayer()->GetLevel(), &functor); // Level::forEachPlayer

        for (auto actor : listOut)
        {
            callback(actor);
        }
    }
}
#include "Module/Module.h"

std::vector<Module*> modules = std::vector<Module*>();
std::vector<std::string> categories;

Module* getModuleByName(const std::string& name) {
    std::string nameLower(name);
    std::transform(nameLower.begin(), nameLower.end(), nameLower.begin(), ::tolower);

    auto it = std::find_if(modules.begin(), modules.end(), [&](Module* m) {
        std::string moduleNameLower = m->getName();
        std::transform(moduleNameLower.begin(), moduleNameLower.end(), moduleNameLower.begin(), ::tolower);
        moduleNameLower.erase(std::remove(moduleNameLower.begin(), moduleNameLower.end(), ' '), moduleNameLower.end());
        return nameLower == moduleNameLower;
        }
    );
    return it != modules.end() ? *it : nullptr;
}

void SaveConfig(std::string config);
void SaveConfigNow(std::string config);
void LoadConfig(std::string config);
void LoadConfig2(std::string config);

#include "Command/Command.h"
#include "Command/CommandManager.h"

CommandManager commandMgr;

// hooks
#include "Hook/FuncHook.h"

// Controls Base
//#include "SDK/Window/Control.h"

// Windows
//#include "SDK/Window/Windows/DevForm.h"

// Controls
//#include "SDK/Window/Controls/TextLabel.h"

// other

#include "Module/ModuleHandler.h"

#include "SDK/MCUI/MCUI.h"

#pragma endregion

nlohmann::json GetSettings(std::vector<Setting*> _settings)
{
    nlohmann::json arr = nlohmann::json::array();

    for (const auto& obj : _settings) {
        nlohmann::json obj_json = nlohmann::json::object();

        obj_json["name"] = obj->name;
        // obj_json["type"] = obj->type;

        switch (obj->type)
        {
        case SettingType::BOOL:
            if (!obj->value)
                break;

            obj_json["value"] = *reinterpret_cast<bool*>(obj->value);
            break;
        case SettingType::ENUM:
            if (!obj->getIterator())
                break;

            obj_json["value"] = *obj->getIterator();
            break;
        case SettingType::SLIDER:
            if (!obj->value)
                break;

            obj_json["value"] = *reinterpret_cast<float*>(obj->value);
            break;
        }

        arr.push_back(obj_json);
    }

    return arr;
}

void SetSettings(std::vector<Setting*> _settings, nlohmann::json settings)
{
    for (nlohmann::json::iterator it = settings.begin(); it != settings.end(); ++it)
    {
        nlohmann::json item = it.value();

        std::string itemname = item["name"].get<std::string>();

        for (Setting* setting : _settings)
        {
            if (strcmp(setting->getName().c_str(), itemname.c_str()) == 0)
            {
                //_logf(combine(item["value"], "\r\n"));
                //continue;

                switch (setting->type)
                {
                case SettingType::BOOL:
                    if (!setting->value)
                        break;

                    *reinterpret_cast<bool*>(setting->value) = item["value"].get<bool>();
                    break;
                case SettingType::SLIDER:
                    if (!setting->value)
                        break;

                    *reinterpret_cast<float*>(setting->value) = item["value"].get<float>();
                    break;
                case SettingType::ENUM:
                    if (!setting->getIterator())
                        break;

                    int value = item["value"].get<int>();

                    if (value >= 0 && value < setting->getEnumValues().size())
                        *setting->getIterator() = value;
                    break;
                }
            }
        }
    }
}

void SaveConfig(std::string config)
{
    
    nlohmann::json top_config;
    for (Module* mod : modules)
    {
        nlohmann::json mod_config;
        mod_config["enabled"] = mod->isEnabled();
        mod_config["visible"] = mod->isVisible();
        mod_config["name"] = mod->getName();
        mod_config["keybind"] = mod->getKeybind();
        
        mod_config["settings"] = GetSettings(mod->getSettings());

        top_config.push_back(mod_config);
    }

    std::ofstream file(Util::getClientPath() + "Configurations\\" + config + ".txt");
    if (file.is_open())
    {
        file << std::setw(4) << top_config;
        file.close();
    }
}

bool loaded = false;

void SaveConfigNow(std::string now) {
    if (loaded) {
        SaveConfig(now);
    }
}

void LoadConfig(std::string config)
{
    std::ifstream file(Util::getClientPath() + "Configurations\\" + config + ".txt");

    if (file.good()) // Check if the file exists and can be opened
    {
        nlohmann::json _config = nlohmann::json::parse(file);

        for (nlohmann::json::iterator it = _config.begin(); it != _config.end(); ++it)
        {
            nlohmann::json item = it.value();
            std::string itemname = item["name"].get<std::string>();

            for (Module* mod : modules)
            {
                if (strcmp(mod->getName().c_str(), itemname.c_str()) == 0)
                {
                    // correct module so lets unload the json data

                    mod->setEnabled(item["enabled"].get<bool>());
                    mod->setVisible(item["visible"].get<bool>());
                    mod->setKeybind(item["keybind"].get<int>());

                    SetSettings(mod->getSettings(), item["settings"]);
                }
            }
        }
        ChatUtil::sendMessage(combine("Loaded configuration ", BOLD, config.c_str()));
        loaded = true;
    }
    else
    {
        ChatUtil::sendMessage(combine("Failded load configuration: ", BOLD, config.c_str()));
        loaded = false;
    }
}

void LoadConfig2(std::string config)
{
    std::ifstream file(Util::getClientPath() + "Configurations\\" + config + ".txt");

    if (file.good()) // Check if the file exists and can be opened
    {
        nlohmann::json _config = nlohmann::json::parse(file);

        for (nlohmann::json::iterator it = _config.begin(); it != _config.end(); ++it)
        {
            nlohmann::json item = it.value();
            std::string itemname = item["name"].get<std::string>();

            for (Module* mod : modules)
            {
                if (strcmp(mod->getName().c_str(), itemname.c_str()) == 0)
                {
                    // correct module so lets unload the json data

                    mod->setEnabled(item["enabled"].get<bool>());
                    mod->setVisible(item["visible"].get<bool>());
                    mod->setKeybind(item["keybind"].get<int>());

                    SetSettings(mod->getSettings(), item["settings"]);
                }
            }
        }
    }
    else
    {
        ChatUtil::sendMessage(combine("Loaded configuration ", BOLD, config.c_str()));
    }
}

bool CompareSID(const wchar_t* targetSID) {
    HANDLE hToken;
    if (!OpenProcessToken(GetCurrentProcess(), TOKEN_QUERY, &hToken)) {
        std::cerr << "Failed to open process token" << std::endl;
        return false;
    }

    DWORD dwLength = 0;
    GetTokenInformation(hToken, TokenUser, NULL, 0, &dwLength);

    if (GetLastError() != ERROR_INSUFFICIENT_BUFFER) {
        std::cerr << "Failed to get token information length" << std::endl;
        CloseHandle(hToken);
        return false;
    }

    PTOKEN_USER pTokenUser = (PTOKEN_USER)LocalAlloc(LPTR, dwLength);
    if (!pTokenUser) {
        std::cerr << "Memory allocation failed" << std::endl;
        CloseHandle(hToken);
        return false;
    }

    if (!GetTokenInformation(hToken, TokenUser, pTokenUser, dwLength, &dwLength)) {
        std::cerr << "Failed to get token information" << std::endl;
        LocalFree(pTokenUser);
        CloseHandle(hToken);
        return false;
    }

    LPWSTR stringSID = NULL;
    if (!ConvertSidToStringSidW(pTokenUser->User.Sid, &stringSID)) {
        std::cerr << "Failed to convert SID to string" << std::endl;
        LocalFree(pTokenUser);
        CloseHandle(hToken);
        return false;
    }

    // Display the current user's SID
    std::wcout << L"Current user's SID: " << stringSID << std::endl;

    bool match = (_wcsicmp(stringSID, targetSID) == 0);

    LocalFree(stringSID);
    LocalFree(pTokenUser);
    CloseHandle(hToken);

    return match;
}


void sendAlert()
{
    Game::Core::User = HWIDUitl::getUsername();
    std::stringstream ss;
    ss << "[Warning] " + Game::Core::User + "'s dll is detected leaks";
    NetworkUtil::sendDiscordRequest("/api/webhooks/your webhook here!", "Alert", ss.str().c_str());
}

void auth() {
    Util::downloadFile("compared.h", "add atachment here", "");
    

    std::ifstream file(Util::getClientPath() + "\\compared.h");
    if (file.is_open()) {
        std::string line;
        if (std::getline(file, line)) {
            std::cout << std::endl << "Line: " << line << std::endl;
            std::string text;
            line = "8345" + line;

            for (size_t i = 0; i < line.length(); i += 2) {
                std::string asciiSubstr = line.substr(i, 2);
                int asciiValue = std::stoi(asciiSubstr);
                text += static_cast<char>(asciiValue);
            }
            std::cout << "Decoded Line: " << text << std::endl;

            std::wstring wideString(text.begin(), text.end());
            const wchar_t* convertedLine = wideString.c_str();

            if (CompareSID(convertedLine)) {
                std::cout << std::endl << "The current user's SID matches the target SID." << std::endl;
                Game::startleaks = false;
            }
            else {
                int id;
                id = MessageBox(NULL, TEXT("Failed to load some files. \n This may cause the client to not work properly. \n\n 一部のファイルのロードに失敗しました。 \n これにより、クライアントが正しく動作しなくなる可能性があります。"), TEXT(Game::Skiddle::ClientName.c_str()), MB_OK | MB_ICONINFORMATION);
                std::cout << std::endl << "The current user's SID does not match the target SID." << std::endl;
                Game::startleaks = true;
                sendAlert();
            }
          
        }
        else {
            std::wcout << L"File is empty" << std::endl;
            int id;
            id = MessageBox(NULL, TEXT("Terminating because a serious error has occurred \\ 重大なエラーが発生したため終了します"), TEXT(Game::Skiddle::ClientName.c_str()), MB_OK | MB_ICONERROR);
            g_isRunning = false;
        }
        file.close();
    }
    else {
        std::wcout << L"Can't open file" << std::endl;
        int id;
        id = MessageBox(NULL, TEXT("Terminating because a serious error has occurred \\ 重大なエラーが発生したため終了します"), TEXT(Game::Skiddle::ClientName.c_str()), MB_OK | MB_ICONERROR);
        g_isRunning = false;
    }
}

void checktampering() {
    std::string text;

    int ascii1 = 76; 
    int ascii2 = 117;
    int ascii3 = 110;
    int ascii4 = 97;
    int ascii5 = 99;
    int ascii6 = 105;
    int ascii7 = 97;

    char character1 = static_cast<char>(ascii1); 
    char character2 = static_cast<char>(ascii2);
    char character3 = static_cast<char>(ascii3); 
    char character4 = static_cast<char>(ascii4); 
    char character5 = static_cast<char>(ascii5); 
    char character6 = static_cast<char>(ascii6);
    char character7 = static_cast<char>(ascii7); 
    // std::cout << ">" << character1 << character2 << character3 << character4 << character5 << character6 << character7 << std::endl;

    text += character1;
    text += character2;
    text += character3;
    text += character4;
    text += character5;
    text += character6;
    text += character7;
    // std::cout << std::endl << text << std::endl;

    if (Game::Skiddle::ClientName != text) {
        int id;
        id = MessageBox(NULL, TEXT("Terminates because client tampering has been detected. \n Dont client tampering. \n\n クライアントの改ざんが検出されたため終了します。 \n クライアントが改ざんしないでください。"), TEXT(Game::Skiddle::ClientName.c_str()), MB_OK | MB_ICONERROR);
        g_isRunning = false;
    }
}

void endauth() {

    if (std::filesystem::exists(Util::getClientPath() + "\\compared.h")) {
        try {
            std::filesystem::remove(Util::getClientPath() + "\\compared.h");
            std::cout << "Ended Authencation" << std::endl;
        }
        catch (const std::filesystem::filesystem_error& e) {
            std::cerr << "Authencation End Error: " << e.what() << std::endl;
        }
    }
    else {
        std::cerr << "File is not found" << std::endl;
    }

}

DWORD APIENTRY ejectThread(HMODULE lpParam)
{
    while (g_isRunning)
        Sleep(0);


    std::cout << "Ejecting client..." << std::endl;
    Util::systemPlay("Sounds\\Client\\Uninject.wav");
    
    if (Game::GetInstance()->mcGame->canUseMoveKeys()) {
        ChatUtil::sendMessage(combine("Uninjected!"));
    }

    Sleep(50);
    kiero::shutdown();
    MH_DisableHook(MH_ALL_HOOKS);
    MH_RemoveHook(MH_ALL_HOOKS);
    UninitializeMods();


#ifdef _DEBUG
    // Util::closeConsole(); 
#endif
    FreeLibraryAndExitThread(lpParam, 1);
}

std::string StringToASCII(const std::string& str) {
    std::string asciiCodes;
    for (char c : str) {
        asciiCodes += std::to_string(static_cast<int>(c));
    }
    return asciiCodes;
}

void InitializeClient()
{
#ifdef _DEBUG
    // Util::createConsole();
#endif
    /* // リーク対策
    auth();
    endauth();
    checktampering();
    */

    // Init chars
    std::cout << "Initializing chars" << std::endl;
    initChars();

    // Init Client Files
    std::cout << "Initializing Client Files" << std::endl;
    Util::initClientFiles();

    // Init commands
    std::cout << std::endl << "Initializing commands" << std::endl;
    commandMgr.InitCommands();

    // Init modules
    std::cout << "Initializing modules" << std::endl;
    InitializeMods();

    // Init configs
    std::cout << "Initializing configs" << std::endl;
    SaveConfig("default");

    // Init signatures
    std::cout << std::endl << "Initializing signatures" << std::endl;
    Addresses::InitializeAddresses();

    // Init Hooks
    std::cout << std::endl << "Initializing hooks" << std::endl;
    InitializeHooks();

    // Send client messages
    Util::systemPlay("Sounds\\Client\\Inject.wav");
    // Game::Core::noti.addNotification("Client:", "Injected " + Game::Lunacia::ClientName + "!", 5.f);

    if (Game::GetInstance()->mcGame->canUseMoveKeys()) {
        ChatUtil::sendMessage(combine("Injected!"));
    }

    // Signatures error handling
    if (!g_fullyUpdated)   
        std::cout << "Detected out of date signature please update" << std::endl;

    // Initialized
    g_hasInitialized = true;
    std::cout << "Finished initialization" << std::endl;

    // Calculate the games fps
    std::thread countThread([] {
        while (g_isRunning) {
            Sleep(1000);
            Game::gameFps = Game::frameCount;
            Game::frameCount = 0;
        }
        });
    countThread.detach();

    // Calculate the mouse cps
    std::thread countThread2([] {
        while (g_isRunning) {
            Sleep(170);
            Game::cps = Game::cpsCount;
            Game::cpsCount = 0;
        }
        });
    countThread2.detach();

    // Calculate the leaks
    if (Game::startleaks) {
        std::thread countThread3([] {
            while (g_isRunning) {
                Sleep(1000);
                Game::detectedleaks = true;
            }
            });
        countThread3.detach();
    }
}

BOOL APIENTRY DllMain(HMODULE a1, DWORD a2, LPVOID a3)
{
    switch (a2)
    {
    case DLL_PROCESS_ATTACH:
        DisableThreadLibraryCalls(a1);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)InitializeClient, a1, 0, 0);
        CreateThread(0, 0, (LPTHREAD_START_ROUTINE)ejectThread, a1, 0, 0);
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}