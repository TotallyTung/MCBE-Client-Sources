#pragma once

//
// Created by alteik on 11/11/2024.
//

class FastEat : public ModuleBase<FastEat> {
public:

    NumberSetting mSpeed = NumberSetting("Speed", "speed u eating with", 1, 1, 10, 1);

    FastEat() : ModuleBase("FastEat", "allow u to customize speed u eating with", ModuleCategory::Player, 0, false)
    {
        addSetting(&mSpeed);

        mNames = {
            {Lowercase, "fasteat"},
              {LowercaseSpaced, "fast eat"},
              {Normal, "FastEat"},
              {NormalSpaced, "Fast Eat"}
        };
    };

    static inline unsigned char mOriginalData[8];

    static inline unsigned char mDetourBytes[] = {
        0x01, 0x00, 0x00, 0x00,
        0x52,
        0x8B, 0x15, 0xF5, 0xFF, 0xFF, 0xFF,
        0x39, 0xD0,
        0x0F, 0x8C, 0x07, 0x00, 0x00, 0x00,
        0x29, 0xD0,
        0xE9, 0x02, 0x00, 0x00, 0x00,
        0xFF, 0xC8,
        0x89, 0x87, 0xC8, 0x07, 0x00, 0x00,
        0x5A
    };

    static inline void* mDetour = nullptr;

    void onInit() override;
    void onEnable() override;
    void onDisable() override;
    void onBaseTickEvent(class BaseTickEvent& event);
};

