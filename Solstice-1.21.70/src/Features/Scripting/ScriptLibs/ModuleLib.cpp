//
// Created by vastrakai on 9/28/2024.
//

#include "ModuleLib.hpp"

void ModuleLib::initialize(lua_State* L)
{
    getGlobalNamespace(L)
        .beginClass<ModuleManager>("ModuleManager")
        .addFunction("isModuleLoaded", [](ModuleManager* manager, std::string moduleName) {
            return manager->getModule(moduleName) != nullptr;
        })
        .addFunction("isEnabled", [](ModuleManager* manager, std::string moduleName) {
            auto module = manager->getModule(moduleName);
            return module != nullptr && module->mEnabled;
        })
        .addFunction("getModule", [](ModuleManager* manager, std::string moduleName) {
            return manager->getModule(moduleName);
        })
        .addStaticFunction("get", []() {
            return gFeatureManager->mModuleManager;
        })
        .endClass()
        .beginClass<Module>("Module")
        .addProperty("name", &Module::mName)
        .addProperty("enabled", &Module::mEnabled)
        .addProperty("description", &Module::mDescription)
        .addProperty("category", &Module::mCategory)
        .addFunction("toggle", &Module::toggle)
        .addFunction("enable", &Module::enable)
        .addFunction("disable", &Module::disable)
        .addFunction("setEnabled", &Module::setEnabled)
        .addFunction("getSettingDisplayText", &Module::getSettingDisplayText)
        .addFunction("getName", &Module::getName)
        .endClass();
}
