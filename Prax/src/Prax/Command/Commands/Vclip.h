#pragma once
//
// Created by vastrakai on 9/7/2023.
//


#include <Prax/Command/Command.h>

class Vclip : public Command {
public:
    Vclip() : Command("VClip", "Teleports you vertically") {
        AddArgument("amount", "Amount to teleport");
    }

    void OnRun(std::vector<std::string> args) override;

    [[nodiscard]] static bool IsNumber(std::string &str);
};