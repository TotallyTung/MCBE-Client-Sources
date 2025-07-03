#pragma once
#include <variant>
#include <Features/FeatureManager.hpp>

#include "luacode.h"
#include "lualib.h"
#include "spdlog/spdlog.h"
#include <Utils/StringUtils.hpp>
//
// Created by vastrakai on 9/22/2024.
//

enum class SolsticeScriptType {
    MODULE,
    COMMAND
};

class SolsticeScript {
public:
    std::string name;          // Name of the script
    std::string code;          // Script code as a string
    lua_State* luaState;       // Lua state specific to the script instance
    bool isValid = false;      // Flag to check if the script was successfully compiled
    SolsticeScriptType type;   // Type of the script
    lua_CompileOptions options; // Compilation options
    static inline std::map<lua_State*, SolsticeScript*> scriptStorage ; // Map of lua state to script instance

    SolsticeScript(const std::string& scriptName, const std::string& scriptCode);

    virtual ~SolsticeScript() {
        scriptStorage.erase(luaState);
    }

    bool compile();

    virtual void execute()
    {
        spdlog::error("Cannot execute base SolsticeScript::execute for script: {}", name);
    }

    void execBase() {
        if (isValid) {
            lua_pcall(luaState, 0, 0, 0);
        }
    }

    virtual void onCompile()
    {
        spdlog::error("Cannot execute base SolsticeScript::onCompile for script: {}", name);
    }

    lua_State* getLuaState() {
        return luaState;
    }

    SolsticeScript() = default;

};

using LuaValue = std::variant<std::string, double, bool, int, void*>;
using EventData = std::map<std::string, LuaValue>;


class ModuleScript : public SolsticeScript {
public:
    ModuleScript(const std::string& scriptName, const std::string& scriptCode)
        : SolsticeScript(scriptName, scriptCode)
    {
        type = SolsticeScriptType::MODULE;
    }

    std::string moduleName; // Name of the module
    std::string moduleDescription; // Description of the module
    std::string moduleAuthor; // Author of the module

    std::mutex eventMutex; // Mutex to protect the event queue

    bool enabled = false; // Flag to check if the module is enabled
    bool ignoreEnabled = false; // Flag to ignore the enabled state of the module. If this is true, the scripts will need to handle the enabled state themselves.
    int keybind = 0; // Keybind for the module

    void onCompile() override;
    void execute() override;
    void execEvent(const std::string& eventName);
    void execEvent(const std::string& eventName, EventData* eventData);

    void registerModuleFunction(const std::string& functionName, lua_CFunction function) {
        lua_pushcfunction(luaState, function, functionName.c_str());
        lua_setglobal(luaState, functionName.c_str());
    }

    void triggerStateChangeEvent();
};

class CommandScript : public SolsticeScript {
public:
    std::string commandName;
    std::vector<std::string> commandAliases;
    std::string commandDescription;

    CommandScript(const std::string& scriptName, const std::string& scriptCode);

    void onCompile() override;
    void execute() override;
    void execCommand(std::vector<std::string>& args);
    bool isMatching(const std::string& command);
    void registerCommand(const std::string& commandName, lua_CFunction commandFunc);
};