//
// Created by vastrakai on 6/25/2024.
//

#include "MinecraftSim.hpp"

#include <libhat/Access.hpp>
#include <SDK/OffsetProvider.hpp>

Simulation* MinecraftSim::getGameSim()
{
    return hat::member_at<Simulation*>(this, OffsetProvider::MinecraftSim_mGameSim);
}

Simulation* MinecraftSim::getRenderSim()
{
    return hat::member_at<Simulation*>(this, OffsetProvider::MinecraftSim_mRenderSim);
}

void MinecraftSim::setSimTimer(float timer)
{
    getGameSim()->mTimer = timer;
    getRenderSim()->mTimer = timer;
}

void MinecraftSim::setSimSpeed(float speed)
{
    getGameSim()->mTimerMultiplier = speed;
    getRenderSim()->mTimerMultiplier = speed;
}

float MinecraftSim::getSimTimer()
{
    return getGameSim()->mTimer;
}

float MinecraftSim::getSimSpeed()
{
    return getGameSim()->mTimerMultiplier;
}

GameSession* MinecraftSim::getGameSession()
{
    return hat::member_at<GameSession*>(this, OffsetProvider::MinecraftSim_mGameSession);
}
