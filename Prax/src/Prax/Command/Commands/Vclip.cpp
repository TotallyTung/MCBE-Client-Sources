//
// Created by vastrakai on 9/7/2023.
//

#include "Vclip.h"

void Vclip::OnRun(std::vector<std::string> args) {
    // Check the arg count
    if (args.size() != 1) {
        AddCommandMessage("Usage: .vclip <amount>");
        return;
    }

    // Check if the arg is a number
    if (!IsNumber(args[0])) {
        AddCommandMessage("Amount must be a number");
        return;
    }

    auto player = Minecraft::ClientInstance->ThePlayer();
    auto pos = *player->getPos();
    auto amount = std::stof(args[0]);

    player->setPosition({pos.x, pos.y + amount, pos.z});

    bool negative = amount < 0;
    std::string direction = negative ? "down" : "up";
    int absValue = abs(amount);
    AddCommandMessage("Teleported " + direction + " " + std::to_string(absValue) + " blocks");
}

[[nodiscard]] bool Vclip::IsNumber(std::string &str) {
    // Parse the number as a float
    try {
        return std::stof(str);
    } catch (std::invalid_argument &e) {
        return false;
    }
}
