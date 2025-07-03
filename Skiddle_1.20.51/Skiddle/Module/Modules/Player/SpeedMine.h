#pragma once

class SpeedMine : public Module
{
public:
    Vector3<int> blockPos;
    unsigned char side;
    int destroyDelay = -1;
    float delay = 80.f;
    SpeedMine(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("SpeedMine", "Player", "Fast destory", keybind, enabled)
    {
        registerFloatSetting("Delay", "Destory speed", &delay, 0, 100);
    }

    void onEvent(StartDestroyBlockEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (!player) return;
        auto gamemode = player->getGameMode();
        if (!gamemode) return;
        blockPos = event->blockPos;
        side = event->side;
        destroyDelay = delay;
    }

    void onEvent(UpdateTickEvent* event) {
        auto player = Game::GetLocalPlayer();
        if (!player) return;
        auto gamemode = player->getGameMode();
        if (!gamemode) return;
        if (destroyDelay > -1) {
            destroyDelay--;
            if (destroyDelay == -1) {
                gamemode->stopDestroyBlock(blockPos);
                gamemode->destroyBlock(blockPos, side, true);
            }
        }
    }

    std::string getModeName() override
    {
        std::ostringstream oss;
        oss.precision(0);
        oss << " " << std::fixed << delay;
        return oss.str();
    }
};