#pragma once
//
// Created by vastrakai on 9/23/2024.
//
#include <LuaBridge/LuaBridge.h>

using namespace luabridge;

// Base class for all script libraries
class ScriptLib
{
public:
    ScriptLib() = default;

    virtual ~ScriptLib() = default;
    virtual void initialize(lua_State* L){
        // Register functions here
        spdlog::error("[lua] Cannot execute base ScriptLib::initialize. This function should be overridden by a derived class.");
    }

    static void generateDocs(lua_State* state);

    template <typename T>
    static void registerLib(lua_State* L) {
        T lib;
        lib.initialize(L);
        spdlog::info("[lua] registered script library: {}", typeid(T).name());
    }

    void registerFunction(lua_State* state, lua_CFunction func, const char* functionName, const char* tableName);
    void registerFunction(lua_State* state, lua_CFunction func, const char* functionName);


    static void lPushField(lua_State* L, const char* key, const char* value)
    {
        lua_pushstring(L, key);
        lua_pushstring(L, value);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, int value)
    {
        lua_pushstring(L, key);
        lua_pushinteger(L, value);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, double value)
    {
        lua_pushstring(L, key);
        lua_pushnumber(L, value);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, bool value)
    {
        lua_pushstring(L, key);
        lua_pushboolean(L, value);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, void* value)
    {
        lua_pushstring(L, key);
        lua_pushlightuserdata(L, value);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, ImVec2& value)
    {
        lua_pushstring(L, key);
        lua_newtable(L);
        lPushField(L, "x", value.x);
        lPushField(L, "y", value.y);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, glm::vec2& value)
    {
        lua_pushstring(L, key);
        lua_newtable(L);
        lPushField(L, "x", value.x);
        lPushField(L, "y", value.y);
        lua_settable(L, -3);
    }

    static void lPushField(lua_State* L, const char* key, glm::vec3& value)
    {
        lua_pushstring(L, key);
        lua_newtable(L);
        lPushField(L, "x", value.x);
        lPushField(L, "y", value.y);
        lPushField(L, "z", value.z);
        lua_settable(L, -3);
    }

    static void lPushFunction(lua_State* L, const char* name, lua_CFunction func)
    {
        lua_pushstring(L, name);
        lua_pushcfunction(L, func, name);
        lua_settable(L, -3);
    }

    static ImVec2 lGetImVec2(lua_State* L, int index)
    {
        ImVec2 vec;
        lua_getfield(L, index, "x");
        vec.x = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "y");
        vec.y = (float)lua_tonumber(L, -1);
        lua_pop(L, 2);
        return vec;
    }

    static ImVec4 lGetImVec4(lua_State* L, int index)
    {
        ImVec4 vec;
        lua_getfield(L, index, "x");
        vec.x = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "y");
        vec.y = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "z");
        vec.z = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "w");
        vec.w = (float)lua_tonumber(L, -1);
        lua_pop(L, 4);
        return vec;
    }

    static ImColor lGetImColor(lua_State* L, int index)
    {
        ImColor color;
        lua_getfield(L, index, "r");
        int r = (int)lua_tointeger(L, -1);
        lua_getfield(L, index, "g");
        int g = (int)lua_tointeger(L, -1);
        lua_getfield(L, index, "b");
        int b = (int)lua_tointeger(L, -1);
        lua_getfield(L, index, "a");
        int a = (int)lua_tointeger(L, -1);
        color = IM_COL32(r, g, b, a);
        lua_pop(L, 4);
        return color;
    }

    static glm::vec3 lGetVec3(lua_State* L, int index)
    {
        glm::vec3 vec;
        lua_getfield(L, index, "x");
        vec.x = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "y");
        vec.y = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "z");
        vec.z = (float)lua_tonumber(L, -1);
        lua_pop(L, 3);
        return vec;
    }

    static glm::vec2 lGetVec2(lua_State* L, int index)
    {
        glm::vec2 vec;
        lua_getfield(L, index, "x");
        vec.x = (float)lua_tonumber(L, -1);
        lua_getfield(L, index, "y");
        vec.y = (float)lua_tonumber(L, -1);
        lua_pop(L, 2);
        return vec;
    }
};
