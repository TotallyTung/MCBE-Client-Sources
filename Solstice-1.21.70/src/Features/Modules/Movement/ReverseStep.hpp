#pragma once
//
// Created by alteik on 09/10/2024.
//

#include <Features/Modules/Module.hpp>

class ReverseStep : public ModuleBase<ReverseStep>
{
public:
    enum class Mode {
        Motion
    };

    EnumSettingT<Mode> mMode = EnumSettingT<Mode>("Mode", "The style of which u falling.", Mode::Motion, "Motion");
    NumberSetting mMaxFallDistance = NumberSetting("Max Fall Distance", "max fall distance to fall (made to avoid fall into the void)", 20, 0, 30, 1);
    BoolSetting mDontUseIfSpeed = BoolSetting("Dont Use If Speed", "avoid using reverse step while speed module enabled", true);
    BoolSetting mDontUseIfLongJump = BoolSetting("Dont Use If LongJump", "avoid using reverse step while long jump module enabled", true);
    BoolSetting mVoidCheck = BoolSetting("Void Check", "avoid using reverse step while u falling into the void", true);

    ReverseStep() : ModuleBase<ReverseStep>("ReverseStep", "Automatically steps down blocks", ModuleCategory::Movement, 0, false) {
        addSetting(&mMode);
        addSetting(&mMaxFallDistance);
        addSetting(&mDontUseIfSpeed);
        addSetting(&mDontUseIfLongJump);
        addSetting(&mVoidCheck);

        mNames = {
                {Lowercase, "reversestep"},
                {LowercaseSpaced, "reverse step"},
                {Normal, "ReverseStep"},
                {NormalSpaced, "Reverse Step"}
        };
    }

    bool mJumped = false;

    bool canFallDown();
    bool isVoid();

    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
    void onPacketOutEvent(class PacketOutEvent& event);
};