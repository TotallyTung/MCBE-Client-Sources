#pragma once
//
// Created by vastrakai on 10/7/2023.
//


#include <Prax/Module/Module.h>
#include <Prax/Module/Settings/NumberSetting.h>

class AutoSpellbook : public ModuleBase<AutoSpellbook> {
public:
    NumberSetting Delay = NumberSetting("Delay", "The delay between using spellbooks (in milliseconds)", 0, 1000, 200, 1);
    BoolSetting ShowNotification = BoolSetting("Show Notification", "Shows a notification when a spellbook is used", true);

    AutoSpellbook() : ModuleBase("AutoSpellbook", "Automatically opens your spellbook", "Player", 0, false) {
        AddSetting(&Delay);
        AddSetting(&ShowNotification);
        Delay.Display = true;
    }

    static uint64_t LastItemUsed;

    void OnTick() override;


};