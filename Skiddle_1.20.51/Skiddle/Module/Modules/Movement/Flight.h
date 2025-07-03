#pragma once

class Flight : public Module
{
public:
    Flight(int keybind = Keyboard::NONE, bool enabled = false) :
        Module("Flight", "Movement", "Fly like a bird!", keybind, enabled)
    {
        registerEnumSetting("Mode", "The type of mode", { "Vanilla", "Cubecraft" }, &mode);
        registerFloatSetting("Horizontal", "The speed of hortizontal movement", &speed, 0, 30);
        registerFloatSetting("Vertical", "The speed of vertical movement", &vertical, 0, 60);
        registerFloatSetting("Timer", "Internal games timer speed", &timerSpeed, 0, 60);
        registerBoolSetting("InMotionTP", "Ported from fadeaway - Founder#2998", &forwardsTp);

        ingameOnly = true; // cant use keybind unless on the toast screen
    }

    int mode = 0;
    float speed = 2.8f;
    float vertical = 2.8f;
    float timerSpeed = 10.f;
    bool forwardsTp = false;
    int delay0;
    int delay = 20;
    int flySpeedIndex = 0;


    void onDisabled() override
    {
        if (!Game::GetInstance()->getMinecraft())
            return;

        ClientInstance* instance = Game::GetInstance();
        Minecraft* minecraft = instance->getMinecraft();

        minecraft->setTimerSpeed(20);
    }

    float CubeFlySpeedArrayThingy[15] = {
    0.910000,
    0.615559,
    0.583347,
    0.554032,
    0.527356,
    0.503081,
    0.480991,
    0.460887,
    0.442595,
    0.425948,
    0.410799,
    0.397015,
    0.384470,
    0.373055,
    0.362665 
    };


    void onEvent(UpdateEvent* event) override
    {
        if (!Game::GetLocalPlayer() || !Game::GetInstance()->getMinecraft() || !Game::GetInstance()->mcGame)
            return;

        Player* player = Game::GetLocalPlayer();
        ClientInstance* instance = Game::GetInstance();
        Minecraft* minecraft = Game::GetInstance()->getMinecraft();
        MinecraftGame* game = Game::GetInstance()->mcGame;

        // (Actor actor, Vector3<float> position, Vector2<float> rot, bool onGround)
        MovePlayerPacket lbPkt(player, player->GetPosition(), player->getMovementProxy()->getRotation(), false);
        instance->getPacketSender()->sendToServer(&lbPkt);

        minecraft->setTimerSpeed(timerSpeed);

        StateVectorComponent* stateVec = player->try_get<StateVectorComponent>();
        AABBShapeComponent* shape = player->try_get<AABBShapeComponent>();
        auto* movement = player->try_get<MovementInterpolatorComponent>();

        if (!stateVec || !shape or !movement)
            return;

        DirectPlayerMovementProxy* proxy = player->getMovementProxy();
        if (!Game::GetLocalPlayer() || !Game::GetLocalPlayer()->getMovementProxy())
            return;

        if (!Game::GetLocalPlayer() || !Game::GetLocalPlayer()->try_get<StateVectorComponent>())
            return;
        
        StateVectorComponent* state = player->try_get<StateVectorComponent>();


        Vector3<float> newVel;
        float angle = (movement->Rotations.y + 90) * 0.01745327; // () + PI / 180

        switch (mode) {
        case 0: // Vanilla
            if (Game::Core::Keymap[GameInput::Forwards] && !Game::Core::IsInChat())
            {
                newVel.x = cos(angle) * (speed / 20);
                newVel.z = sin(angle) * (speed / 20);
            }

            if (game->canUseMoveKeys())
            {
                if (Game::Core::Keymap[VK_SHIFT])
                {
                    newVel.y += -(vertical / 20);
                }

                if (Game::Core::Keymap[VK_SPACE])
                {
                    newVel.y += (vertical / 20);
                }
            }

            stateVec->r_Velocity = newVel;

            if (Game::Core::Keymap[VK_CONTROL])
            {
                stateVec->r_Velocity = stateVec->r_Velocity * 3;
            }

            if (forwardsTp == false)
            {
                stateVec->r_Velocity = newVel;

                if (Game::Core::Keymap[VK_CONTROL])
                {
                    stateVec->r_Velocity = stateVec->r_Velocity * 3;
                }
            }

            if (forwardsTp == true)
            {
                bool w = Game::Core::Keymap[GameInput::Forwards];
                bool a = Game::Core::Keymap[GameInput::Left];
                bool s = Game::Core::Keymap[GameInput::Backwards];
                bool d = Game::Core::Keymap[GameInput::Right];

                bool pressed = game->canUseMoveKeys() && (w || a || s || d);
                auto calcYaw = player->getMovementProxy()->getRotation().y;

                updateYaw(w, a, s, d, calcYaw);

                auto calcYawF = calcYaw * (3.1415927f / 180);
                if (pressed && !Game::Core::IsInChat())
                {
                    newVel.x = cos(calcYawF) * (speed / 20);
                    newVel.z = sin(calcYawF) * (speed / 20);
                }

                if (Game::Core::Keymap[VK_CONTROL])
                {
                    player->SetPos(shape->r_Pos_Lower + (newVel * 3));
                    stateVec->r_Velocity = newVel * 3;
                }
                else
                {
                    player->SetPos(shape->r_Pos_Lower + newVel);
                    stateVec->r_Velocity = newVel;
                }
            }
            break;
        case 1: // Cubecraft
            minecraft->setTimerSpeed(13.f);
            if (Game::Core::Keymap[GameInput::Forwards] && !Game::Core::IsInChat())
            {
                if (delay0 >= delay) {
                    float moveSpeed = CubeFlySpeedArrayThingy[flySpeedIndex++ % 15];

                    newVel.x = cos(angle) * moveSpeed;
                    newVel.z = sin(angle) * moveSpeed;
                    if (delay0 >= delay + 10) {
                        delay0 = 0;
                    }
                }
                else {
                    delay0++;
                }
            }

            newVel.y -= 0.20;
            stateVec->r_Velocity = newVel;
            break;
        }
    }

    void onEnabled() {
        flySpeedIndex = 0;
    }

    void updateYaw(bool w, bool a, bool s, bool d, float& calcYaw)
    {
        if (w)
        {
            if (!a && !d)
                calcYaw += 90.f;
            else if (a)
                calcYaw += 45.f;
            else if (d)
                calcYaw += 135.f;
        }
        else if (s)
        {
            if (!a && !d)
                calcYaw -= 90.f;
            else if (a)
                calcYaw -= 45.f;
            else if (d)
                calcYaw -= 135.f;
        }
        else if (!w && !s)
        {
            if (!a && d)
                calcYaw += 180.f;
        }
        if (w && s) {
            calcYaw -= 90.f;
        }
    }
};