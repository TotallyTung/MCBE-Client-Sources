// Created by Tony on 2024-10-10 10:01:54

#include "CommandManager.h"
#include "../Client.h"

CommandManager::CommandManager() {
    commandList.push_back(new Test());
    commandList.push_back(new ItemId());
    commandList.push_back(new Baritone());
    commandList.push_back(new Save());
    commandList.push_back(new IRCOMMAND());
    commandList.push_back(new Bind());
    commandList.push_back(new Friend());
    commandList.push_back(new PlayerList());
}

CommandManager::~CommandManager() {
    for (Command* com : commandList) {
        delete com;
    }
}

void CommandManager::onSendTextPacket(TextPacket* packet, bool& shouldCancel) {
    std::string message = packet->message;
    if (message[0] != prefix) return;
    shouldCancel = true;
    std::vector<std::string> args;
    std::string mmb;
    for (int i = 1; i < message.size(); i++) {
        if (message[i] == ' ') {
            args.push_back(mmb);
            mmb.clear();
            continue;
        }
        mmb += message[i];
        if (i == message.size() - 1) {
            args.push_back(mmb);
            mmb.clear();
            break;
        }
    }
    if (args.size() == 0) {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command!", DARK_PURPLE, WHITE, RED);
        return;
    }
    Command* current = nullptr;
    for (Command* command : commandList) {
        bool shouldBreak = false;
        for (const std::string& alias : command->aliases) {
            if (alias == args[0]) {
                current = command;
                shouldBreak = true;
                break;
            }
        }
        if (shouldBreak) break;
    }
    if (current == nullptr) {
        mc.DisplayClientMessage("[%sMelody%s] %sInvalid command!", DARK_PURPLE, WHITE, RED);
        return;
    }
    current->execute(args);
}
