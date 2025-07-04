#pragma once
#include "../../ModuleBase/Module.h"

class Disabler : public Module {
private:
    int Mode = 0;
public:
    Disabler();
    virtual void onSendPacket(Packet* packet) override;
    virtual std::string getModeText() override;
};
