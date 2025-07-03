//
// Created by vastrakai on 9/23/2024.
//

#include "FSLib.hpp"

void FSLib::initialize(lua_State* state)
{
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
            auto path = luaL_checkstring(L, 1);
            // expand any %VAR% in the path
            char expanded[MAX_PATH];
            ExpandEnvironmentStringsA(path, expanded, MAX_PATH);
            lua_pushstring(L, expanded);
            return LUA_YIELD; // Return true if successful
        }, "expandEnv", "fs");

    // get solstice dir
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        lua_pushstring(L, FileUtils::getSolsticeDir().c_str());
        return LUA_YIELD; // Return true if successful
    }, "getSolsticeDir", "fs");

    // Check if a path exists
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        lua_pushboolean(L, std::filesystem::exists(path));
        return LUA_YIELD; // Return true if successful
    }, "exists", "fs");

    // Check if a path is a directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        lua_pushboolean(L, std::filesystem::is_directory(path));
        return LUA_YIELD; // Return true if successful
    }, "isDirectory", "fs");

    // Check if a path is a file
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        lua_pushboolean(L, std::filesystem::is_regular_file(path));
        return LUA_YIELD; // Return true if successful
    }, "isFile", "fs");

    // Create a directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        try {
            bool result = std::filesystem::create_directory(path);
            lua_pushboolean(L, result);
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "createDirectory", "fs");

    // Remove a file or directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        try {
            bool result = std::filesystem::remove(path);
            lua_pushboolean(L, result);
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "remove", "fs");

    // Rename a file or directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto oldPath = luaL_checkstring(L, 1);
        auto newPath = luaL_checkstring(L, 2);
        try {
            std::filesystem::rename(oldPath, newPath);
            lua_pushboolean(L, true);
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "rename", "fs");

    // List files in a directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        lua_newtable(L); // Create a new Lua table to hold file names
        try {
            int index = 1;
            for (const auto& entry : std::filesystem::directory_iterator(path)) {
                lua_pushstring(L, entry.path().filename().string().c_str());
                lua_rawseti(L, -2, index++);
            }
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "listFiles", "fs");

    // Get the current working directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        try {
            std::string cwd = std::filesystem::current_path().string();
            lua_pushstring(L, cwd.c_str());
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "getCurrentDirectory", "fs");

    // Set the current working directory
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        try {
            std::filesystem::current_path(path);
            lua_pushboolean(L, true);
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "setCurrentDirectory", "fs");

    // Get the size of a file
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        try {
            lua_pushinteger(L, std::filesystem::file_size(path));
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "fileSize", "fs");

    // Get the last write time of a file
    ScriptManager::registerFunction(state, [](lua_State* L) -> int {
        auto path = luaL_checkstring(L, 1);
        try {
            auto time = std::filesystem::last_write_time(path);
            lua_pushinteger(L, time.time_since_epoch().count());
        } catch (const std::filesystem::filesystem_error& e) {
            luaL_error(L, e.what());
        }
        return LUA_YIELD; // Return true if successful
    }, "lastWriteTime", "fs");
}
