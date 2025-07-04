#include "Client.h"

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
    }

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