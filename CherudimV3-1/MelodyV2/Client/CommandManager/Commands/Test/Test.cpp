#pragma once
#include <vector>
#include <string>
#include <algorithm>
#include "../Command.h"

#include "../../CommandManager.h"
#include "../../../Client.h"
Test::Test() : Command("test", "Texts", { "t1", "t2", "t3" }, "<u got some stupititys>") {}

void Test::execute(const std::vector<std::string>& args) {
    if (args.size() < 2) {
        char errorMsg[] = "[%sMelody%s] %sInvalid command! Usage: .test <argument>";
        mc.DisplayClientMessage(errorMsg, DARK_PURPLE, WHITE, RED);
        return;
    }
    char* output = const_cast<char*>(args[1].c_str());
    mc.DisplayClientMessage(output);
}