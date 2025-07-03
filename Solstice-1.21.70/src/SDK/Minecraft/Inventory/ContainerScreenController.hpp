#pragma once
//
// Created by vastrakai on 7/5/2024.
//

#include <string>

class ContainerScreenController {
public:
    void handleAutoPlace(const std::string& name, int slot);
    void* _tryExit();
};