#pragma once

#include "../../Menus/ClickGUI/FrostGUI.hpp"
#include "../../Menus/ClickGUI/ModernGUI.hpp"

class ClickGUI : public Module
{
public:
    float mAnimation = 0;
    int mScrollDirection = 0;

    ClickGUI()
        : Module("ClickGUI", "Visual", "Manage your modules.", Keys::TAB, false)
    {
        mToggleInGameOnly = false;
        mCallEventsWhenDisabled = true;
    }

    void onEvent(ImRenderEvent* e) override {
        static EasingUtil inEase;
        static FrostGUI frostGUI;
        static ModernGUI modernGUI;

        this->isEnabled() ? inEase.incrementPercentage(ImRenderUtil::getDeltaTime() * 15.5f / 10)
            : inEase.decrementPercentage(ImRenderUtil::getDeltaTime() * 2 * 15.5f / 10);

        float inScale = ClickGUIManager::getEaseAnim(inEase);

        if (inEase.isPercentageMax())
            inScale = 1;

        mAnimation = MathUtil::lerp(0, 1, inEase.easeOutExpo());

        if (mAnimation < 0.0001f) {
            return;
        }

        frostGUI.render(mAnimation, inScale, mScrollDirection);
        //modernGUI.render(mAnimation, inScale, mScrollDirection);
    }


    void onEnabled() override
    {
        InstanceManager::get<ClientInstance>()->releaseMouse();

        for (auto key : GlobalVariables::mKey)
        {
            key.second == true ? Memory::CallFunc<void*, __int32, bool>(onKeyboardDevice, key.first, false) : 0;
        }
    }

    void onDisabled() override
    {
        InstanceManager::get<ClientInstance>()->grabMouse();
    }

    void onEvent(KeyboardEvent* event) override
    {
        if (!mEnabled)
            return;

        if (*event->mKey == VK_ESCAPE && *event->mHeld) {
            this->setEnabled(false);
            *event->mCancelled = true;
            InstanceManager::get<ClientInstance>()->grabMouse();
        }
    }

    void onEvent(MouseEvent* event) override
    {
        if (!mEnabled) return;
        *event->mCancelled = true;
    }

    void onEvent(MouseScrollEvent* event) override
    {
        if (!mEnabled) return;
        bool direction = event->mMouseDirection;

        if (!direction) mScrollDirection++; else mScrollDirection--;
    }
};