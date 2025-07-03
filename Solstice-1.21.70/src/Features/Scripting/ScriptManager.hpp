#pragma once

#include <unordered_set>
#include <unordered_map>
#define _AMD64_
#include <minwindef.h>

#include "lua.h"
#include "SolsticeScript.hpp"
#include <Features/Events/ClientTickEvent.hpp>

//
// Created by vastrakai on 9/22/2024.
//


class ScriptManager {
public:
    lua_State* mGlobalLuaState = nullptr;
    std::atomic<bool> mRunning = false;
    std::string mScriptDir;
    std::unordered_map<std::string, FILETIME> mFileTimes;
    std::unordered_set<std::string> mKnownScripts;
    std::vector<std::shared_ptr<SolsticeScript>> mScripts;
    std::atomic<bool> mLoadingScripts = true;

    static void registerFunction(lua_State* state, lua_CFunction func, const char* functionName, const char* tableName);
    static void registerFunction(lua_State* state, lua_CFunction func, const char* functionName);
    void registerFunctions(lua_State* L);
    void registerEvents(lua_State* L);
    void init();
    void execTests();
    void shutdown();
    bool findAndExecCommand(const std::string& string, const std::vector<std::string>& args);
    std::vector<std::shared_ptr<SolsticeScript>>& getScripts();
    static bool isCompilable(const std::string& code);
    void execute(const std::string& code);

    void loadUserScripts();
    void unloadUserScripts();
    void dispatchEvent(const std::string& eventName);
    void dispatchEvent(const std::string& eventName, EventData* eventData);
    void onClientTick(ClientTickEvent& event);
    void startFileWatcher();
};