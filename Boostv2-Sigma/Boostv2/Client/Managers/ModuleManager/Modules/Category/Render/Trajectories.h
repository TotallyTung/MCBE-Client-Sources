#pragma once
#include "../../ModuleBase/Module.h"

class Trajectories : public Module {
public:

    Trajectories();
    void onLevelRender() override;
};