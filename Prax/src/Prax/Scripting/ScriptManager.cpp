//
// Created by Flash on 2/3/2024.
//

#include "ScriptManager.h"
#include <Prax/UI/ScriptGUI.h>
#include "Scripting.h"

/*#include <lua.h>
#include "lualib.h"
#include <luacode.h>
#include "Luau/Compiler.h"
#include "Includes/Luau/VM/src/lvm.h"
*/

/*
lua_State* ScriptManager::L = nullptr;

void ScriptManager::RegisterGlobalFunc(const char* name, lua_CFunction func)
{
    lua_pushcfunction(L, func, NULL);
    lua_setglobal(L, name);
}

void ScriptManager::InitFunctions()
{
    RegisterGlobalFunc("print", Scripting::print);

}


void ScriptManager::Init()
{
    try
    {

        L = luaL_newstate();

        luaL_openlibs(L);

        Logger::Write("ScriptManager", "Initialized Lua state", Logger::Info);

        Execute("print('Hello from Luau!')");

        lua_close(L);
    }
    catch (const std::exception& e)
    {
        Logger::Write("ScriptManager", e.what(), Logger::Error);
    }
    catch (...)
    {
        Logger::Write("ScriptManager", "Unknown error", Logger::Error);
    }

}

void ScriptManager::LoadScripts() {

}

void ScriptManager::Execute(const std::string& code)
{

    size_t bytecodeSize = 0;
    char* bytecode = luau_compile(code.c_str(), strlen(code.c_str()), NULL, &bytecodeSize);
    int result = luau_load(L, "SCRIPTMANAGER", bytecode, bytecodeSize, 0);
    free(bytecode);

    if (result == LUA_OK)
    {
        Logger::Write("ScriptManager", "Executed code", Logger::Info);
    }
    else
    {
        const char* err = lua_tostring(L, -1);
        Logger::Write("LUA", err);
        lua_pop(L, 1);
        Logger::Write("ScriptManager", "Failed to execute code " + std::to_string(result), Logger::Error);
    }

    int r = lua_pcall(L, 0, 1, 0);
    if (r != 0) {
        const char* err = lua_tostring(L, -1);
        printf("[LUA] %s\n", err);
        lua_pop(L, 1);
    }

}*/
