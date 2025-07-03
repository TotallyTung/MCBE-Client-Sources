//
// Created by vastrakai on 6/24/2024.
//

#include "Detour.hpp"

#include <magic_enum.hpp>
#include <MinHook.h>
#include <string>

#include "spdlog/spdlog.h"

Detour::~Detour()
{
    if (mFunc)
    {
        MH_DisableHook(mFunc);
        MH_RemoveHook(mFunc);
    }
}

Detour::Detour(const std::string& name, void* addr, void* detour, bool silent)
    : mName(name), mFunc(addr)
{
    this->mFunc = addr;
    this->mName = name;

    const MH_STATUS result = MH_CreateHook(mFunc, detour, &mOriginalFunc);

    if (!silent)
    {
        if (result == MH_STATUS::MH_OK) {
            spdlog::info("Created detour for {} at {}", name, addr);
        } else {
            spdlog::critical("Failed to create detour for {} at {}, error: {}", name, addr, magic_enum::enum_name(result));
        }
    }
}

void Detour::enable(bool silent) const
{
    const MH_STATUS status = MH_EnableHook(mFunc);

    if (silent) return;
    switch (status)
    {
    case MH_OK:
        spdlog::info("Enabled detour for {}", mName);
        break;
    default:
        spdlog::critical("Failed to enable detour for {}", mName);
        break;
    }
}

void Detour::restore() const
{
    if (!this)
    {
        spdlog::critical("Failed to restore detour [this == nullptr]");
        return;
    }
    if (!mFunc)
    {
        spdlog::critical("Failed to restore detour for {} [mFunc == nullptr]", mName);
        return;
    }
    MH_STATUS status = MH_DisableHook(mFunc);
    switch (status)
    {
    case MH_OK:
        spdlog::info("Restored detour for {}", mName);
        break;
    default:
        spdlog::critical("Failed to restore detour for {} [MH_DisableHook returned {}]", mName, magic_enum::enum_name(status));
        break;
    }
}

