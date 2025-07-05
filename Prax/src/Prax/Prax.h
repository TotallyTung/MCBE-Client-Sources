//
// Created by Flash on 6/10/2023.
//

#pragma once
#include "PrefManager.h"

#include <MinHook.h>


class Prax {
public:
    static HMODULE ModuleHandle;
    static bool Initialized;
    static bool ShouldUIRender;
    static Preferences* Preferences;
    static uint64_t InjectTime;

    static void Init();
    static void Shutdown();

    static std::string ClientName;
};

