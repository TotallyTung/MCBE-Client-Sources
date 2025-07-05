#pragma once


#include "Hook.h"
#include <vector>

class HookManager {
public:
    static std::vector<Hook*> Hooks;

    static void Init();
    static Hook* GetHookByName(std::string name);
    static void Shutdown();
};