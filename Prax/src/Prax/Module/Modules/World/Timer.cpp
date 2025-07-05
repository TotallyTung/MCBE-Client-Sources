#pragma once

#include <Prax/Utils/Render/D2D.h>
#include <Prax/SDK/Game/Network/Network.h>
#include "Timer.h"


void Timer::OnEnable() {
}

void Timer::OnTick() {
    if (!Minecraft::ClientInstance)
        return;
    Minecraft::ClientInstance->SetTimer(TimerSetting.Value);
}

void Timer::OnDisable() {
    Minecraft::ClientInstance->SetTimer(20.f);
}


