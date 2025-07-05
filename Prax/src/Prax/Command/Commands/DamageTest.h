//
// Created by vastrakai on 12/31/2023.
//


#pragma once
#include <Prax/Command/Command.h>

// Ment to be used for testing damage fly
class DamageTest : public Command {
public:
    DamageTest() : Command("DamageTest", "Test damage fly", "", { "dt" }) {
    }

    void OnRun(std::vector<std::string> args) override;
};