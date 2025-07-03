#pragma once
//
// 3/3/2025.
//

#include <Features/Modules/Module.hpp>

class AutoEat : public ModuleBase<AutoEat> {
public:
    enum class Mode {
        Animation,
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The mode to use", Mode::Animation, "Animation");
    BoolSetting mHotbarOnly = BoolSetting{ "Hotbar Only", "Only switch to items in the hotbar", true };
    BoolSetting mIgnoreBoomBox = BoolSetting{ "Ignore Boom Box", "Stop eating while holding boom box", true };
    AutoEat() : ModuleBase("AutoEat", "Automatically eats items", ModuleCategory::Player, 0, false)
    {
        addSetting(&mMode);
        addSetting(&mHotbarOnly);
        addSetting(&mIgnoreBoomBox);

        mNames = {
            {Lowercase, "autoeat"},
              {LowercaseSpaced, "auto eat"},
              {Normal, "AutoEat"},
              {NormalSpaced, "Auto Eat"}
        };
    };

    uint64_t mLastEat = 0;
    bool mShouldEat = false;

    void onEnable();
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};