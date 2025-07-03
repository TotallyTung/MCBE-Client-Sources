#pragma once
#include "../FuncHook.h"
#include "../../../../Client.h"
#include <iostream> 

class SendChatMessageHook : public FuncHook {
private:
    using func_t = bool(__thiscall*)(__int64, const std::string&);
    static inline func_t oFunc;


    static void Log(const std::string& message) {
        std::cout << message << std::endl;
    }

    static bool SendChatMessageCallback(__int64 _this, const std::string& message) {
        if (!oFunc)
            return false;

        try {
            Log("SendChatMessage called. Message: " + message);

            if (!message.empty()) {
                char firstChar = message[0];
                if (firstChar == CommandManager::prefix) {
                    Log("Command detected. Executing: " + message);
                    CommandManager::execute(message); 
                    return false;
                }
            }
        }
        catch (const std::exception& e) {
            Log("Exception caught in SendChatMessageCallback: " + std::string(e.what()));
            return false;
        }
        catch (...) {
            Log("Unknown exception caught in SendChatMessageCallback.");
            return false;
        }
        return oFunc(_this, message);
    }

public:
    SendChatMessageHook() {
        OriginFunc = (void*)&oFunc;
        func = (void*)&SendChatMessageCallback;
    }
};
