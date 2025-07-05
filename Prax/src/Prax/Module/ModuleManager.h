#pragma once

#include <Psapi.h>
#include <Prax/ExceptionHandler.h>

#include "Module.h"

#define REGISTER_MODULE(ModuleType) \
    namespace { \
        struct ModuleType##Register { \
            ModuleType##Register() { \
                 \
                ModuleManager::AddModule(new ModuleType()); \
            } \
        }; \
        static ModuleType##Register global_##ModuleType##Register; \
    }

#ifdef __DEBUG__
#define REGISTER_DEBUG_MODULE(ModuleType) \
    namespace { \
        struct ModuleType##Register { \
            ModuleType##Register() { \
                 \
                ModuleManager::AddModule(new ModuleType()); \
            } \
        }; \
        static ModuleType##Register global_##ModuleType##Register; \
    }
#else
#define REGISTER_DEBUG_MODULE(ModuleType)
#endif

#ifndef __RELEASEFREE__
#define REGISTER_PAID_MODULE(ModuleType) \
    namespace { \
        struct ModuleType##Register { \
             \
            ModuleType##Register() { \
                ModuleManager::AddModule(new ModuleType()); \
            } \
        }; \
        static ModuleType##Register global_##ModuleType##Register; \
    }
#else
#define REGISTER_PAID_MODULE(ModuleType)
#endif

#define ADD_MODULE(ModuleType) \
     \
    ModuleManager::AddModule(new ModuleType());

class ModuleManager {
public:
    inline static std::vector<Module*> Modules = std::vector<Module*>();
    static bool initialized;

    static inline void AddModule(Module* module) {
        Modules.push_back(module);

    }

    static Module* FindModuleByName(std::string name);
    static void Init();
    static void Shutdown();
    static void EnableModule(const std::string& name);
    static void DisableModule(const std::string& name);
    static void ToggleModule(const std::string& name);


    // templated GetModule
    template <typename T>
    static T* GetModule() {
        const char* typeID = ModuleBase<T>::TypeID();
         for (auto module : Modules) {
            if (std::strcmp(module->getTypeID(), typeID) == 0) {
                return static_cast<T*>(module);
            }
        }
        return nullptr;
    }
};