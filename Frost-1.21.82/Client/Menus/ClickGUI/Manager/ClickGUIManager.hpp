#pragma once

class ClickGUIManager
{
public:
    static std::vector<std::shared_ptr<Module>> getModulesInCategory(const std::string& category, std::vector<std::shared_ptr<Module>>& modules) {
        std::vector<std::shared_ptr<Module>> mods;
        std::copy_if(modules.begin(), modules.end(), std::back_inserter(mods), [category](const std::shared_ptr<Module>& mod) { return mod->getCategory() == category; });
        return mods;
    }

    static inline bool isBinding = false;

    static float getEaseAnim(EasingUtil ease) {
        return ease.easeOutExpo();
    }
};
