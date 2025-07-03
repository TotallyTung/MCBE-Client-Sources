#pragma once
#include "RenderGUI.h"

class ClickGUI : public Module
{
public:
    float animation = 0;
    int animationMode = 0; // Ease enum
    float animationSpeed = 15.f; // Ease speed
    int scrollDirection = 0;

    ClickGUI(int keybind = Keyboard::INSERT, bool enabled = false) : Module("ClickGUI", "Render", "Display all modules", keybind, enabled)
    {
        registerEnumSetting("Ease", "The way it zooms in", { "Expo", "Elastic" }, &animationMode);
        registerFloatSetting("EaseSpeed", "The speed it zooms in", &animationSpeed, 5, 15);
        
        visible_ = false;
        ingameOnly = true;
        callWhenDisabled = true;
    }

    void onEvent(ImGUIRenderEvent* e) override {
        if (!Game::Core::showMenu) return;

        static EasingUtil inEase;

        static RenderGUI renderGUI;

        this->isEnabled() ? inEase.incrementPercentage(ImRenderUtil::getDeltaTime() * animationSpeed / 10) 
            : inEase.decrementPercentage(ImRenderUtil::getDeltaTime() * 2 * animationSpeed / 10);
        float inScale = ClickGUIManager::getEaseAnim(inEase, animationMode);
        if (inEase.isPercentageMax()) inScale = 1;
        animation = Math::lerp(0, 1, inEase.easeOutExpo());

        if (animation < 0.0001f) {
            return;
        }
        

        renderGUI.render(animation, inScale, scrollDirection);
    }
    

    void onEnabled() override
    {
        for (auto key : Game::Core::Keymap)
        {
            key.second == true ? CallFunc<void*, __int32, bool>(__o__SendKeyboardKey, key.first, false) : 0;
        }
    }

    void onDisabled() override
    {
        // Game::GetInstance()->refocusCursor();
        Game::GetInstance()->grabCursor();
    }

    void onEvent(MouseEvent* event) override
    {
        if (!enabled_) return;
        *event->cancelled = true;
    }

    void onEvent(KeyboardEvent* event) override
    {
        if (!enabled_) return;
        __int32 key = *event->Key;
        bool held = *event->Held;

        if (key == VK_ESCAPE && !ClickGUIManager::isBinding && held) {
            this->setEnabled(false);
            Game::GetInstance()->grabCursor();
        }

        *event->cancelled = true;
    }

    void onEvent(MouseScrollEvent* event) override
    {
        if (!enabled_) return;
        bool direction = event->MouseDirection;

        if (!direction) scrollDirection++; else scrollDirection--;
    }

private:
    // Add any additional properties or methods specific to your module here
};