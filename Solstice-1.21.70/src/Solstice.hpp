#pragma once
#include <Windows.h>
#include <Features/Configs/PreferenceManager.hpp>
#include "spdlog/logger.h"
//
// Created by vastrakai on 6/24/2024.
//


class Solstice {
public:
    /* Fields */
    static inline HMODULE mModule;
    static inline bool mInitialized = false;
    static inline bool mRequestEject = false;
    static inline int64_t mLastTick = 0;
    static inline std::shared_ptr<spdlog::logger> console;
    static inline std::shared_ptr<Preferences> Prefs;
    static inline std::string sHWID;
    static inline std::thread mThread;


    /* Functions */
    static void init(HMODULE hModule);
    static void shutdownThread();
};