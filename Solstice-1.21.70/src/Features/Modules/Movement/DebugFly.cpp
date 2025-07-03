//
// Created by Tozic on 9/16/2024.
//

#include "DebugFly.hpp"
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/MinecraftSim.hpp>

void DebugFly::onEnable()
{
    gFeatureManager->mDispatcher->listen<BaseTickEvent, &DebugFly::onBaseTickEvent>(this);
    mJustEnabled = true;
}

void DebugFly::onDisable()
{
    gFeatureManager->mDispatcher->deafen<BaseTickEvent, &DebugFly::onBaseTickEvent>(this);

    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    ClientInstance::get()->getMinecraftSim()->setSimTimer(20);
    player->getStateVectorComponent()->mVelocity.x = 0;
    player->getStateVectorComponent()->mVelocity.y = 0;
    player->getStateVectorComponent()->mVelocity.z = 0;
}

void DebugFly::onBaseTickEvent(class BaseTickEvent &event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if (mJustEnabled)
    {
        if (mDebug.mValue)
        {
            ChatUtils::displayClientMessage("Enable clumsy and select all sending packets then enable lag and set it to 2000");
        }

        mJustEnabled = false;
    }

    StateVectorComponent* state = player->getStateVectorComponent();

    // Adjustment so the player won't have an invalid number
    if (mMaxSpeed.mValue < mMinSpeed.mValue) mMaxSpeed.mValue = mMinSpeed.mValue;
    if (mTimerMax.mValue < mTimerMin.mValue) mTimerMax.mValue = mTimerMin.mValue;

    float deltaSpeed = mMaxSpeed.mValue - mMinSpeed.mValue;
    float deltaTimer = mTimerMax.mValue - mTimerMin.mValue;
    float latestTimerValue = 0;
    float currentSpeed = 0;
    float nextSpeed = 0;
    float nextTimer = 0;

    switch ((int)mSpeedMode.mValue)
    {
    case 0: // Static
        currentSpeed = mSpeed.mValue / 10;
        break;
    case 1: // Decrease
        currentSpeed = mMaxSpeed.mValue - (deltaSpeed * mSpeedIndex * mSpeedMultiplier.mValue / 10);
        if (currentSpeed > mMinSpeed.mValue) mSpeedIndex++;
        else currentSpeed = mMinSpeed.mValue;
        currentSpeed /= 10;
        break;
    case 2: // Loop
        currentSpeed = mMaxSpeed.mValue - (deltaSpeed * mSpeedIndex * mSpeedMultiplier.mValue / 10);
        nextSpeed = mMaxSpeed.mValue - (deltaSpeed * (mSpeedIndex + 1) * mSpeedMultiplier.mValue / 10);
        if (nextSpeed > mMinSpeed.mValue) mSpeedIndex++;
        else {
            mSpeedIndex = 0;
            currentSpeed = mMinSpeed.mValue;
        }
        currentSpeed /= 10;
        break;
    case 3: // Wave
        if (mIsSpeedIncrease) {
            currentSpeed = mMinSpeed.mValue + (deltaSpeed * mSpeedIndex * mSpeedMultiplier.mValue / 10);
            nextSpeed = mMinSpeed.mValue + (deltaSpeed * (mSpeedIndex + 1) * mSpeedMultiplier.mValue / 10);
            if (nextSpeed < mMaxSpeed.mValue) mSpeedIndex++;
            else {
                mIsSpeedIncrease = false;
                mSpeedIndex = 1;
                currentSpeed = mMaxSpeed.mValue;
            }
        }
        else
        {
            currentSpeed = mMaxSpeed.mValue - (deltaSpeed * mSpeedIndex * mSpeedMultiplier.mValue / 10);
            nextSpeed = mMaxSpeed.mValue - (deltaSpeed * (mSpeedIndex + 1) * mSpeedMultiplier.mValue / 10);
            if (nextSpeed > mMinSpeed.mValue) mSpeedIndex++;
            else {
                mIsSpeedIncrease = true;
                mSpeedIndex = 1;
                currentSpeed = mMinSpeed.mValue;
            }
        }
        currentSpeed /= 10;
        break;
    case 4: // Test
        // I'll do this later on
        break;
    }

    switch ((int)mTimerMode.mValue)
    {
    case 0: // Static
        latestTimerValue = mTimer.mValue;
        break;
    case 1: // Decrease
        latestTimerValue = mTimerMax.mValue - (deltaTimer * mTimerIndex * mTimerMultiplier.mValue / 10);
        if (latestTimerValue > mTimerMin.mValue) mTimerIndex++;
        else latestTimerValue = mTimerMin.mValue;
        break;
    case 2: // Loop
        latestTimerValue =  mTimerMax.mValue - (deltaTimer * mTimerIndex * mTimerMultiplier.mValue / 10);
        nextTimer =  mTimerMax.mValue - (deltaTimer * (mTimerIndex + 1) * mTimerMultiplier.mValue / 10);
        if (nextTimer > mTimerMin.mValue) mTimerIndex++;
        else {
            mTimerIndex = 0;
            latestTimerValue = mTimerMin.mValue;
        }
        break;
    case 3: // Wave
        if (mIsTimerIncrease) {
            latestTimerValue = mTimerMin.mValue + (deltaTimer * mTimerIndex * mTimerMultiplier.mValue / 10);
            nextTimer = mTimerMin.mValue + (deltaTimer * (mTimerIndex + 1) * mTimerMultiplier.mValue / 10);
            if (nextTimer <  mTimerMax.mValue) mTimerIndex++;
            else {
                mIsTimerIncrease = false;
                mTimerIndex = 1;
                latestTimerValue =  mTimerMax.mValue;
            }
        }
        else
        {
            latestTimerValue =  mTimerMax.mValue - (deltaTimer * mTimerIndex * mTimerMultiplier.mValue / 10);
            nextTimer =  mTimerMax.mValue - (deltaTimer * (mTimerIndex + 1) * mTimerMultiplier.mValue / 10);
            if (nextTimer > mTimerMin.mValue) mTimerIndex++;
            else {
                mIsTimerIncrease = true;
                mTimerIndex = 1;
                latestTimerValue = mTimerMin.mValue;
            }
        }
        break;
    }

    float currentGlide = mGlide.mValue / 1000;
    if (mTestGlide.mValue) {
        currentGlide = mGlide.mValue / 1000 * (1 - latestTimerValue);
    }
    else currentGlide = mGlide.mValue / 1000;

    if (mDebug.mValue) {
        ChatUtils::displayClientMessage("Current Speed:" + std::to_string(currentSpeed * 10));
        ChatUtils::displayClientMessage("Current Timer;" + std::to_string(latestTimerValue));
        ChatUtils::displayClientMessage("Current Glide;" + std::to_string(-currentGlide));
    }

    ClientInstance::get()->getMinecraftSim()->setSimTimer(latestTimerValue);
    state->mVelocity.y = -currentGlide;

    glm::vec3 motion = glm::vec3(0, 0, 0);

    glm::vec2 calc = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, currentSpeed);
    motion.x = calc.x;
    motion.z = calc.y;

    player->getStateVectorComponent()->mVelocity.x = motion.x;
    player->getStateVectorComponent()->mVelocity.z = motion.z;

    bool isPressed = player->getMoveInputComponent()->mForward || player->getMoveInputComponent()->mBackward || player->getMoveInputComponent()->mLeft || player->getMoveInputComponent()->mRight;

    if (isPressed)
    {
        //player->setSprinting(true);
        player->getMoveInputComponent()->mIsSprinting = true;
    }
}