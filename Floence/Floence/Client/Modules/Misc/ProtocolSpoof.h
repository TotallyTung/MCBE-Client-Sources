#pragma once

class ProtocoolSpoof : public Module
{
public:
    float delay = 500.f;

    ProtocoolSpoof(int keybind = Keys::NONE, bool enabled = false) :
        Module("JumpSpoof", "Misc", "Simply this is a ping spoofer", keybind, enabled)
    {
        addEnum("Mode", "The mode of netskip", { "Ticks" }, &mode);
        addSlider("Ms", "Delay to wait before resending packets (ms)", &delay, 0.f, 1000.f);
    }

    int mode = 0;
    bool offground = false;

    void onEvent(ActorBaseTickEvent* event) {

        Player* player = Address::getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        auto onGround = player->isOnGround();

        if (!onGround) {
            static auto lastTime = std::chrono::high_resolution_clock::now();
            auto currentTime = std::chrono::high_resolution_clock::now();
            if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() < delay) {
                Global::shouldLagTicks = true;
            }
            else {
                Global::shouldLagTicks = false;
                lastTime = std::chrono::high_resolution_clock::now();
            }
        }
        if (onGround) {
            Global::shouldLagTicks = false;
        }
    }

    void onDisabled() override {
        Global::shouldLagTicks = false;
    }
};