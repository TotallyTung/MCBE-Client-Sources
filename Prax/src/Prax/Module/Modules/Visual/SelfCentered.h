#pragma once
//
// Created by vastrakai on 9/18/2023.
//


#include <Prax/Module/Module.h>

class SelfCentered : public ModuleBase<SelfCentered> {
public:
    SelfCentered() : ModuleBase("SelfCentered", "For people with huge egos.", "Visual", 0, false) {
    }

};