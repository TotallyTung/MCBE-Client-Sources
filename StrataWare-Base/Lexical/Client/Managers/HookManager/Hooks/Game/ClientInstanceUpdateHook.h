#pragma once
#include "../FuncHook.h"
#include <iostream> 

class ClientInstanceUpdateHook : public FuncHook {
private:
    using func_t = __int64(__thiscall*)(ClientInstance*, bool);
    static inline func_t oFunc;

    static void Log(const std::string& message) {
        std::cout << message << std::endl;
    }

    static __int64 ClientInstance_UpdateCallback(ClientInstance* _this, bool a2) {
        if (!_this || !oFunc) {
            Log("Error: Invalid ClientInstance or original function.");
            return 0;
        }

        try {
            Log("ClientInstance Update called.");
            Game::clientInstance = _this;
            Log("Assigned client instance to Game::clientInstance.");
        }
        catch (const std::exception& e) {
            Log("Exception caught in ClientInstance_UpdateCallback: " + std::string(e.what()));
        }
        catch (...) {
            Log("Unknown exception caught in ClientInstance_UpdateCallback.");
        }
        return oFunc(_this, a2);
    }

public:
    ClientInstanceUpdateHook() {
        OriginFunc = (void*)&oFunc;
        func = (void*)&ClientInstance_UpdateCallback;
    }
};
