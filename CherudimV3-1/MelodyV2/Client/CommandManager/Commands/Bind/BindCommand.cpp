#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"
#include "BindCommand.h"
#include "../../CommandManager.h"
#include "../../../Client.h"

Bind::Bind() : Command("bind", "Bind Mobules", { "b", "bi", "bin" }, "<u got some stupititys>") {}
void Bind::execute(const std::vector<std::string>& args) {
    if (args.size() < 3) {
        const char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .(b,bi,bind) <module> <key>";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }

    std::string moduleName = args[1];
    std::string key = args[2];
    std::transform(moduleName.begin(), moduleName.end(), moduleName.begin(), ::tolower);

    auto module = client->moduleMgr->getModuleByName(moduleName);
    if (!module.has_value()) {
        mc.DisplayClientMessage("%sModule '%s' not found!", RED, moduleName);
        return;
    }

    if (key == "none") {
        (*module)->setKeybind(0x0);
        mc.DisplayClientMessage("%sSuccessfully unbound %s!", GREEN, moduleName);
        return;
    }

    // Convert the key to lowercase for case-insensitive comparison
    std::transform(key.begin(), key.end(), key.begin(), ::tolower);

    for (int i = 0; i < 190; i++) {
        const char* keyName = KeyNames[i];
        std::string keyNameLower = keyName;
        std::transform(keyNameLower.begin(), keyNameLower.end(), keyNameLower.begin(), ::tolower);

        if (key == keyNameLower) {
            (*module)->setKeybind(i);
            mc.DisplayClientMessage("%sThe keybind of %s is now '%s'", GREEN, moduleName, keyName);
            return;
        }
    }

    mc.DisplayClientMessage("%sInvalid key!", RED);
}
