#pragma once
//
// Created by vastrakai on 10/8/2024.
//
#include "ScriptLib.hpp"

class InventoryLib : public ScriptLib {
public:
    void initialize(lua_State* L) override;
};