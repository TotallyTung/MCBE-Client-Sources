#pragma once
//
// Created by vastrakai on 6/29/2024.
//

#include <Features/Modules/Module.hpp>

#include "HudEditor.hpp"

class Watermark : public ModuleBase<Watermark> {
public:
    enum class Style {
        Solstice,
        SevenDays
    };
    EnumSettingT<Style> mStyle = EnumSettingT<Style>("Style", "The style of the watermark.", Style::Solstice, "Solstice", "7 Days");
    BoolSetting mGlow = BoolSetting("Glow", "Enables glow", true);
    BoolSetting mBold = BoolSetting("Bold", "Enables bold text", true);
    Watermark() : ModuleBase("Watermark", "Displays a watermark on the screen", ModuleCategory::Visual, 0, true) {
        addSetting(&mStyle);
        addSetting(&mGlow);
        //addSetting(&mBold);
        gFeatureManager->mDispatcher->listen<RenderEvent, &Watermark::onRenderEvent>(this);

        mNames = {
            {Lowercase, "watermark"},
            {LowercaseSpaced, "watermark"},
            {Normal, "Watermark"},
            {NormalSpaced, "Watermark"}
        };

        mElement = std::make_unique<HudElement>();
        mElement->mPos = { 20, 20 };

        const char* ModuleBaseType = ModuleBase<Watermark>::getTypeID();;
        mElement->mParentTypeIdentifier = const_cast<char*>(ModuleBaseType);
        HudEditor::gInstance->registerElement(mElement.get());
    }

    std::unique_ptr<HudElement> mElement;

    void onEnable() override;
    void onDisable() override;

    void onRenderEvent(class RenderEvent& event);

    std::string getSettingDisplay() override {
        return mStyle.mValues[mStyle.as<int>()];
    }
};
