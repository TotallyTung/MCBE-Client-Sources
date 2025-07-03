//
// Created by vastrakai on 9/23/2024.
//

#include "KeyboardLib.hpp"

void KeyboardLib::initialize(lua_State* L)
{
    // bool isDown = Keyboard::mPressedKeys[key];
    /*registerFunction(L, [](lua_State* L) -> int
    {
        std::string key = std::string(luaL_checkstring(L, 1));
        key = StringUtils::toLower(key);
        int keyId = Keyboard::mKeyMap[std::string(key)];
        if (keyId == 0) {
            luaL_error(L, "Invalid key name");
        }
        bool isDown = Keyboard::mPressedKeys[keyId];
        lua_pushboolean(L, isDown);

        return LUA_YIELD;
    }, "isKeyDown", "Keyboard");*/
    getGlobalNamespace(L)
        .beginNamespace("Keyboard")
            .addFunction("isKeyDown", [](std::string key) { return Keyboard::mPressedKeys[Keyboard::mKeyMap[key]]; })
            .addFunction("isUsingMoveKeys", []() { return Keyboard::isUsingMoveKeys(); })
        .endNamespace();
};