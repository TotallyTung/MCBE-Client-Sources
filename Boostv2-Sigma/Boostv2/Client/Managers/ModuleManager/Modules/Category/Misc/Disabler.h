#pragma once
#include "../../ModuleBase/Module.h"

class Disabler : public Module
{
private:
    enum class BypassMode : int {
        PREDICTION_AC = 0,    
        GREG = 1,             
        RPP = 2,
        COMBINED = 3         
    };

    static constexpr float POSITION_ADJUSTMENT = 0.01f; 
    static constexpr int POSITION_UPDATE_INTERVAL = 250; 
    static constexpr float VERTICAL_OFFSET = 0.2f * -6.6f; 

    void applyRaytraceBypass(PlayerAuthInputPacket* packet);
    void handleGregBypass(PlayerAuthInputPacket* packet);
    void handleRppBypass(PlayerAuthInputPacket* packet, LocalPlayer* player);
    void handlePredictionRppBypass(PlayerAuthInputPacket* packet, LocalPlayer* player);
    void handlePredictionBypass(PlayerAuthInputPacket* packet, LocalPlayer* player);

public:
   
    Disabler();
    void onSendPacket(Packet* packet) override;
    std::string getModeText() override;
    int Mode = 0;  
    bool raytracebypass = false;

    Disabler(const Disabler&) = delete;
    Disabler& operator=(const Disabler&) = delete;
    Disabler(Disabler&&) = delete;
    Disabler& operator=(Disabler&&) = delete;
};