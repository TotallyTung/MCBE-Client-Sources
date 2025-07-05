#pragma once
//
// Created by vastrakai on 10/5/2023.
//


#include <Prax/Command/Command.h>

class Queue : public Command {
public:
    Queue() : Command("Queue", "Queues for a new game on The Hive", "", {"q"}) {
    }

    void OnRun(std::vector<std::string> args) override;
};