#include "Timer.h"
#include "../Exploit/AntiDesync.h"
#include "../../../Client.h"
Timer::Timer() : Module("Timer", "Changes your tps(tick per seconds).", Category::MISC) {
	slider<int>("TPS", "NULL", ValueType::INT_T, &tps, 1, 150);
}

void Timer::onNormalTick(Actor* actor) {
    static AntiDesync* AntiDesyncMod = (AntiDesync*)client->moduleMgr->getModule("AntiDesync");
    if (AntiDesyncMod->isEnabled() && mc.getLocalPlayer()->getItemUseDuration() > 0) mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
    if (AntiDesyncMod->isEnabled() && mc.getLocalPlayer()->getItemUseDuration() > 0) return;
    if (mc.getLocalPlayer() == nullptr) return;
    mc.getClientInstance()->minecraft->mctimer->tps = (float)tps;
}


void Timer::onDisable() {
	mc.getClientInstance()->minecraft->mctimer->tps = 20.f;
}
