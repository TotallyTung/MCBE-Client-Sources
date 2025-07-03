#pragma once
#include <memory>
#include "Detour.hpp"
#include "spdlog/spdlog.h"
//
// Created by vastrakai on 6/25/2024.
//


class Hook {
public:
    std::string mName;
    Hook() = default;
    bool mLocalPlayerDependent = false; // if this is true, the init function will not be called until the local player is valid

    virtual void init();
    virtual void shutdown() { spdlog::error("Hook::shutdown() not implemented for hook: {}", mName); };
};

// TODO: Hookset class that manages multiple hooks