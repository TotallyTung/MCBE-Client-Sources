//
// Created by Flash on 7/30/2023.
//

#pragma once


#include <vector>
#include "Command.h"

class CommandManager {
public:

    static std::vector<Command*> Commands;


    static void Init();
    static void Shutdown();
    static void HandleInput(const std::string& originalInput, bool* cancelPacket);
};