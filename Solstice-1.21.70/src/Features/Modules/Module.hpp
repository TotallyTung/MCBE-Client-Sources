#pragma once
//
// Created by vastrakai on 6/28/2024.
//

#include "ModuleCategory.hpp"
#include <string>
#include <utility>
#include <vector>

#include <nlohmann/json.hpp>
#include <Utils/Logger.hpp>

#include "Setting.hpp"
#include "Features/Events/ModuleStateChangeEvent.hpp"

enum NamingStyle {
    Lowercase,
    LowercaseSpaced,
    Normal,
    NormalSpaced
};

class Module
{
public:
    std::string mName;
    std::unordered_map<NamingStyle, std::string> mNames = {
        {Lowercase, "unnamed"},
        {LowercaseSpaced, "unnamed"},
        {Normal, "Unnamed"},
        {NormalSpaced, "Unnamed"}
    };
    std::string mDescription;
    ModuleCategory mCategory;
    bool mEnabled = false;
    bool mWantedState = false; // pretty much a way of queueing up a state change for the next client tick
    int mKey;
    int mEnableWhileHeld = false;

    BoolSetting mVisibleInArrayList = BoolSetting("Visible", "Whether the module is visible in the Arraylist", true);

    std::vector<class Setting*> mSettings = {
        &mVisibleInArrayList
    };

    Module(std::string name, std::string description, const ModuleCategory category, const int key, const bool enabled, std::unordered_map<NamingStyle, std::string> names = {})
        : mName(std::move(name)), mDescription(std::move(description)), mCategory(category), mWantedState(enabled), mKey(key), mNames(std::move(names)) {}

    std::string getSettingDisplayText();

    virtual void onEnable() {}
    virtual void onDisable() {}
    virtual void onTick() {}
    virtual void onInit() {}
    virtual std::string getSettingDisplay() { return ""; } // Used for ArrayList
    [[nodiscard]] virtual const char* getTypeID() const = 0;

    std::string& getName();

    void setEnabled(bool enabled);
    void toggle();
    void enable();
    void disable();
    void addSetting(class Setting* setting);
    std::string& getCategory();

    // Serialize the module
    nlohmann::json serialize();
    Setting* getSetting(const std::string& string);

    template <typename... Args>
    void addSettings(Args... args) {
        (addSetting(args), ...);
    }

    float animPercentage = 0.f;
    glm::vec2 location = glm::vec2(0, 0);
    glm::vec2 size = glm::vec2(0, 0);
    glm::vec2 pos = glm::vec2(0, 0);
    bool showSettings = false;
    float cAnim = 0;
    float cFade = 0;
    float cScale = 0;

    float mArrayListAnim = 0.f;
};

template <typename T>
class ModuleBase : public Module
{
public:
    ModuleBase(std::string name, std::string description, const ModuleCategory category, const int key, const bool enabled, std::unordered_map<NamingStyle, std::string> names = {})
        : Module(std::move(name), std::move(description), category, key, enabled, std::move(names)) {}

    static const char* TypeID() {
        return typeid(T).name();
    }

    const char* getTypeID() const override {
        return TypeID();
    }
};