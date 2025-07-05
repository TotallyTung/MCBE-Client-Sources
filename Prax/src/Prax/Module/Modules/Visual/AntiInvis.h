#pragma once
//
// Created by vastrakai on 9/25/2023.
//


#include <Prax/Module/Module.h>

class AntiInvis : public ModuleBase<AntiInvis> {
public:
    AntiInvis() : ModuleBase("AntiInvis", "Makes invisible players visible", "Visual", 0, false) {}
};