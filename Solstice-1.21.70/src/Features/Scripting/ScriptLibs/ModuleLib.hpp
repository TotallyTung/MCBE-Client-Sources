#pragma once
//
// Created by vastrakai on 9/28/2024.
//
#include "ScriptLib.hpp"

class ModuleLib : public ScriptLib {
public:
    void initialize(lua_State* L) override;
};