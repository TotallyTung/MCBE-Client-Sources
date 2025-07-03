#pragma once
//
// Created by vastrakai on 9/21/2024.
//


class CustomChat : public ModuleBase<CustomChat> {
public:
    NumberSetting mMaxLifeTime = NumberSetting("Life Time", "The max amount of seconds a message will be displayed for", 5, 1, 8, 1);

    CustomChat() : ModuleBase("CustomChat", "A customized, Solstice-themed chat!", ModuleCategory::Visual, 0, false) {
        mNames = {
            {Lowercase, "customchat"},
            {LowercaseSpaced, "custom chat"},
            {Normal, "CustomChat"},
            {NormalSpaced, "Custom Chat"},
        };

        addSettings(&mMaxLifeTime);
    }

    struct ChatMessage {
        std::chrono::time_point<std::chrono::system_clock> mTime;
        float mLifeTime;
        std::string mText;
        int mCount = 1;
        float mPercent;
    };

    std::vector<ChatMessage> mMessages;
    std::vector<ChatMessage> mCachedMessages;
    std::string mLastMessage;
    std::map<std::string, int> messageCount;

    void addMessage(std::string messasge);

    void onEnable() override;
    void onDisable() override;
    void onRenderEvent(class RenderEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};