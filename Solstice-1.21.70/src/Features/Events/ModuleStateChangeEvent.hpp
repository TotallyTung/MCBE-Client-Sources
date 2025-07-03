//
// Created by vastrakai on 6/29/2024.
//

#pragma once

#include "Event.hpp"

class Module;

class ModuleStateChangeEvent : public CancelableEvent {
public:
    Module* mModule;
    bool mEnabled;
    bool mWasEnabled;

    explicit ModuleStateChangeEvent(Module* module, bool enabled, bool wasEnabled) : CancelableEvent() {
        mModule = module;
        mEnabled = enabled;
        mWasEnabled = wasEnabled;
    }
};

class ModuleScript;

class ModuleScriptStateChangeEvent : public CancelableEvent {
public:
    ModuleScript* mModule;
    bool mEnabled;
    bool mWasEnabled;

    explicit ModuleScriptStateChangeEvent(ModuleScript* module, bool enabled, bool wasEnabled) : CancelableEvent() {
        mModule = module;
        mEnabled = enabled;
        mWasEnabled = wasEnabled;
    }
};