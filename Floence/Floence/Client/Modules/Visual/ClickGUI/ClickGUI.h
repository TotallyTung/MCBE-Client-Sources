#pragma once
#include "ShadowGUI.h"
#include "RiseGUI.h"
#include "FloenceGUI.h"

class ClickGUI : public Module
{
public:
    float animation = 0;
    int animationMode = 1; // Ease enum
    bool blur = true;
    int styleMode = 1; // Ease enum
    float animationSpeed = 5.0f; // Ease speed
    int scrollDirection = 0;

    char searchQuery[256];
    char h[2] = { 0 };

    ClickGUI(int keybind = Keys::INSERT, bool enabled = false) : Module("ClickGUI", "Visual", "Display all modules", keybind, enabled)
    {
        addEnum("Style", "The style of ClickGUI", { "Test", "Drop Down" }, &styleMode);
        //addEnum("Animation", "The way it zooms in", { "Expo", "Elastic" }, &animationMode);
        //addBool("Darken", "Blurs the clickgui's background", &blur);
        //addSlider("EaseSpeed", "The speed it zooms in", &animationSpeed, 5, 15);

        ingameOnly = false;
        callWhenDisabled = true;
    }

    void onEvent(ImGuiRenderEvent* e) override {
        static EasingUtil inEase;
        static ShadowGUI shadowGUI;
        static RiseGUI riseGUI;
        static FloenceGUI floenceGUI;
        this->isEnabled() ? inEase.incrementPercentage(ImRenderUtil::getDeltaTime() * animationSpeed / 10)
            : inEase.decrementPercentage(ImRenderUtil::getDeltaTime() * 2 * animationSpeed / 10);
        float inScale = ClickGUIManager::getEaseAnim(inEase, animationMode);
        if (inEase.isPercentageMax()) inScale = 1;
        animation = Math::lerp(0, 1, inEase.easeOutExpo());

        if (animation < 0.0001f) {
            return;
        }

        if (styleMode == 0) {
            floenceGUI.render(animation, inScale, scrollDirection);
        }
        else if (styleMode == 1) {
            shadowGUI.render(animation, inScale, scrollDirection, searchQuery, h, blur);
        }
        else if (styleMode == 2) {
            riseGUI.render(animation, inScale, scrollDirection, searchQuery, h, blur);
        }
    }


    void onEnabled() override
    {
        Address::getClientInstance()->grabMouse();
        for (auto key : Global::Keymap)
        {
            key.second == true ? Memory::CallFunc<void*, __int32, bool>(onSendKey, key.first, false) : 0;
        }
    }

    void onDisabled() override
    {
        Address::getClientInstance()->releaseMouse();

        searchQuery[0] = '\0';
    }

    void onEvent(MouseEvent* event) override
    {
        if (!enabled) return;
        *event->cancelled = true;
    }

    void onEvent(KeyboardEvent* event) override
    {
        if (!enabled) return;
        __int32 key = *event->Key;
        bool held = *event->Held;

        if (key == VK_ESCAPE && !ClickGUIManager::isBinding && held) {
            this->setEnabled(false);
            searchQuery[0] = '\0';
            Address::getClientInstance()->releaseMouse();
        }

        //*event->cancelled = true;
    }

    void onEvent(MouseScrollEvent* event) override
    {
        if (!enabled) return;
        bool direction = event->MouseDirection;

        if (!direction) scrollDirection++; else scrollDirection--;
    }

private:
    // Add any additional properties or methods specific to your module here
};