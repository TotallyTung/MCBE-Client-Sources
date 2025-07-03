#pragma once
#include <future>
#include <memory>
#include <vector>

#include "Module.hpp"
//
// Created by vastrakai on 6/28/2024.
//



class ModuleManager {
public:
    std::vector<std::shared_ptr<Module>> mModules;

    void init();
    void shutdown();
    void registerModule(const std::shared_ptr<Module>& module);
    std::vector<std::shared_ptr<Module>>& getModules();
    Module* getModule(const std::string& name) const;
    template <typename T>
    T* getModule()
    {
        // TypeID based search
        const auto& typeID = typeid(T).name();
        for (const auto& module : mModules)
        {
            if (module->getTypeID() == typeID)
            {
                return static_cast<T*>(module.get());
            }
        }
        return nullptr;
    }

    void removeModule(const std::string& name);
    std::vector<std::shared_ptr<Module>>& getModulesInCategory(int catId);
    std::unordered_map<std::string, std::shared_ptr<Module>> getModuleCategoryMap();
    void onClientTick();
    [[nodiscard]] nlohmann::json serialize() const;
    nlohmann::json serializeModule(Module* module);
    void deserialize(const nlohmann::json& j, bool showMessages = true);
};