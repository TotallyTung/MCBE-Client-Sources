//
// Created by vastrakai on 9/22/2024.
//

#include "SolsticeScript.hpp"

SolsticeScript::SolsticeScript(const std::string& scriptName, const std::string& scriptCode): name(scriptName), code(scriptCode)
{
    luaState = luaL_newstate();
    luaL_openlibs(luaState);
    gFeatureManager->mScriptManager->registerFunctions(luaState);
    gFeatureManager->mScriptManager->registerEvents(luaState);

    scriptStorage[luaState] = this;

    spdlog::info("Created script: {}", name);
}

bool SolsticeScript::compile()
{
    try
    {
        // Ensure the code is compilable
        if (!ScriptManager::isCompilable(code)) {
            spdlog::error("Script code is not compilable, cannot compile.");
            isValid = false;
            return isValid;
        }

        // Ensure the script has content
        if (code.empty()) {
            spdlog::error("Script code is empty, cannot compile.");
            isValid = false;
            return isValid;
        }

        size_t bytecodeSize = 0;
        static auto* options = new lua_CompileOptions();
        options->debugLevel = 1;
        options->optimizationLevel = 1;
        options->typeInfoLevel = 0;
        options->coverageLevel = 0;

        // Compile the code into bytecode
        char* bytecode = luau_compile(code.c_str(), strlen(code.c_str()), options, &bytecodeSize);

        // Check if bytecode compilation was successful
        if (bytecode == nullptr) {
            // get error message
            const char* errorMsg = lua_tostring(luaState, -1);
            if (errorMsg) {
                spdlog::error("Failed to compile script: {}\nError: {}", name, errorMsg);
                ChatUtils::displayClientMessage("§cFailed to compile script: " + name + "\n§cError: " + errorMsg);
            } else {
                spdlog::error("Failed to compile script: {}", name);
                ChatUtils::displayClientMessage("§cFailed to compile script: " + name);
            }
            isValid = false;
            return isValid;
        }

        // Load the bytecode into the Lua state
        int loadResult = luau_load(luaState, name.c_str(), bytecode, bytecodeSize, 0);

        // Free the allocated memory for the bytecode
        free(bytecode);

        // Check if the bytecode was loaded successfully
        if (loadResult != LUA_OK) {
            // Get the error message
            const char* errorMsg = lua_tostring(luaState, -1);
            if (errorMsg) {
                spdlog::error("Failed to load compiled bytecode for script: {}\nError: {}", name, errorMsg);
                ChatUtils::displayClientMessage("§cFailed to load compiled bytecode for script: " + name + "\n§cError: " + errorMsg);
            } else
            {
                spdlog::error("Failed to load compiled bytecode for script: {}", name);
            }
            isValid = false;
            return isValid;
        }

        isValid = true;
        onCompile();
        return isValid;
    } catch (const std::exception& e) {
        spdlog::error("An error occurred while compiling script: {}\nError: {}", name, e.what());
        isValid = false;
        return isValid;
    } catch (...) {
        spdlog::error("An unknown error occurred while compiling script: {}", name);
        isValid = false;
        return isValid;
    }
}

void CommandScript::onCompile()
{
    // Execute the base onCompile
    execBase();

    if (isValid) {
        // Load the exports table
        lua_getglobal(luaState, "exports");

        // Check if the exports table exists
        if (lua_istable(luaState, -1)) {
            // Load the command name
            lua_getfield(luaState, -1, "name");
            if (lua_isstring(luaState, -1)) {
                commandName = lua_tostring(luaState, -1);
            } else {
                // Invalidate the command and return
                spdlog::error("Failed to load command name for script: {}", name);
                ChatUtils::displayClientMessage("§cFailed to load command name for script: " + name);
                isValid = false;
                return;
            }

            lua_pop(luaState, 1);

            // Load the command aliases
            lua_getfield(luaState, -1, "aliases");
            if (lua_istable(luaState, -1)) {
                // use lua_next
                lua_pushnil(luaState);

                while (lua_next(luaState, -2) != 0) {
                    if (lua_isstring(luaState, -1)) {
                        commandAliases.push_back(lua_tostring(luaState, -1));
                    }
                    lua_pop(luaState, 1);
                }
            } else {
                // Invalidate the command and return
                spdlog::warn("No command aliases found for script: {}", name);
                ChatUtils::displayClientMessage("§eWarning: No command aliases found for script: " + name);
            }

            lua_pop(luaState, 1);

            // Load the command description
            lua_getfield(luaState, -1, "description");
            if (lua_isstring(luaState, -1)) {
                commandDescription = lua_tostring(luaState, -1);
            } else {
                commandDescription = "No description provided.";
                spdlog::warn("No command description found for script: {}", name);
                ChatUtils::displayClientMessage("§eWarning: No command description found for script: " + name);
            }
        }

        // make sure the script contains a runCommand function
        lua_getglobal(luaState, "runCommand");
        if (!lua_isfunction(luaState, -1)) {
            spdlog::error("runCommand is not a function or is missing in the script.");
            ChatUtils::displayClientMessage("§cError: runCommand is not a function or is missing in script " + name);
            isValid = false;
        }

        spdlog::info("Successfully executed onCompile for script: {}", name);
    } else {
        spdlog::error("Failed to execute onCompile for script: {}", name);
    }
}


