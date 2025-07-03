#pragma once

class Speed : public Module
{
public:
    Speed(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Speed", "Movement", "Move faster", keybind, enabled)
    {
        registerEnumSetting("Mode", "The mode of speed niga", { "Vanilla", "Friction", "Hive", "FlareonBoost" }, &mode);
        registerFloatSetting("Speed", "How fast you will go", &speed, 0, 2);
        registerFloatSetting("Friction", "How fast you will slowdown off ground", &friction, 0.1, 5);
        registerFloatSetting("Height", "How high you will jump", &height, 0, 0.42);
        registerFloatSetting("Timer", "Increase the game timer", &timerSpeed, 0.f, 60.f);
        registerFloatSetting("Boost Timer", "Boost timer", &boostTimer, 0.f, 60.f);
        registerFloatSetting("Speed Min", "unko", &speedMin, 0, 1.f);
        registerFloatSetting("Speed Max", "unko", &speedMax, 0, 1.f);

        ingameOnly = true; // cant use keybind unless on the toast screen
    }

    float timerSpeed = 20;
    float boostTimer = 25;
    float speedMax = 0.59f;
    float speedMin = 0.49f;
    float speedFriction;

    float speed = 0.5f;
    float height = 0.42f;
    float friction = 2.50f;

    int mode = 0;

    void onEvent(PacketSendingEvent* event) override
    {
        auto player = Game::GetLocalPlayer();
        if (!player || !event->Packet) {
            return;
        }

        if (event->Packet->instanceOf<PlayerAuthInputPacket>()) {
            PlayerAuthInputPacket* p = reinterpret_cast<PlayerAuthInputPacket*>(event->Packet);
            p->setFlag(InputFlag::StopSprinting, false);
            p->setFlag(InputFlag::StartSprinting, true);
            p->setFlag(InputFlag::SprintDown, true);
            p->setFlag(InputFlag::Sprinting, true);

            if (player->isOnGround()) {
                p->setFlag(InputFlag::StartJumping, true);
            }
            else {
                p->setFlag(InputFlag::JumpDown, true);
                p->setFlag(InputFlag::Jumping, true);
            }
        }
    }

    inline float RandomFloat(float a, float b) {
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<float> dis(a, b);
        return dis(gen);
    }
    /*

    inline float RandomFloat(float a, float b) {
        float random = ((float)rand()) / (float)RAND_MAX;
        float diff = b - a;
        float r = random * diff;
        return a + r;
    }*/

    void onEvent(UpdateTickEvent* event) override
    {
        if (Game::Core::IsInChat())
            return;

        if (getModuleByName("flight")->isEnabled()) {
            return;
        }

        auto context = Game::GetInstance();
        if (!context) {
            return;
        }

        auto player = Game::GetLocalPlayer();
        if (!player) {
            return;
        }

        player->setSprinting(true);

        auto stateVec = player->try_get<StateVectorComponent>();
        if (!stateVec)
            return;

        auto
            w = Game::Core::Keymap[GameInput::Forwards],
            a = Game::Core::Keymap[GameInput::Left],
            s = Game::Core::Keymap[GameInput::Backwards],
            d = Game::Core::Keymap[GameInput::Right];

        bool keyPressed = context->mcGame->canUseMoveKeys() && (w || a || s || d);

        auto onGround = Game::GetLocalPlayer()->getMovementProxy()->isOnGround();

        if (mode == 3) {
            auto space = Game::Core::Keymap[GameInput::Jump];

            auto player = Game::GetLocalPlayer();
            if (!player)
                return;

            auto velocity = player->try_get<StateVectorComponent>();
            if (!velocity) {
                return;
            }

            if (keyPressed) {
                if (space) {
                    if (onGround) {
                        float calcYaw = (player->getMovementProxy()->getRotation().y + 90) * (PI / 180);
                        float c = cos(calcYaw);
                        float s = sin(calcYaw);

                        Vector2<float> moveVec = player->getMoveVec().normalize();
                        moveVec = { moveVec.x * c - moveVec.y * s, moveVec.x * s + moveVec.y * c };

                        stateVec->r_Velocity.x = moveVec.x * RandomFloat(speedMin - 0.05, speedMax - 0.05);
                        stateVec->r_Velocity.y = height;
                        stateVec->r_Velocity.z = moveVec.y * RandomFloat(speedMin - 0.05, speedMax - 0.05);
                        player->jumpFromDaGround();
                    }
                    Game::GetInstance()->getMinecraft()->setTimerSpeed(boostTimer);
                }
                else {
                    if (onGround) {
                        float calcYaw = (player->getMovementProxy()->getRotation().y + 90) * (PI / 180);
                        float c = cos(calcYaw);
                        float s = sin(calcYaw);

                        Vector2<float> moveVec = player->getMoveVec().normalize();
                        moveVec = { moveVec.x * c - moveVec.y * s, moveVec.x * s + moveVec.y * c };

                        stateVec->r_Velocity.x = moveVec.x * RandomFloat(speedMin, speedMax);
                        stateVec->r_Velocity.y = height;
                        stateVec->r_Velocity.z = moveVec.y * RandomFloat(speedMin, speedMax);
                        player->jumpFromDaGround();
                    }
                    Game::GetInstance()->getMinecraft()->setTimerSpeed(timerSpeed);
                }
            }
        }
        else {
            Game::GetInstance()->getMinecraft()->setTimerSpeed(timerSpeed);
        }


        switch (mode)
        {
        case 0:
        {
            // Are we holding movement keys?
            if (keyPressed)
            {
                // Should we jump?
                if (height > 0 && onGround)
                {
                    player->jumpFromDaGround();
                    stateVec->r_Velocity.y = height;
                }
                MoveUtil::setSpeed(speed);
            }
            break;
        }
        case 1:
        {
            // Are we holding movement keys?
            if (keyPressed)
            {
                // Should we jump?
                if (height > 0 && onGround)
                {
                    player->jumpFromDaGround();
                    stateVec->r_Velocity.y = height;
                }
                MoveUtil::setSpeed(speed);
            }
            break;
        }
        case 2: // Hive
        {
            static float currentSpeed = speed;

            // Are we holding movement keys?
            if (keyPressed)
            {
                // Should we jump?
                if (height > 0 && onGround)
                {
                    player->jumpFromDaGround();
                    stateVec->r_Velocity.y = 0.400;

                    // Set our speed onGround
                    currentSpeed = 0.4412150828799868;
                }
                MoveUtil::setSpeed(currentSpeed);
            }
            // Apply air friction
            float frictionFactor = friction; // Adjust this value to control the rate of speed reduction
            currentSpeed *= (1 - frictionFactor * RenderUtil::getDeltaTime());
            break;
        }
        }
    }

    void onDisabled() override
    {
        Game::GetInstance()->getMinecraft()->setTimerSpeed(20.f);
    }
};