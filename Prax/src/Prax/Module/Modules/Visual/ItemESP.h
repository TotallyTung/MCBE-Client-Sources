//
// Created by vastrakai on 4/17/2024.
//


#pragma once

#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/ModeSetting.h>
#include <Prax/Module/Settings/NumberSetting.h>

class ItemESP : public ModuleBase<ItemESP> {
public:
    ModeSetting Mode = ModeSetting("Mode", "The mode to use", {"Outline"}, 0);
    BoolSetting DistanceLimited = BoolSetting("Distance Limited", "Only render items within a certain distance", true);
    NumberSetting Distance = NumberSetting("Distance", "The distance to render items within", 1, 64, 10, 1);
    NumberSetting FontSize = NumberSetting("Font Size", "The size of the font", 1, 40, 20, 1);
    BoolSetting HighlightBetterItems = BoolSetting("Highlight Better Items", "Highlights items that are better than what you have", true);

    ItemESP() : ModuleBase("ItemESP", "Draws a box around items on the ground", "Visual", 0, false) {
        AddSetting(&Mode);
        AddSetting(&DistanceLimited);
        AddSetting(&Distance);
        AddSetting(&FontSize);
        AddSetting(&HighlightBetterItems);
        Mode.Display = true;
    };

    static int BestHelmetValue;
    static int BestChestplateValue;
    static int BestLeggingsValue;
    static int BestBootsValue;
    static int BestSwordValue;
    static int BestPickaxeValue;
    static int BestAxeValue;
    static int BestShovelValue;

    void OnRender() override;
    void OnTick() override;
    bool ShouldHighlightItem(ItemStack *stack);

    void DoRender(Actor *actor);
};