void ModuleScript::onCompile()
{
    std::lock_guard<std::mutex> lock(eventMutex);
    registerModuleFunction("registerEvent", [](lua_State* L) -> int {
        const char* eventName = luaL_checkstring(L, 1);

        if (!lua_isfunction(L, 2)) {
            luaL_error(L, "Expected a function as the second argument");
        }

        // Ensure the _G.eventHandlers table exists
        lua_getglobal(L, "eventHandlers");
        if (lua_isnil(L, -1)) {
            lua_pop(L, 1);  // pop nil
            lua_newtable(L);  // create eventHandlers table
            lua_setglobal(L, "eventHandlers");
            lua_getglobal(L, "eventHandlers");
        }

        // Store the event function in the eventHandlers table
        lua_pushstring(L, eventName);  // push event name
        lua_pushvalue(L, 2);  // copy the function to the top of the stack
        lua_settable(L, -3);  // eventHandlers[eventName] = function

        lua_pop(L, 1);  // pop eventHandlers table

        return LUA_OK;
    });




    // Execute the base onCompile
    execBase();

    // Load the exports table
    lua_getglobal(luaState, "exports");

    // Check if the exports table exists
    if (lua_istable(luaState, -1)) {
        // Load the module name
        lua_getfield(luaState, -1, "name");
        if (lua_isstring(luaState, -1)) {
            moduleName = lua_tostring(luaState, -1);
        } else {
            // Invalidate the module and return
            spdlog::error("Failed to load module name for script: {}", name);
            ChatUtils::displayClientMessage("§cFailed to load module name for script: " + name);
            isValid = false;
            return;
        }

        lua_pop(luaState, 1);

        // Load the module description
        lua_getfield(luaState, -1, "description");
        if (lua_isstring(luaState, -1)) {
            moduleDescription = lua_tostring(luaState, -1);
        } else {
            moduleDescription = "No description provided.";
            spdlog::warn("No module description found for script: {}", name);
            ChatUtils::displayClientMessage("§eWarning: No module description found for script: " + name);
        }

        lua_pop(luaState, 1);

        // Load the module author
        lua_getfield(luaState, -1, "author");
        if (lua_isstring(luaState, -1)) {
            moduleAuthor = lua_tostring(luaState, -1);
        } else {
            moduleAuthor = "Unknown";
            spdlog::warn("No module author found for script: {}", name);
            ChatUtils::displayClientMessage("§eWarning: No module author found for script: " + name);
        }

        lua_pop(luaState, 1);

        // Load the module enabled state
        lua_getfield(luaState, -1, "enabled");
        if (lua_isboolean(luaState, -1)) {
            enabled = lua_toboolean(luaState, -1);
        } else {
            enabled = false;
            spdlog::warn("No enabled state found for script: {}", name);
            ChatUtils::displayClientMessage("§eWarning: No enabled state found for script: " + name);
        }

        lua_pop(luaState, 1);

        //ignoreEnabled
        lua_getfield(luaState, -1, "ignoreEnabledState");
        if (lua_isboolean(luaState, -1)) {
            ignoreEnabled = lua_toboolean(luaState, -1);
        } else {
            ignoreEnabled = false;
            spdlog::warn("No ignoreEnabledState found for script: {}", name);
            ChatUtils::displayClientMessage("§eWarning: No ignoreEnabled state found for script: " + name);
        }

        lua_pop(luaState, 1);

        // Get the keybind
        lua_getfield(luaState, -1, "keybind");
        if (lua_isstring(luaState, -1)) {
            keybind = Keyboard::mKeyMap[StringUtils::toLower(lua_tostring(luaState, -1))];
        } else {
            keybind = 0;
            spdlog::warn("No keybind found for script: {}", name);
            ChatUtils::displayClientMessage("§eWarning: No keybind found for script: " + name);
        }
    }

    /* example script
    exports = {
        name = "test",
        description = "A test module.",
        author = "vastrakai",
        enabled = true
    }

    function onBaseTick()
        print("Base tick event called.")
    end

    registerEvent("onBaseTick", onBaseTick)
    */
}

