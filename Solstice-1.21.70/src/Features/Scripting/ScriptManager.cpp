//
// Created by vastrakai on 9/22/2024.
//

#include "ScriptManager.hpp"

#include <lua.h>
#include <lualib.h>
#include <Features/Events/BaseTickEvent.hpp>
#include <Features/Events/KeyEvent.hpp>
#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/RenderEvent.hpp>

#include "luacode.h"

#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Network/Packets/MovePlayerPacket.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/TextPacket.hpp>

#include "ScriptLibs/ClientInstanceLib.hpp"
#include "ScriptLibs/FSLib.hpp"
#include "ScriptLibs/InventoryLib.hpp"
#include "ScriptLibs/KeyboardLib.hpp"
#include "ScriptLibs/LocalPlayerLib.hpp"
#include "ScriptLibs/MathLib.hpp"
#include "ScriptLibs/MemoryLib.hpp"
#include "ScriptLibs/ModuleLib.hpp"
#include "ScriptLibs/NetworkLib.hpp"
#include "ScriptLibs/RenderLib.hpp"
#include "ScriptLibs/ScriptLib.hpp"
#include "ScriptLibs/WorldLib.hpp"


void ScriptManager::registerFunction(lua_State* state, lua_CFunction func, const char* functionName,
                                     const char* tableName)
{
    lua_getglobal(state, tableName);

    if (lua_isnil(state, -1))
    {
        lua_pop(state, 1);
        lua_newtable(state);
    }

    lua_pushcfunction(state, func, functionName);
    lua_setfield(state, -2, functionName);

    lua_setglobal(state, tableName);
}

// register global function
void ScriptManager::registerFunction(lua_State* state, lua_CFunction func, const char* functionName)
{
    lua_pushcfunction(state, func, functionName);
    lua_setglobal(state, functionName);
}

void ScriptManager::registerFunctions(lua_State* state)
{
    // Redefine print function to use chat
    registerFunction(state, [](lua_State* L) -> int
    {
        std::string message;
        int n = lua_gettop(L); // number of arguments
        lua_getglobal(L, "tostring"); // Get the tostring function

        for (int i = 1; i <= n; i++)
        {
            lua_pushvalue(L, -1); // Push tostring function
            lua_pushvalue(L, i); // Push the argument to be converted to string
            lua_call(L, 1, 1); // Call tostring(L, i)

            const char* s = lua_tostring(L, -1); // Get result from stack
            if (s == nullptr)
            {
                luaL_error(L, "'tostring' must return a string to 'print'");
            }

            if (i > 1) message += "\t"; // Add tab for multiple arguments
            message += s;
            lua_pop(L, 1); // Pop the result from stack
        }

        ChatUtils::displayClientMessageRaw(message); // Send message to chat
        return 0;
    }, "print");

    registerFunction(state, [](lua_State* L) -> int
    {
        auto message = luaL_checkstring(L, 1);
        ChatUtils::displayClientMessage(std::string(message));
        return 0;
    }, "displayClientMessage", "ChatUtils");

    registerFunction(state, [](lua_State* L) -> int
    {
        auto message = luaL_checkstring(L, 1);
        auto duration = luaL_checknumber(L, 2);
        auto type = luaL_checkstring(L, 3);

        if (strcmp(type, "Info") == 0)
        {
            NotifyUtils::notify(std::string(message), duration, Notification::Type::Info);
        }
        else if (strcmp(type, "Warning") == 0)
        {
            NotifyUtils::notify(std::string(message), duration, Notification::Type::Warning);
        }
        else if (strcmp(type, "Error") == 0)
        {
            NotifyUtils::notify(std::string(message), duration, Notification::Type::Error);
        }
        else
        {
            luaL_error(L, "Invalid notification type!");
        }

        return 0;
    }, "notify", "NotifyUtils"); //example: NotifyUtils.notify("message", 5, "Info")

    // setEnabled (for disabling the current script)
    registerFunction(state, [](lua_State* L) -> int
    {
        auto _this = gFeatureManager->mScriptManager;
        if (_this == nullptr) return 0;

        bool enabled = lua_toboolean(L, 1);
        std::string name;
        // Get the name from exports table .name
        lua_getglobal(L, "exports");
        lua_getfield(L, -1, "name");
        name = lua_tostring(L, -1);
        lua_pop(L, 2);


        for (const auto& script : _this->mScripts)
        {
            if (script->name == name)
            {
                if (script->type != SolsticeScriptType::MODULE)
                {
                    luaL_error(L, "Script is not a module");
                    return -1;
                }

                auto moduleScript = std::static_pointer_cast<ModuleScript>(script);
                moduleScript->enabled = enabled;
                moduleScript->triggerStateChangeEvent();
                return LUA_OK;
            }
        }

        luaL_error(L, "Script not found? (please report this...)"); //noreturn
    }, "setEnabled");

    // toggle
    registerFunction(state, [](lua_State* L) -> int
    {
        auto _this = gFeatureManager->mScriptManager;
        if (_this == nullptr) return 0;

        std::string name;
        // Get the name from exports table .name
        lua_getglobal(L, "exports");
        lua_getfield(L, -1, "name");
        name = lua_tostring(L, -1);
        lua_pop(L, 2);

        for (const auto& script : _this->mScripts)
        {
            if (script->name == name)
            {
                if (script->type != SolsticeScriptType::MODULE)
                {
                    luaL_error(L, "Script is not a module");
                    return -1;
                }

                auto moduleScript = std::static_pointer_cast<ModuleScript>(script);
                moduleScript->enabled = !moduleScript->enabled;
                moduleScript->triggerStateChangeEvent();
                return LUA_OK;
            }
        }

        luaL_error(L, "Script not found? (please report this...)"); //noreturn
    }, "toggle");

    ScriptLib::registerLib<FSLib>(state);
    ScriptLib::registerLib<ClientInstanceLib>(state);
    ScriptLib::registerLib<WorldLib>(state);
    ScriptLib::registerLib<LocalPlayerLib>(state);
    ScriptLib::registerLib<KeyboardLib>(state);
    ScriptLib::registerLib<RenderLib>(state);
    ScriptLib::registerLib<MathLib>(state);
    ScriptLib::registerLib<MemoryLib>(state);
    ScriptLib::registerLib<ModuleLib>(state);
    ScriptLib::registerLib<NetworkLib>(state);
    ScriptLib::registerLib<InventoryLib>(state);

    spdlog::info("[lua] registered functions");
}

