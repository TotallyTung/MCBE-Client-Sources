#include <chrono>
#include <thread>
#include <cstdlib> // for rand() and srand()
#include <ctime>   // for srand() seed

#pragma once
#include "../../../Util/Time/TimeUtil.h"

class Disabler : public Module
{
public:
    Disabler(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Disabler", "Misc", "Basic version of an anticheat disabler", keybind, enabled)
    {
        registerEnumSetting("Mode", "The mode of which the disabler will work", { "GroundSpoof", "Sentinel" }, &disablerMode);
        registerFloatSetting("CombatTick", "Attack speed", &ctm, 0, 50);
        IngameOnly();
        srand(time(nullptr)); // Seed srand() for random number generation
    }

    int disablerMode = 0;
    float ctm = 1;

    void onEvent(UpdateEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (player == nullptr) return;

        switch (disablerMode)
        {
        case 1: // Sentinel
            if (Game::GetInstance()->mcGame->canUseMoveKeys()) {
                // Disable player movement
                player->getMovementProxy()->setSpeed(0.0f);
                player->getMovementProxy()->setJumping(false);

                // Introduce random delays and randomize packet parameters
                for (int i = 0; i < 5; i++) {
                    // Generate random parameters
                    float randX = generateRandomFloat(-100.0f, 100.0f);
                    float randY = generateRandomFloat(-100.0f, 100.0f);
                    float randZ = generateRandomFloat(-100.0f, 100.0f);
                    float randYaw = generateRandomFloat(0.0f, 360.0f);
                    float randPitch = generateRandomFloat(-90.0f, 90.0f);

                    // Create and send fake movement packets with randomized parameters
                    MovePlayerPacket packet = MovePlayerPacket(player, player->getPosition() + Vector3(randX, randY, randZ), Vector2(randYaw, randPitch), true);
                    packet.mode == 2; // Assuming mode should be set with '=' instead of '=='
                    packet.tick == generateRandomInt(1000000, 2000000); // Random tick value
                    Game::GetInstance()->getPacketSender()->sendToServer(&packet);

                    // Introduce random delay between packet sends
                    std::this_thread::sleep_for(std::chrono::milliseconds(generateRandomInt(50, 200)));
                }
            }
            break;
        default:
            break;
        }
    }

    void onEvent(PacketSendingEvent* event) override
    {
        Packet* pkt = event->Packet;

        switch (disablerMode)
        {
        case 0: // GroundSpoof
            if (pkt->instanceOf<MovePlayerPacket>())
            {
                MovePlayerPacket* packet = reinterpret_cast<MovePlayerPacket*>(pkt);
                packet->onGround = true;
            }
            if (pkt->instanceOf<PlayerAuthInputPacket>() && pkt->instanceOf<MovePlayerPacket>())
            {
                MovePlayerPacket* packet = reinterpret_cast<MovePlayerPacket*>(pkt);
                packet->onGround = true;
                packet->mode == 1;
                packet->tick == 1;
            }
            break;
        default:
            break;
        }
    }

    void onEvent(UpdateTickEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (player == nullptr) return;

        switch (disablerMode)
        {
        default:
            break;
        }
    }

    void onDisabled() override
    {
        // Cleanup or additional actions when the module is disabled
    }

private:
    // Helper functions for generating random numbers
    float generateRandomFloat(float min, float max) {
        return min + static_cast <float> (rand()) / (static_cast <float> (RAND_MAX / (max - min)));
    }

    int generateRandomInt(int min, int max) {
        return min + (rand() % (max - min + 1));
    }
};
