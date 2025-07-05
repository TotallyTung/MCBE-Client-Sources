//
// Created by Flash on 9/9/2023.
//

#include "Help.h"


void Help::OnRun(std::vector<std::string> args) {

    for (auto command : CommandManager::Commands) {
        AddCommandMessage("§f§l- §r" + command->Description, "§r§l§6" + command->Trigger + "§f§l");
    }
}