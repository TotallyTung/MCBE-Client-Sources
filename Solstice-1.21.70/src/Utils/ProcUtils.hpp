#pragma once
#include <Hook/Hooks/ActorHooks/BaseTickHook.hpp>
//
// Created by vastrakai on 6/24/2024.
//


class ProcUtils {
public:
    static int getModuleCount();
    static HWND getMinecraftWindow();
    static std::vector<std::wstring> getModulePaths();
    static std::string getVersion();
};