void ModuleScript::execute()
{
    spdlog::error("Cannot execute ModuleScript::execute for script: {}", name);
}

void ModuleScript::execEvent(const std::string& eventName)
{
    bool isEnableOrDisable = (eventName == "onEnable" || eventName == "onDisable");
    if (!enabled && (!ignoreEnabled && !isEnableOrDisable)) return;

    std::lock_guard<std::mutex> lock(eventMutex);

    lua_getglobal(luaState, "eventHandlers");
    if (lua_istable(luaState, -1)) {
        lua_pushstring(luaState, eventName.c_str());
        lua_gettable(luaState, -2);  // get eventHandlers[eventName]

        if (lua_isfunction(luaState, -1)) {
            if (!TRY_CALL([&]() {
                if (lua_pcall(luaState, 0, 0, 0) != LUA_OK) {
                    const char* errorMsg = lua_tostring(luaState, -1);
                    spdlog::error("Error executing event {}: {}", eventName, errorMsg);
                    ChatUtils::displayClientMessage("§cError executing event {} in {}: {}", eventName, name, errorMsg);
                    lua_pop(luaState, 1);  // pop error
                }
            })) {
                ChatUtils::displayClientMessage("§cError executing event {} in {}: {}", eventName, name, "An error occurred while executing the event.");
                spdlog::error("An error occurred while executing event {} in {}: {}", eventName, name);
            }
        } else {
            lua_pop(luaState, 1);  // pop nil or invalid type
        }
    } else {
        spdlog::error("eventHandlers table is missing or not valid.");
    }
    lua_pop(luaState, 1);  // pop eventHandlers table
}

// always_false is a helper struct to make static_assert fail
template <typename T>
struct always_false : std::false_type {};


void ModuleScript::execEvent(const std::string& eventName, EventData* eventData)
{
    if (!enabled && !ignoreEnabled) return;

    std::lock_guard<std::mutex> lock(eventMutex);

    lua_getglobal(luaState, "eventHandlers");
    if (lua_istable(luaState, -1)) {
        lua_pushstring(luaState, eventName.c_str());
        lua_gettable(luaState, -2);

        if (lua_isfunction(luaState, -1)) {
            // Push eventData table to Lua
            lua_newtable(luaState);
            for (const auto& [key, value] : *eventData) {
                lua_pushstring(luaState, key.c_str());

                // Handle different types (using std::visit to deal with variant types)
                std::visit([this]<typename luaval_t>(luaval_t&& arg) {
                    using T = std::decay_t<luaval_t>;
                    if constexpr (std::is_same_v<T, std::string>)
                        lua_pushstring(luaState, arg.c_str());
                    else if constexpr (std::is_same_v<T, double>)
                        lua_pushnumber(luaState, arg);
                    else if constexpr (std::is_same_v<T, bool>)
                        lua_pushboolean(luaState, arg);
                    else if constexpr (std::is_same_v<T, int>)
                        lua_pushinteger(luaState, arg);
                    else if constexpr (std::is_same_v<T, void*>)
                        lua_pushlightuserdata(luaState, arg);
                    else
                        static_assert(always_false<T>::value, "non-exhaustive visitor!");
                }, value);

                lua_settable(luaState, -3);
            }

            // Call Lua function with the eventData table
            if (!TRY_CALL([&]()
            {
                if (lua_pcall(luaState, 1, 1, 0) != LUA_OK) {
                    const char* errorMsg = lua_tostring(luaState, -1);
                    spdlog::error("Error executing event {}: {}", eventName, errorMsg);
                    ChatUtils::displayClientMessage("§cError executing event {} in {}: {}", eventName, name, errorMsg);
                    lua_pop(luaState, 1);  // pop error
                } else {
                    // Check if Lua returned a table
                    if (lua_istable(luaState, -1)) {
                        // Update eventData with modified values from Lua
                        for (const auto& [key, value] : *eventData) {
                            lua_pushstring(luaState, key.c_str());
                            lua_gettable(luaState, -2);

                            // Update C++ eventData based on Lua modifications
                            if (lua_isstring(luaState, -1)) {
                                (*eventData)[key] = std::string(lua_tostring(luaState, -1));
                            } else if (lua_isnumber(luaState, -1)) {
                                (*eventData)[key] = lua_tonumber(luaState, -1);
                            } else if (lua_isboolean(luaState, -1)) {
                                (*eventData)[key] = static_cast<bool>(lua_toboolean(luaState, -1));
                            } else if (lua_isnumber(luaState, -1))
                            {
                                (*eventData)[key] = lua_tointeger(luaState, -1);
                            } // no need to pop back user data
                            lua_pop(luaState, 1);  // pop the value
                        }
                    }
                }
            })) {
                ChatUtils::displayClientMessage("§cError executing event {} in {}: {}", eventName, name, "An error occurred while executing the event.");
                spdlog::error("An error occurred while executing event {} in {}: {}", eventName, name);
            }

            lua_pop(luaState, 1);  // pop the returned table or value
        } else {
            lua_pop(luaState, 1);  // pop nil or invalid type
        }
    } else {
        spdlog::error("eventHandlers table is missing or not valid.");
    }
    lua_pop(luaState, 1);  // pop eventHandlers table
}

