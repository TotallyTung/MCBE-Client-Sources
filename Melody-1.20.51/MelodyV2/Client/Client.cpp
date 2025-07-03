// Created by Tony on 2024-10-10 10:01:54

#include "Client.h"
#include "../Libs/xor.hpp"
#include "../Utils/Internet.h"
std::string Client::getClientName() {
    return clientName;
}

void Client::setClientName(const std::string& newName) {
    clientName = newName;
}

std::string Client::getClientVersion() {
    return clientVersion;
}

void Client::init() {
    AllocConsole();
    AttachConsole(GetCurrentProcessId());

    FILE* f;

    SetConsoleTitleA("Melo Console");

    freopen_s(&f, "CONIN$", "r", stdin);
    freopen_s(&f, "CONOUT$", "w", stderr);
    freopen_s(&f, "CONOUT$", "w", stdout);

    printf("Test Message *******************");

    if (MemoryUtils::getBase() == 0) return;
    long start = clock();

    { // initClientFiles

        std::string logsPath = Utils::getClientPath() + "Logs.txt";
        auto f = std::filesystem::path(logsPath);
        if (std::filesystem::exists(f)) std::filesystem::remove(f);

        std::string clientPath = Utils::getClientPath();
        if (!Utils::doesClientPathExist(clientPath)) {
            Utils::createPath(clientPath);
        }
        std::string configsPath = clientPath + "Configs\\";
        if (!Utils::doesClientPathExist(configsPath)) {
            Utils::createPath(configsPath);
        }
        std::string MusicPath = clientPath + "Music\\";
        if (!Utils::doesClientPathExist(MusicPath)) {
            Utils::createPath(MusicPath);
        }
        std::string AssetPath = clientPath + "Assets\\";
        if (!Utils::doesClientPathExist(AssetPath)) {
            Utils::createPath(AssetPath);
        }
        std::string CustomMusicPath = clientPath + "CustomMusic\\";
        if (!Utils::doesClientPathExist(CustomMusicPath)) {
            Utils::createPath(CustomMusicPath);
        }
    }

    //https://media.discordapp.net/attachments/1131979935691579485/1266325235368460359/IMG_1614.jpg?ex=67030007&is=6701ae87&hm=13b5a48d8a7934b23603c01b2389cb5e8e2016e2b634dc7ba3ab6ef119850a27&=&format=webp&width=720&height=717
    Network::downloadFile(xorstr_("inject.wav"), xorstr_("https://cdn.discordapp.com/attachments/1131979935691579485/1292179207945715823/mixkit-retro-game-notification-212.wav?ex=6702cb2a&is=670179aa&hm=6fcd5fc15f0adf1dcbbb9519a0006fd068cdcd5683647616dfc65effdec965f5&"), xorstr_("Music"));
    Network::downloadFile(xorstr_("mainMenuBackground.jpg"), xorstr_("https://media.discordapp.net/attachments/1131979935691579485/1266325235368460359/IMG_1614.jpg?ex=67030007&is=6701ae87&hm=13b5a48d8a7934b23603c01b2389cb5e8e2016e2b634dc7ba3ab6ef119850a27&=&format=webp&width=720&height=717"), xorstr_("Assets"));
    logF("Injected at %s", __TIMESTAMP__);
    initMCTextFormat();
    configMgr = new ConfigManager();
    moduleMgr = new ModuleManager();
    commandMgr = new CommandManager();
    configMgr->loadConfig("default");
    HookManager::init();

    this->initialized = true;
    Notifications::addNotifBox("Successfully injected Melody", 5.f);
    logF("Init times: %.2fs", (float)(clock() - start) / 1000.f);
}

Client::~Client() {
    initialized = false;
    HookManager::Restore();
    //configMgr->saveConfig();
    delete commandMgr;
    delete moduleMgr;
    delete configMgr;
}

Client* client = new Client();