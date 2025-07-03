#pragma once
//
// Created by vastrakai on 7/4/2024.
//

#include <Features/FeatureManager.hpp>
#include <Features/Events/NotifyEvent.hpp>


class Notifications : public ModuleBase<Notifications> {
public:
    enum class Style {
        Solaris,
    };
    EnumSettingT<Style> mStyle = EnumSettingT<Style>("Style", "The style of the notifications", Style::Solaris, "Solaris");
    BoolSetting mShowOnToggle = BoolSetting("Show on toggle", "Show a notification when a module is toggled", true);
    BoolSetting mShowOnJoin = BoolSetting("Show on join", "Show a notification when you join a server", true);
    BoolSetting mColorGradient = BoolSetting("Color gradient", "Enable a color gradient on the notifications", false);
    BoolSetting mLimitNotifications = BoolSetting("Limit notifications", "Limit the number of notifications shown at one time", false);
    NumberSetting mMaxNotifications = NumberSetting("Max notifications", "The maximum number of notifications shown at one time", 6, 1, 25, 1);


    Notifications() : ModuleBase("Notifications", "Shows notifications on module toggle and other events", ModuleCategory::Visual, 0, true) {
        addSetting(&mStyle);
        addSetting(&mShowOnToggle);
        addSetting(&mShowOnJoin);
        //addSetting(&mColorGradient);
        addSetting(&mLimitNotifications);
        addSetting(&mMaxNotifications);
        VISIBILITY_CONDITION(mMaxNotifications, mLimitNotifications.mValue == true);

        mNames = {
            {Lowercase, "notifications"},
            {LowercaseSpaced, "notifications"},
            {Normal, "Notifications"},
            {NormalSpaced, "Notifications"}
        };

        gFeatureManager->mDispatcher->listen<RenderEvent, &Notifications::onRenderEvent, nes::event_priority::VERY_LAST>(this);
    }

    std::vector<Notification> mNotifications;

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onModuleStateChange(ModuleStateChangeEvent& event);
    void onModuleScriptStateChange(ModuleScriptStateChangeEvent& event);
    void onConnectionRequestEvent(class ConnectionRequestEvent& event);
    void onNotifyEvent(class NotifyEvent& event);

    std::string getSettingDisplay() override {
        return mStyle.mValues[mStyle.as<int>()];
    }
};
