#pragma once

class Timer : public Module
{
public:
    Timer() :
        Module("Timer", "Misc", "Changes the game tick speed.")
    {

    }
private:
    float mTimer = 50.f;

public:
    void onEvent(BaseTickEvent* event) override
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;

        InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(mTimer);
    }
};