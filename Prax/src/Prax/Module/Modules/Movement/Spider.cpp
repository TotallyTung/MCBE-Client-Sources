//
// Created by vastrakai on 8/23/2023.
//

#include "Spider.h"

#include <Prax/Module/Modules/Misc/NetSkip.h>

#include "Prax/Hook/Hooks/RakPeerHooks.h"

float posY = 0.f;
bool wasCollided = false;

void setMotion(float yVel)
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto state = player->GetStateVectorComponent();
    state->velocity.y = yVel;
}

Vector3 getPosition()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    return *player->getPos();
}

bool forward()
{
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto moveInput = player->GetMoveInputComponent();
    return moveInput->forward;

}

void addToY(float value) {
    auto player = Minecraft::ClientInstance->ThePlayer();
    player->GetAABBShapeComponent()->AAY += value;
    player->GetAABBShapeComponent()->BBY += value;
}

void Spider::OnTick(){
    auto player = Minecraft::ClientInstance->ThePlayer();
    auto state = player->GetStateVectorComponent();

    if (Mode == Vanilla)
    {
        if (player->isCollidingHorizontal() && !player->isOnGround())
        {
            state->velocity.y = SpeedSetting.Value / 10;
        }
    }
    else if (Mode == Flareon)
    {
        if (!player->isCollidingHorizontal() || !forward()) {
            posY = 0.f;
            if (!player->isCollidingHorizontal() && wasCollided) {
                wasCollided = false;
                setMotion(0.05f);

            }
            return;
        }

        wasCollided = true;

        if (posY == 0.f) {
            posY = getPosition().y;
            setMotion(0.f);
        }
        auto dist = getPosition().y - posY;
        if (dist < 1.3f) {
            setMotion(SpeedSetting.Value);
        }
        else {
            setMotion(-(SpeedSetting.Value / 10.f));
            auto pos = getPosition();
            pos.y -= 0.06f;
            posY = pos.y;
        }
    } else if (Mode == Clip)
    {
        if (player->isCollidingHorizontal() && forward())
        {
            // Get the block the player is colliding with
            static bool flippingBool = false;
            flippingBool = !flippingBool;

            float sped = SpeedSetting.Value / 10.f;

            // teleport up
            Vector3 pos = *player->getPos();
            pos.y = pos.y - 1.62f;

            setMotion(0.f);

            /*if (flippingBool) {
                // Teleport down by half the speed
                return;
            }*/

            // Teleport up
            addToY(sped);
        }
    }

}

void Spider::OnDisable()
{
    if (wasCollided) {
        setMotion(0.05f);
    }
}
