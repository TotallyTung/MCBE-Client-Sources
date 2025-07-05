#pragma once

#include "Hook.h"


Hook::Hook(std::string name, bool initOnConstruct) {
    this->Name = std::move(name);
    InitOnConstruct = initOnConstruct;

    Logger::Write("Hook", "Created hook " + this->Name, Logger::LogType::Debug);
}

void Hook::Init() {
    Logger::Write("Hook", "Init was called on " + Name + " but no override was found", Logger::LogType::Warning);
}

void Hook::UnHook() {
    Logger::Write("Hook", "UnHook was called on " + Name + " but no override was found", Logger::LogType::Debug);

    for (Detour* detour : Detours) {
        Logger::Write("Hook", "Detour: " + detour->Name + " Restored", Logger::Debug);
        detour->Restore();
    }
}