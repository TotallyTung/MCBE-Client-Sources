#pragma once

#include <string>
#include <vector>
#include <Prax/SDK/Game/Entity/Actor.h>
#include <Prax/Module/Settings/Setting.h>
#include <Prax/Module/Settings/BoolSetting.h>
#include <Prax/Utils/System/KeyManager.h>

#include "Settings/SettingGroup.h"

struct UIInfo
{
    float arrayListPercent = 0.0f;
    float expandingAnimationPercent = 0.0f;
    ImColor color = ImColor(0, 0, 0, 0);

    bool IsAnimating() {
        return arrayListPercent < 0.99f;
    };
};

class Module {
public:
    enum class Priority : int {
        AbsoluteFirst,
        VeryFirst,
        First,
        KindaFirst,
        Normal,
        KindaLast,
        Last,
        VeryLast,
        AbsoluteLast
    };

    std::string Name, Description, Category;
    int KeyId;

    bool Enabled, Expanded = false;
    bool Binding = false;
    bool EnableWhileHeld = false; // Only lets you enable the module while the key is held and disables it when you let go
    bool DisableOnDimensionSwitch = false;
    bool TickOnDisabled = false;
    bool RenderOnDisabled = false;
    bool HideInArraylist = false;
    float expandedHeight = 0;

    Priority PacketPriority = Priority::Normal;
    std::vector <SettingGroup*> SettingGroups;
    std::vector <Setting*> Settings;

    Setting* GetSettingByName(std::string name);

    UIInfo UIInfo;
    

    Module(std::string const& name,
            std::string_view const& description,
    std::string_view const& category,
    int keyId = 0,
    bool enable = false);

    virtual ~Module()
    {
        Logger::Write("Module", "Deleting module: " + Name);
    }
    virtual const char* getTypeID() const = 0;

    int GetVisibleSettingCount() {
        int count = 0;
        for (auto setting : Settings) {
            if (setting->IsVisible()) {
                count++;
            }
        }
        return count;
    }

    std::string GetKeyText()
    {
        std::string keyText = "None";

        if (KeyId != 0) {
            // Get the key name
            auto keys = KeyManager::KeyMap;

            for (auto key: keys) {
                if (key.second == KeyId) {
                    keyText = key.first;
                }
            }
        }

        if (Binding)
            keyText = "Binding...";

        return keyText;
    }

    std::string GetSettingDisplayText();
    void AddSetting(Setting* setting);
    void Toggle();
    void SetToggleState(bool enable);
    virtual void OnInitialize();
    virtual void OnEject();
    virtual void OnEnable();
    virtual void OnDisable();
    virtual void OnTick();
    virtual void OnClientTick();
    virtual void OnRender();
    virtual void OnTessellate();
    virtual void OnSettingRender();
    virtual void OnRenderMatrix(glm::mat4* matrix) {}
    virtual void OnSlowTick(Actor*) {}
    virtual void OnEngineRender(class MinecraftUIRenderContext* ctx, class Font* font) {}
    virtual void OnKey(uint64_t key, bool held) {}
    virtual void OnPacket(class LoopbackPacketSender* lbps, Packet* packet, bool* canceled) {}
    virtual void OnPacketReceive(std::shared_ptr<Packet> packet, bool* cancel) {}
    virtual void OnMouseInput(void* Instance, int mouseButton, int isDown, __int16 mouseX, __int16 mouseY, __int64 a1, __int64 a2, __int64 a3, bool* sendToGame) {}
    virtual void OnLerp(Actor* actor, Vector3& vel) {}
    virtual void OnSocketRecieve(char* buf) {}


};

template <typename T>
class ModuleBase : public Module {
public:
    ModuleBase(std::string const& name,
               std::string_view const& description,
               std::string_view const& category,
               int keyId = 0,
               bool enable = false) : Module(name, description, category, keyId, enable) {}

    static const char* TypeID() {
        return typeid(T).name();
    }

    const char* getTypeID() const override {
        return TypeID();
    }
};