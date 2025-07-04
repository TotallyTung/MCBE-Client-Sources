#include "HealthCheck.h"

HealthCheck::HealthCheck() : IModule(0x0, Category::PLAYER, "checks your health") {
    this->registerIntSetting("Health Slider", &this->HealthSlider, this->HealthSlider, 0, 20);
}

HealthCheck::~HealthCheck() {
}

const char* HealthCheck::getModuleName() {
    return ("HealthCheck");
}

bool sent = false;

void HealthCheck::onTick(C_GameMode* gm) {
    int healthy = g_Data.getLocalPlayer()->getHealth();

    if (moduleMgr->getModule<HealthCheck>()->isEnabled()) {
        // ?????????????????
        if (healthy < HealthSlider && !sent) {
            g_Data.getClientInstance()->getGuiData()->displayClientMessageF("[%sAuraV4%s] %sLow Health!!!", BLUE, RED, RED);
            sent = true;  // ???????????
        }
        // ?????????????????
        else if (healthy >= HealthSlider && sent) {
            sent = false;  // ?????????
        }
    }
}
