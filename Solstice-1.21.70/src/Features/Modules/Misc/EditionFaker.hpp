//
// Created by alteik on 25/09/2024.
//

#pragma once
#include <windows.h>
#include <iostream>

class EditionFaker : public ModuleBase<EditionFaker> {
public:
    enum class OS {
        Unknown,
        Android,
        iOS,
        macOS,
        FireOs,
        GearVR,
        HoloLens,
        Windows10,
        Windows,
        Dedicated,
        Orbis,
        NX
    };

    enum class InputMethod {
        Unknown,
        Mouse,
        Touch,
        GamePad,
        MotionController
    };

    EnumSettingT<OS> mOs = EnumSettingT<OS>("OS", "Operating system to spoof", OS::Windows10, "Unknown", "Android", "iOS", "macOS", "FireOs", "GearVR", "HoloLens", "Windows 10", "Windows", "Dedicated", "Orbis", "NX");
    EnumSettingT<InputMethod> mInputMethod = EnumSettingT<InputMethod>("Input Method", "Input method to spoof", InputMethod::Mouse,  "Unknown", "Mouse", "Touch", "GamePad", "MotionController");

    EditionFaker() : ModuleBase("EditionFaker", "Spoofs your operating system and input method", ModuleCategory::Misc, 0, false)
    {
        addSetting(&mOs);
        addSetting(&mInputMethod);

        mNames = {
                {Lowercase, "editionfaker"},
                {LowercaseSpaced, "edition faker"},
                {Normal, "EditionFaker"},
                {NormalSpaced, "Edition Faker"}
        };
    }

    static inline unsigned char mOriginalData[sizeof(int32_t)];

    static inline unsigned char mOriginalData1[5];
    static inline unsigned char mOriginalData2[5];

    static inline unsigned char mOriginalInputData1[8];
    static inline unsigned char mOriginalInputData2[8];

    static inline unsigned char mDetourBytes1[] = {
        0x48, 0xBF, 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // mov rdi, 1 (input)
        0x8B, 0xD7, // mov edx, edi
        0x48, 0x8B, 0xCE }; // mov rcx, rsi

    static inline unsigned char mDetourBytes2[] = {
        0x48, 0xBF, 0x01, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, // mov rdi, 1 (input)
        0x49, 0x8B, 0x07, // mov rax, [r15]
        0x8B, 0xD7 }; // mov edx, edi

    static inline void* mDetour1 = nullptr;
    static inline void* mDetour2 = nullptr;

    void injectOsSpoofer();
    void ejectOsSpoofer();
    void updateOs();
    void inject();
    void eject();
    void spoofEdition();

    void onInit() override;
    void onEnable() override;
    void onDisable() override;
    void onPacketOutEvent(class PacketOutEvent& event);
    void onConnectionRequestEvent(class ConnectionRequestEvent& event);
};