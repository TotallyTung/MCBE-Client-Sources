#pragma once
//
// Created by vastrakai on 9/26/2024.
//

#include "ScriptLib.hpp"

class MathLib : public ScriptLib {
public:
    void initialize(lua_State* L) override;
};