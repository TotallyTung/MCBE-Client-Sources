#pragma once

#include "Detour.h"
#include <MinHook.h>
#include <Prax/Utils/Misc.h>

Detour::~Detour() {
    Restore();
}

Logger::LogType LogTypeFromMH_STATUS(MH_STATUS status)
{
    switch (status)
    {
    case MH_OK:
        return Logger::Info;
        break;
    default:
        return Logger::Error;
        break;
    }
}

Detour::Detour(const std::string& name, void* addr, void* detour)
{
    this->FuncPtr = addr;
    this->Name = name;

    const MH_STATUS result = MH_CreateHook(FuncPtr, detour, &OFunc);

    Logger::Write("Detour", Logger::Debug, ("Created detour for " + name + " at: " + Misc::GetModuleOfAddress(reinterpret_cast<uintptr_t>(addr)) + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
}

Detour::Detour(const std::string& name, void* addr, void* detour, bool silent)
{
    this->FuncPtr = addr;
    this->Name = name;

    const MH_STATUS result = MH_CreateHook(FuncPtr, detour, &OFunc);

    if (!silent)
    {
        Logger::Write("Detour", Logger::Debug, ("Created detour for " + name + " at: " + Misc::GetModuleOfAddress(reinterpret_cast<uintptr_t>(addr)) + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
    }
}

Detour::Detour(std::string name, uintptr_t addr, void* detour)
{
    this->FuncPtr = reinterpret_cast<void*>(addr);
    this->Name = name;

    const MH_STATUS result = MH_CreateHook(FuncPtr, detour, &OFunc);

    Logger::Write("Detour", Logger::Debug, ("Created detour for " + name + " at: " + Misc::GetModuleOfAddress(addr) + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
}

Detour::Detour(std::string name, uintptr_t** addr, void* detour)
{
    this->FuncPtr = reinterpret_cast<void*>(addr);
    this->Name = name;


    const MH_STATUS result = MH_CreateHook(FuncPtr, detour, &OFunc);

    Logger::Write("Detour", Logger::Debug, ("Created detour for " + name + " at: " + Misc::GetModuleOfAddress(reinterpret_cast<uintptr_t>(addr)) + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
}


void Detour::Enable(bool silent) const
{

    // Make sure funcptr is not null
    if (FuncPtr == nullptr)
    {
        Logger::Write("Detour", "Failed to enable detour for " + Name + " because FuncPtr is null", Logger::LogType::Error);
        return;
    }

    const MH_STATUS result = MH_EnableHook(FuncPtr);



    if (!silent) Logger::Write("Detour", Logger::Debug, ("Attempted enable for " + Name + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));

}

void Detour::Restore() const
{
    if (FuncPtr != nullptr)
    {
        MH_STATUS result = MH_DisableHook(FuncPtr);
        Logger::Write("Detour", Logger::Debug, ("Disabled detour for " + Name + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
        result = MH_RemoveHook(FuncPtr);
        Logger::Write("Detour", Logger::Debug, ("Restored detour for " + Name + ", Status: " + MH_StatusToString(result)).c_str(), LogTypeFromMH_STATUS(result));
    }
}