#define CHECK_AND_CANCEL() if (std::get<bool>(eventData["cancelled"])) { event.cancel(); return; }


void ScriptManager::init()
{
    spdlog::info("initializing ScriptManager");

    mGlobalLuaState = luaL_newstate();
    spdlog::info("Lua state created");
    luaL_openlibs(mGlobalLuaState);
    spdlog::info("Lua libraries opened");

    registerFunctions(mGlobalLuaState);
    registerEvents(mGlobalLuaState);
    ScriptLib::generateDocs(mGlobalLuaState);

    spdlog::info("Successfully initialized Lua state");

    loadUserScripts();
    startFileWatcher();

    gFeatureManager->mDispatcher->listen<ClientTickEvent, &ScriptManager::onClientTick>(this);

    gFeatureManager->mDispatcher->listen<BaseTickEvent>([this](BaseTickEvent& event)
    {
        dispatchEvent("onBaseTick");
    });
    gFeatureManager->mDispatcher->listen<RenderEvent>([this](RenderEvent& event)
    {
        dispatchEvent("onRender");
    });
    gFeatureManager->mDispatcher->listen<PacketInEvent>([this](PacketInEvent& event)
    {
        // handle chat
        if (event.mPacket->getId() == PacketID::Text)
        {
            auto packet = event.getPacket<TextPacket>();
            std::string message = packet->mMessage;
            if (packet->mType == TextPacketType::Chat) message = "<" + packet->mAuthor + "> " + message;

            EventData eventData;
            eventData["message"] = message;
            eventData["author"] = packet->mAuthor;
            eventData["type"] = std::string(magic_enum::enum_name(packet->mType));
            eventData["cancelled"] = event.mCancelled;

            dispatchEvent("onChatReceived", &eventData);
            CHECK_AND_CANCEL();
        }

        switch (event.mPacket->getId())
        {
        case PacketID::Text:
            {
                auto packet = event.getPacket<TextPacket>();

                EventData eventData;
                eventData["cancelled"] = event.mCancelled;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["packet"] = packet.get();

                dispatchEvent("onPacketReceived", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        case PacketID::MovePlayer:
            {
                auto packet = event.getPacket<MovePlayerPacket>();

                EventData eventData;
                eventData["cancelled"] = event.mCancelled;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["packet"] = packet.get();

                dispatchEvent("onPacketReceived", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        default:
            {
                EventData eventData;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["cancelled"] = event.mCancelled;

                dispatchEvent("onPacketReceived", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        }
    });

    gFeatureManager->mDispatcher->listen<PacketOutEvent>([this](PacketOutEvent& event)
    {
        switch (event.mPacket->getId())
        {
        case PacketID::Text:
            {
                auto packet = event.getPacket<TextPacket>();
                std::string message = packet->mMessage;
                if (packet->mType == TextPacketType::Chat) message = "<" + packet->mAuthor + "> " + message;

                EventData eventData;
                eventData["message"] = message;
                eventData["author"] = packet->mAuthor;
                eventData["type"] = std::string(magic_enum::enum_name(packet->mType));
                eventData["cancelled"] = event.mCancelled;

                dispatchEvent("onChatSent", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        case PacketID::PlayerAuthInput:
            {
                auto packet = event.getPacket<PlayerAuthInputPacket>();

                EventData eventData;
                eventData["cancelled"] = event.mCancelled;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["packet"] = packet;

                dispatchEvent("onPacketSent", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        case PacketID::MovePlayer:
            {
                auto packet = event.getPacket<MovePlayerPacket>();

                EventData eventData;
                eventData["cancelled"] = event.mCancelled;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["packet"] = packet;

                dispatchEvent("onPacketSent", &eventData);
                CHECK_AND_CANCEL();
                break;
            }
        default:
            {
                EventData eventData;
                eventData["packetId"] = std::to_string((int)event.mPacket->getId());
                eventData["packetName"] = event.mPacket->getName();
                eventData["cancelled"] = event.mCancelled;

                dispatchEvent("onPacketSent", &eventData);
                CHECK_AND_CANCEL();
            }
        }
    });


    gFeatureManager->mDispatcher->listen<KeyEvent>([this](KeyEvent& event)
    {
        if (event.mPressed && !ClientInstance::get()->getMouseGrabbed() && ClientInstance::get()->getScreenName() != "chat_screen")
        {
            // Go through each script and check if the keybind matches
            for (const auto& script : mScripts)
            {
                if (script->type != SolsticeScriptType::MODULE) continue;

                auto moduleScript = std::static_pointer_cast<ModuleScript>(script);

                if (moduleScript->keybind == event.mKey)
                {
                    moduleScript->enabled = !moduleScript->enabled;
                    moduleScript->triggerStateChangeEvent();
                }
            }
        }

        EventData eventData;
        eventData["key"] = event.mKey;
        std::string keyStr = "unk" + std::to_string(event.mKey);
        for (const auto& [key, value] : Keyboard::mKeyMap)
        {
            if (value == event.mKey)
            {
                keyStr = key;
                break;
            }
        }
        eventData["keyString"] = keyStr;
        eventData["pressed"] = event.mPressed;
        eventData["cancelled"] = event.mCancelled;
        dispatchEvent("onKeyPress", &eventData);
        CHECK_AND_CANCEL();
    });
}

void ScriptManager::registerEvents(lua_State* state)
{
    // define registerEvent function
    registerFunction(state, [](lua_State* L) -> int
    {
        auto eventName = luaL_checkstring(L, 1);


        auto _this = gFeatureManager->mScriptManager;
        if (_this == nullptr) return 0;

        // if the string isn't valid, return
        if (eventName == nullptr || strlen(eventName) == 0)
        {
            luaL_error(L, "Invalid event name!");
            return -1;
        }

        return 0;
    }, "registerEvent", "ScriptManager");
}

void ScriptManager::execTests()
{
    NotifyUtils::notify("no", 5.f, Notification::Type::Info);
}

void ScriptManager::shutdown()
{
    spdlog::info("shutting down ScriptManager");

    lua_close(mGlobalLuaState);

    gFeatureManager->mDispatcher->deafen<ClientTickEvent, &ScriptManager::onClientTick>(this);
}


bool ScriptManager::findAndExecCommand(const std::string& string, const std::vector<std::string>& args)
{
    for (const auto& s : mScripts)
    {
        // Continue if the script type is not a command
        if (s->type != SolsticeScriptType::COMMAND)
        {
            spdlog::info("Skipping non-command script: {}", s->name);

            continue;
        }
        auto script = std::static_pointer_cast<CommandScript>(s);

        if (script->isMatching(string))
        {
            auto a = std::vector<std::string>(args.begin() + 1, args.end()); // Exclude the command name
            script->execCommand(a);
            return true;
        }
    }

    spdlog::error("Failed to find script: {}", string);
    return false;
}

std::vector<std::shared_ptr<SolsticeScript>>& ScriptManager::getScripts()
{
    return mScripts;
}

bool ScriptManager::isCompilable(const std::string& code)
{
    size_t bytecodeSize = 0;
    // compile option
    static lua_CompileOptions* options = new lua_CompileOptions();
    options->debugLevel = 1;
    options->optimizationLevel = 1;
    options->typeInfoLevel = 0;
    options->coverageLevel = 0;

    char* bytecode = luau_compile(code.c_str(), strlen(code.c_str()), options, &bytecodeSize);
    free(bytecode);

    return bytecodeSize > 0;
}

void ScriptManager::execute(const std::string& code)
{
    try
    {
        size_t bytecodeSize = 0;

        static lua_CompileOptions* options = new lua_CompileOptions();
        options->debugLevel = 1;
        options->optimizationLevel = 1;
        options->typeInfoLevel = 0;
        options->coverageLevel = 0;

        char* bytecode = luau_compile(code.c_str(), strlen(code.c_str()), options, &bytecodeSize);
        int result = luau_load(mGlobalLuaState, "ScriptManager", bytecode, bytecodeSize, 0);
        free(bytecode);

        if (result != LUA_OK)
        {
            const char* err = lua_tostring(mGlobalLuaState, -1);
            spdlog::error("Failed to execute code: {}", err);
            lua_pop(mGlobalLuaState, 1);
        }

        int r = lua_pcall(mGlobalLuaState, 0, 1, 0);
        if (r != 0)
        {
            const char* err = lua_tostring(mGlobalLuaState, -1);
            printf("[lua] %s\n", err);
            ChatUtils::displayClientMessage("§c" + std::string(err));
            lua_pop(mGlobalLuaState, 1);
        }
    }
    catch (const std::exception& e)
    {
        spdlog::error("Failed to execute code: {}", e.what());
    } catch (...)
    {
        spdlog::error("Failed to execute code: unknown error");
    }
}


void ScriptManager::loadUserScripts()
{
    mLoadingScripts = true;
    // Set up the directory path for user scripts
    std::string scriptPath = FileUtils::getSolsticeDir() + "Scripts\\";
    std::vector<std::string> scriptFiles;

    // Load command scripts in Scripts\Commands
    for (const auto& entry : std::filesystem::directory_iterator(scriptPath + "Commands\\"))
    {
        if (entry.path().extension() == ".lua")
        {
            std::string scriptName = entry.path().filename().string();
            FILETIME lastWriteTime;

            // Get the last write time of the file
            HANDLE fileHandle = CreateFile(entry.path().string().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (fileHandle != INVALID_HANDLE_VALUE)
            {
                GetFileTime(fileHandle, nullptr, nullptr, &lastWriteTime);
                CloseHandle(fileHandle);
            }

            // Check if script is already loaded and unchanged
            if (!mKnownScripts.contains(scriptName))
            {
                // Load script content
                std::ifstream scriptFile(entry.path().string());
                std::string scriptCode((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

                // Create a new script object
                auto solsticeScript = std::make_shared<CommandScript>(scriptName, scriptCode);

                // Compile the script
                if (solsticeScript->compile())
                {
                    mScripts.push_back(solsticeScript);
                    mKnownScripts.insert(scriptName);
                    mFileTimes[scriptName] = lastWriteTime;
                    spdlog::info("Loaded command script: {}", scriptName);
                    ChatUtils::displayClientMessage("Loaded command script: " + scriptName);
                }
                else
                {
                    spdlog::error("Failed to compile script: {}", scriptName);
                    ChatUtils::displayClientMessage("Failed to compile script: " + scriptName);
                }
            }
        }
    }

    // Load module scripts in Scripts\Modules
    for (const auto& entry : std::filesystem::directory_iterator(scriptPath + "Modules\\"))
    {
        if (entry.path().extension() == ".lua")
        {
            std::string scriptName = entry.path().filename().string();
            FILETIME lastWriteTime;

            // Get the last write time of the file
            HANDLE fileHandle = CreateFile(entry.path().string().c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr,
                                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
            if (fileHandle != INVALID_HANDLE_VALUE)
            {
                GetFileTime(fileHandle, nullptr, nullptr, &lastWriteTime);
                CloseHandle(fileHandle);
            }

            // Check if script is already loaded and unchanged
            if (!mKnownScripts.contains(scriptName) || CompareFileTime(&mFileTimes[scriptName], &lastWriteTime) != 0)
            {
                // Load script content
                std::ifstream scriptFile(entry.path().string());
                std::string scriptCode((std::istreambuf_iterator<char>(scriptFile)), std::istreambuf_iterator<char>());

                // Create a new script object
                auto solsticeScript = std::make_shared<ModuleScript>(scriptName, scriptCode);

                // Compile the script
                if (solsticeScript->compile())
                {
                    mScripts.push_back(solsticeScript);
                    mKnownScripts.insert(scriptName);
                    mFileTimes[scriptName] = lastWriteTime;
                    spdlog::info("Loaded module script: {}", scriptName);
                    ChatUtils::displayClientMessage("Loaded module script: " + scriptName);
                }
                else
                {
                    spdlog::error("Failed to compile script: {}", scriptName);
                    ChatUtils::displayClientMessage("Failed to compile script: " + scriptName);
                }
            }
        }
    }

    spdlog::info("Loaded {} scripts", mScripts.size());
    mLoadingScripts = false;
}

void ScriptManager::unloadUserScripts()
{
    // Iterate through loaded scripts and clean up
    for (auto& script : mScripts)
    {
        if (script->type == SolsticeScriptType::MODULE)
        {
            // Disable the script
            auto moduleScript = std::static_pointer_cast<ModuleScript>(script);
            if (moduleScript->enabled)
            {
                moduleScript->enabled = false;
                moduleScript->triggerStateChangeEvent();
            }
        }
        else if (script->getLuaState() != nullptr)
        {
            lua_close(script->getLuaState());
        }
    }

    // Clear script data
    mScripts.clear();
    mKnownScripts.clear();

    mLoadingScripts = true;
    spdlog::info("Unloaded all user scripts");
}

void ScriptManager::dispatchEvent(const std::string& eventName)
{
    if (mLoadingScripts) return;
    for (const auto& script : mScripts)
    {
        if (script->type != SolsticeScriptType::MODULE) continue;

        auto moduleScript = std::static_pointer_cast<ModuleScript>(script);
        moduleScript->execEvent(eventName);
    }
}

void ScriptManager::dispatchEvent(const std::string& eventName, EventData* eventData)
{
    if (mLoadingScripts) return;
    for (const auto& script : mScripts)
    {
        if (script->type != SolsticeScriptType::MODULE) continue;

        auto moduleScript = std::static_pointer_cast<ModuleScript>(script);
        moduleScript->execEvent(eventName, eventData);
    }
}

void ScriptManager::onClientTick(ClientTickEvent& event)
{
    if (!mRunning) return;

    static uint64_t lastCheck = 0;
    if (NOW - lastCheck < 200) return;
    lastCheck = NOW;

    std::unordered_set<std::string> currentFiles; // Track current files in the directory

    // Iterate through the directory and subdirectories
    for (const auto& entry : std::filesystem::recursive_directory_iterator(mScriptDir))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                FILETIME ft;
                GetFileTime(hFile, nullptr, nullptr, &ft);
                CloseHandle(hFile);

                // Check if the file has been modified
                if (!mFileTimes.contains(filePath) || CompareFileTime(&mFileTimes[filePath], &ft) != 0)
                {
                    // Load the script
                    ChatUtils::displayClientMessage("§aChanges detected in script: {}, reloading scripts...",
                                                    entry.path().filename().string());
                    unloadUserScripts();
                    loadUserScripts();
                    mFileTimes[filePath] = ft;
                }

                currentFiles.insert(filePath);
            }
        }
    }
}

void ScriptManager::startFileWatcher()
{
    mScriptDir = FileUtils::getSolsticeDir() + "Scripts\\";

    // Initialize fileTimes with the current state of the directory
    for (const auto& entry : std::filesystem::recursive_directory_iterator(mScriptDir))
    {
        if (entry.is_regular_file())
        {
            std::string filePath = entry.path().string();
            HANDLE hFile = CreateFile(filePath.c_str(), GENERIC_READ, FILE_SHARE_READ, nullptr, OPEN_EXISTING,
                                      FILE_ATTRIBUTE_NORMAL, nullptr);
            if (hFile != INVALID_HANDLE_VALUE)
            {
                FILETIME ft;
                GetFileTime(hFile, nullptr, nullptr, &ft);
                CloseHandle(hFile);
                mFileTimes[filePath] = ft;
            }
        }
    }
};
