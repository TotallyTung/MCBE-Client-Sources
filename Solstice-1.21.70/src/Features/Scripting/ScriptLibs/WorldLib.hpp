#pragma once
//
// Created by vastrakai on 10/2/2024.
//
#include "ScriptLib.hpp"

class WorldLib : public ScriptLib {
public:
    void initialize(lua_State* L) override;
};