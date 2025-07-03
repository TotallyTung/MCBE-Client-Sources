#pragma once
//
// Created by alteik on 03/09/2024.
//
#include <Features/Modules/Module.hpp>


class AutoDodge : public ModuleBase<AutoDodge> {
public:

    BoolSetting mBalance = BoolSetting("Balance", "Dodge map Balance", false);
    BoolSetting mBaroque = BoolSetting("Baroque", "Dodge map Baroque", false);
    BoolSetting mEuropa = BoolSetting("Europa", "Dodge map Europa", false);
    BoolSetting mGrove = BoolSetting("Grove", "Dodge map Grove", false);
    BoolSetting mIvory = BoolSetting("Ivory", "Dodge map Ivory", false);
    BoolSetting mLush = BoolSetting("Lush", "Dodge map Lush", false);
    BoolSetting mMonastery = BoolSetting("Monastery", "Dodge map Monastery", false);
    BoolSetting mPillars = BoolSetting("Pillars", "Dodge map Pillaris", false);
    BoolSetting mPineconePoint = BoolSetting("Pinecone Point", "Dodge map Pinecone Point", false);
    BoolSetting mTurmoil = BoolSetting("Turmoil", "Dodge map Turmoil", false);
    BoolSetting mVillage = BoolSetting("Village", "Dodge map Village", false);
    BoolSetting mVioletKeep = BoolSetting("VioletKeep", "Dodge map Violet Keep", false);
    BoolSetting mVolcano = BoolSetting("Volcano", "Dodge map Volcano", false);
    BoolSetting mWildWood = BoolSetting("WildWood", "Dodge map Wild Wood", false);
    BoolSetting mWoodpine = BoolSetting("Wood pine", "Dodge map Woodpine", false);

    AutoDodge() : ModuleBase("AutoDodge", "Automatically dodges selected maps on the hive", ModuleCategory::Misc, 0, false){
        addSettings(
                &mBalance,
                &mBaroque,
                &mEuropa,
                &mGrove,
                &mIvory,
                &mLush,
                &mMonastery,
                &mPillars,
                &mPineconePoint,
                &mTurmoil,
                &mVillage,
                &mVioletKeep,
                &mVolcano,
                &mWildWood,
                &mWoodpine
        );

        mNames = {
                {NamingStyle::Lowercase, "autododge"},
                {NamingStyle::LowercaseSpaced, "auto dodge"},
                {NamingStyle::Normal, "AutoDodge"},
                {NamingStyle::NormalSpaced, "Auto Dodge"}
        };
    }

    void onEnable() override;
    void onDisable() override;
    void onPacketInEvent(class PacketInEvent& event);
};
