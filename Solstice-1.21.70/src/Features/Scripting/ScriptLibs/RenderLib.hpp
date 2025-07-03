#pragma once
//
// Created by vastrakai on 9/24/2024.
//
#include "ScriptLib.hpp"

class RenderLib : public ScriptLib {
public:
    void initialize(lua_State* L) override;
};