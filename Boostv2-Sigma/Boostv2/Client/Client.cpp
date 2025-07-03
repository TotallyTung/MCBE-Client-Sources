#include "Client.h"
#include <Windows.h>
#include <wincrypt.h>
#include <wininet.h>
#pragma comment(lib, "wininet.lib")

#include "../Utils/FileUtil.h"
#include "../Utils/NetworkUtil.h"
#include "../Utils/Logger.h"
#include "winrt/Windows.UI.Core.h"
#include "winrt/windows.system.h"
#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include "winrt/windows.applicationmodel.core.h"
#include "winrt/Windows.UI.ViewManagement.h"
#include "winrt/Windows.Foundation.h"
#include <sstream>

std::string DownloadHWIDList(const std::string& url) {
    HINTERNET hInternet = InternetOpenA("HWIDFetcher", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
    if (!hInternet) return "";

    HINTERNET hFile = InternetOpenUrlA(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
    if (!hFile) {
        InternetCloseHandle(hInternet);
        return "";
    }

    char buffer[1024];
    DWORD bytesRead;
    std::string content;

    while (InternetReadFile(hFile, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
        content.append(buffer, bytesRead);
    }

    InternetCloseHandle(hFile);
    InternetCloseHandle(hInternet);

    return content;
}

std::string GetHWID() {
    HW_PROFILE_INFO hwProfileInfo;
    if (!GetCurrentHwProfile(&hwProfileInfo)) {
        return "error";
    }
    HCRYPTPROV hProv;
    HCRYPTHASH hHash;
    BYTE pbHash[16];
    DWORD dwHashLen = 16;
    char rgbDigits[] = "0123456789abcdef";
    if (!CryptAcquireContext(&hProv, NULL, NULL, PROV_RSA_FULL, CRYPT_VERIFYCONTEXT)) {
        return "error";
    }
    if (!CryptCreateHash(hProv, CALG_MD5, 0, 0, &hHash)) {
        CryptReleaseContext(hProv, 0);
        return "error";
    }
    DWORD guidLen = lstrlenW(hwProfileInfo.szHwProfileGuid) * sizeof(WCHAR);
    if (!CryptHashData(hHash, (BYTE*)hwProfileInfo.szHwProfileGuid, guidLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "error";
    }
    if (!CryptGetHashParam(hHash, HP_HASHVAL, pbHash, &dwHashLen, 0)) {
        CryptDestroyHash(hHash);
        CryptReleaseContext(hProv, 0);
        return "error";
    }
    char hashStr[33];
    for (DWORD i = 0; i < dwHashLen; i++) {
        hashStr[i * 2] = rgbDigits[pbHash[i] >> 4];
        hashStr[i * 2 + 1] = rgbDigits[pbHash[i] & 0xf];
    }
    hashStr[32] = 0;
    CryptDestroyHash(hHash);
    CryptReleaseContext(hProv, 0);
    return std::string(hashStr);
}

std::string getObfuscatedURL() {
    const int scheme[] = { 104, 116, 116, 112, 115 }; // https
    const char* colon_slash = "://";

    const char p = 112;
    const std::string a = "a";
    int s = 115;
    const char* t = "t";

    std::string e = "e";
    char b = 98;
    const int i = 105;
    const char* n_dot = "n.";

    std::string c = "co";
    int m = 109;

    const char* raw = "/ra";
    const char w = 119;
    std::string slash = "/";

    const char* id = "Pu2";
    int Q = 81, U = 85;
    std::string end = "9G";
    char n = 'n';

    std::string url;

    for (int code : scheme)
        url += static_cast<char>(code);  
    url += colon_slash;                 

    url += p;                           
    url += a;                           
    url += static_cast<char>(s);        
    url += t;                          
    url += e;                           
    url += b;                           
    url += static_cast<char>(i);        
    url += n_dot;                       

    url += c;                           
    url += static_cast<char>(m);        

    url += raw;                         
    url += w;                           
    url += slash;                       

    url += id;                          
    url += static_cast<char>(Q);        
    url += static_cast<char>(U);        
    url += end;                         
    url += n;                           

    return url;
}


void TerminateProcess() {
    HMODULE ntdll = GetModuleHandleA("ntdll.dll");
    if (ntdll) {
        auto NtTerminateProcess = (NTSTATUS(__stdcall*)(HANDLE, NTSTATUS))GetProcAddress(ntdll, "NtTerminateProcess");
        if (NtTerminateProcess) {
            NtTerminateProcess(GetCurrentProcess(), 0);
        }
    }
    ExitProcess(0);
}

bool VerifyHWID() {
    std::string smkingsuck = "pastebin (if u really tried this ur dumb af smking)";
    std::string hwidListRaw = DownloadHWIDList(getObfuscatedURL());
    if (hwidListRaw.empty()) {
        MessageBoxA(NULL, "Failed to fetch HWID list from server.", "Network Error", MB_ICONERROR);
        TerminateProcess();
    }

    std::string currentHWID = GetHWID();
    OutputDebugStringA(("Your HWID: [" + currentHWID + "]\n").c_str());

    std::istringstream iss(hwidListRaw);
    std::string line;
    while (std::getline(iss, line)) {

        line.erase(std::remove_if(line.begin(), line.end(), [](unsigned char c) {
            return std::isspace(c);
            }), line.end());

        OutputDebugStringA(("Checking line: [" + line + "]\n").c_str());

        if (currentHWID == line) {
            OutputDebugStringA("HWID matched!\n");
            return true;
        }
    }

    MessageBoxA(NULL, "Unauthorized HWID! Please buy dxqni premium.", "Access Denied", MB_ICONERROR);
    OutputDebugStringA("HWID not found in list.\n");
    TerminateProcess();
    return false;
}

void AddFont(const std::string& fontPath) {
    std::wstring temp = std::wstring(fontPath.begin(), fontPath.end());
    AddFontResource(temp.c_str());
}

void Client::DisplayClientMessage(const char* fmt, ...) {
    LocalPlayer* localPlayer = Game.getLocalPlayer();
    if (localPlayer == nullptr)
        return;

    va_list arg;
    va_start(arg, fmt);
    char message[300];
    vsprintf_s(message, 300, fmt, arg);
    va_end(arg);

    static std::string headerMessage;
    if (headerMessage.empty()) {
        char headerCStr[50];
        sprintf_s(headerCStr, 50, "%s[%sBoost%s] %s", MCTF::GRAY, MCTF::BLUE, MCTF::GRAY, MCTF::WHITE);
        headerMessage = std::string(headerCStr);
    }

    std::string messageStr = headerMessage + std::string(message);
    localPlayer->displayClientMessage(messageStr);
}

#include <winrt/Windows.UI.Notifications.h>
#include <winrt/Windows.Data.Xml.Dom.h>
#include <winrt/Windows.Foundation.h>
#include <winrt/Windows.UI.Notifications.Management.h>

using namespace winrt::Windows::UI::Notifications;

void ShowNotification(const std::wstring& title, const std::wstring& message, int duration = 5) {
    winrt::Windows::Data::Xml::Dom::XmlDocument toastXml;
    toastXml.LoadXml(L"<toast duration='short'><visual><binding template='ToastGeneric'><text>" + title + L"</text><text>" + message + L"</text></binding></visual></toast>");

    ToastNotification toast(toastXml);
    toast.ExpirationTime(winrt::Windows::Foundation::DateTime::clock::now() + std::chrono::seconds(duration));
    auto notifier = ToastNotificationManager::CreateToastNotifier();
    notifier.Show(toast);
}

void Client::init() {
    if (!VerifyHWID()) return;

    std::string ClientPath = FileUtil::getClientPath();
    if (!FileUtil::doesFilePathExist(ClientPath)) {
        FileUtil::createPath(ClientPath);
    }

    Logger::init();
    ConfigManager::init();

    Addresses::init();
    MCTF::initMCTF();

    ModuleManager::init();
    CommandManager::init();
    HookManager::init();

    if (Game.clientInstance == nullptr)
        return;

    initialized = true;

    Beep(50, 255);
    NotificationManager::addNotification("Successfully injected Boost!", 5.f);
    ShowNotification(L"BoostV2 Client", L"Injection successful!", 5);

    if (!ConfigManager::doesConfigExist("default"))
        ConfigManager::createNewConfig("default");

    ConfigManager::loadConfig("default");
}

void Client::shutdown() {
    ConfigManager::saveConfig();
    initialized = false;
}
