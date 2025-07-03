#pragma once

#include "Settings.h"

enum SliderType
{
    Int         = 0,
    Float       = 1,
    DoubleFloat = 2,
    TripleFloat = 3
};

class Module
{
public:
    Module(const std::string& name, const std::string& category, const std::string& description,
        __int32 keybind = 0x7, bool enabled = false) :
        mName(name), mCategory(category), mDescription(description), mEnabled(enabled), mKeybind(keybind), mVisible(true)
    {
        BoolSetting("Visible", "Draw in arraylist.", &mVisible);
    }

    virtual ~Module() = default;

    std::string getName() const { return mName; }
    std::string getDescription() const { return mDescription; }
    std::string getCategory() const { return mCategory; }
    int getKeybind() { return mKeybind; }
    bool isEnabled() const { return mEnabled; }
    bool isVisible() const { return mVisible; }
    const std::vector<Setting*> getSettings() const { return mSettings; }

    void setKeybind(__int32 _Keybind) { mKeybind = _Keybind; }
    void setEnabled(bool _Enabled) { mEnabled = _Enabled; }
    void setVisible(bool _Visible) { mVisible = _Visible; }

    void toggle() {
        mEnabled = !mEnabled;
        mEnabled ? onEnabled() : onDisabled();

        std::string str = mEnabled ? " has been enabled" : " has been disabled";
    }

    void SliderSetting(const std::string& name, const std::string& description, float* value,
        float min, float max, SliderType number = SliderType::Float, std::function<bool()> state = [] { return true; })
    {
        mSettings.emplace_back(new Setting(name, description, value, min, max, static_cast<int>(number), state));
    }

    void BoolSetting(const std::string& name, const std::string& description, bool* state,
        std::function<bool()> render = [] { return true; })
    {
        mSettings.emplace_back(new Setting(name, description, state, render));
    }

    void EnumSetting(const std::string& name, const std::string& description, std::vector<std::string> enums,
        int* iterator, std::function<bool()> render = [] { return true; })
    {
        mSettings.emplace_back(new Setting(name, description, enums, iterator, render));
    }

    virtual std::string getModeName() {
        std::ostringstream oss;
        bool firstEnum = false;
        for (auto set : mSettings)
        {
            if (set->getType() == SettingType::ENUM && set->getEnumValues().size() > 0 && set->getEnumValues()[0].size() > 0)
            {
                if (!firstEnum)
                {
                    oss << set->getEnumValues()[*set->getIterator()];
                    firstEnum = true;
                }
            }
        }
        return oss.str();
    }

    virtual void onEnabled() {};
    virtual void onDisabled() {};

    virtual void onEvent(class BaseTickEvent* event) {};
    virtual void onEvent(class ActorSetMotionEvent* event) {};
    virtual void onEvent(class DimensionEvent* event) {};
    virtual void onEvent(class ContainerTickEvent* event) {}; 
    virtual void onEvent(class ImRenderEvent* event) {};
    virtual void onEvent(class KeyboardEvent* event) {};
    virtual void onEvent(class LevelEventPacketEvent* event) {};
    virtual void onEvent(class MouseEvent* event) {};
    virtual void onEvent(class MouseScrollEvent* event) {};
    virtual void onEvent(class PacketEvent* event) {};
    virtual void onEvent(class RenderContextEvent* event) {};
    virtual void onEvent(class RunUpdateCycleEvent* event) {};
    virtual void onEvent(class BobHurtEvent* event) {};
    virtual void onEvent(class UpdateBlockEvent* event) {};

    float mAnimationPercentage = 0.f;
    Vector2<float> mArrayListPos = Vector2<float>();
    bool mSettingsExtended = false;
    float mClickGUIAnim = 0;
    float mClickGUIScale = 0;
    float mSettingsExtendAnimation = 0;

    bool mToggleInGameOnly = true;
    bool mCallEventsWhenDisabled = false;
protected:
    std::vector<Setting*> mSettings;
    std::string mName;
    std::string mDescription;
    std::string mCategory;
    __int32 mKeybind;
    bool mEnabled;
    bool mVisible;
};