#pragma once
//
// 8/30/2024.
//

class Spammer : public ModuleBase<Spammer> {
public:
    enum class Mode {
        Custom
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode of the spammer", Mode::Custom, "Custom");
    NumberSetting mDelayMs = NumberSetting("Delay", "The delay in ms to send message", 1000, 0, 5000, 50);
    BoolSetting mAvoidKicks = BoolSetting("AvoidKicks", "Avoid getting kicked for spamming", false);
    NumberSetting mAddedDelay = NumberSetting("AddedDelay", "The added delay in ms to avoid getting kicked", 0, 0, 5000, 50);

    Spammer();

    MessageTemplate mSpammerMessageTemplate = MessageTemplate("spammerMessageTemplate", "Good game!", std::make_pair("!randMention!", "Randomly mention a player, excluding yourself"));
    uint64_t mLastMessageSent = 0;

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketInEvent(class PacketInEvent& event);
};