void ModuleScript::triggerStateChangeEvent()
{
    auto holder = nes::make_holder<ModuleScriptStateChangeEvent>(this, enabled, !enabled);
    gFeatureManager->mDispatcher->trigger(holder);
    if (holder->isCancelled()) return;

    { // scoped-lock
        std::lock_guard<std::mutex> lock(eventMutex);

        lua_getglobal(luaState, "exports");
        lua_pushboolean(luaState, enabled);
        lua_setfield(luaState, -2, "enabled");
        lua_pop(luaState, 1);
    }

    execEvent(enabled ? "onEnable" : "onDisable");
}

CommandScript::CommandScript(const std::string& scriptName, const std::string& scriptCode): SolsticeScript(scriptName, scriptCode)
{
    type = SolsticeScriptType::COMMAND;
}

bool CommandScript::isMatching(const std::string& command)
{
    // use insensitive comparison
    if (StringUtils::equalsIgnoreCase(command, commandName)) {
        return true;
    }

    for (const auto& alias : commandAliases) {
        if (StringUtils::equalsIgnoreCase(command, alias)) {
            return true;
        }
    }
    return false;
}

void CommandScript::execute()
{
    if (isValid) {
        spdlog::error("Cannot execute CommandScript::execute for script: {}", name);
    }
}

void CommandScript::execCommand(std::vector<std::string>& args)
{
    if (isValid) {
        lua_getglobal(luaState, "runCommand");
        if (lua_isfunction(luaState, -1)) {
            lua_newtable(luaState);
            for (size_t i = 0; i < args.size(); i++) {
                lua_pushstring(luaState, args[i].c_str());
                lua_rawseti(luaState, -2, i + 1);
            }

            // Call the function with error handling
            int result = lua_pcall(luaState, 1, 0, 0);
            if (result != LUA_OK) {
                const char* errorMsg = lua_tostring(luaState, -1);
                if (errorMsg) {
                    spdlog::error("Error executing command: {}", errorMsg);
                    ChatUtils::displayClientMessage("§cError in {}: {}", name, errorMsg);
                } else {
                    spdlog::error("Unknown error occurred during command execution.");
                    ChatUtils::displayClientMessage("§cUnknown error occurred during execution of command: {}", name);
                }
                lua_pop(luaState, 1); // Pop the error message from the stack
            }
        } else {
            spdlog::error("runCommand is not a function or is missing in the script.");
            ChatUtils::displayClientMessage("§cError: runCommand is not a function or is missing in the script.");
            lua_pop(luaState, 1); // Pop non-function value
        }
    } else {
        spdlog::error("Cannot execute command: script is not valid.");
    }
}


/* Script example
exports = {
    name = "test",
    aliases = {"t"},
    description = "A test command.",
}

function runCommand(args)
    print("Running command: " .. args[1])
end
*/


void CommandScript::registerCommand(const std::string& commandName, lua_CFunction commandFunc)
{
    lua_pushcfunction(luaState, commandFunc, commandName.c_str());
    lua_setglobal(luaState, commandName.c_str());
}
