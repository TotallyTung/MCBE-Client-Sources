//
// Created by vastrakai on 9/4/2023.
//

#include "TimerSlider.h"


float TimerSlider::GetSpeedAttrib(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return 1.f;

    float attrib = player->getSpeed() * 10.f;
    attrib += AttribShift.Value;
    if (attrib < 1.0f) return 1.0f;
    return attrib;
}

int TimerSlider::OffGroundTicks = 0;

void TimerSlider::OnTick() {
    auto player = Minecraft::ClientInstance->ThePlayer();
    if (!player) return;

    if (DownMotion.Enabled) {
        bool isOnGround = player->isOnGround();

        if (isOnGround) {
            OffGroundTicks = 0;
        } else {
            OffGroundTicks++;
        }

        auto stateVector = player->GetStateVectorComponent();

        if (OffGroundTicks == DownMotionTick.Value) {
            for (int i = 0; i < DownMotionPredict.Value; i++) {
                stateVector->velocity.y = player->PredictNextMotion(stateVector->velocity.y);
            }
        }
    }

    float totalSeconds = Minecraft::ClientInstance->GetTiming().timer->totalTime;

    float divisor = (abs((int)totalSeconds - totalSeconds));

    divisor = 1 - divisor;
    divisor += 1;
    divisor = abs(divisor);
    float finalSpeed = (float)SpeedSetting.Value * divisor;
    if (AdjustForSpeedAttributeSetting.Enabled) {
        finalSpeed *= GetSpeedAttrib();
    }

    if (divisor > (float)MaxDivisor.Value) {
        divisor = (float)MaxDivisor.Value;
        finalSpeed = (float)MinSpeedSetting.Value;
    };

    if(finalSpeed < (float)MinSpeedSetting.Value) finalSpeed = (float)MinSpeedSetting.Value;
    Minecraft::ClientInstance->SetTimer(finalSpeed);

    if(!Enabled) Minecraft::ClientInstance->SetTimer(20.f);
}

void TimerSlider::OnDisable() {
    Minecraft::ClientInstance->SetTimer(20.f);
}

void TimerSlider::OnEnable() {

}
