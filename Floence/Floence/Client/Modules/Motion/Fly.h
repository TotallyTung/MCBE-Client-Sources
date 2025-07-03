#pragma once

class Fly : public Module
{
public:
    Fly(int keybind = 70, bool enabled = false) :
        Module("Fly", "Motion", "Fly like a superhero.", keybind, enabled)
    {
        addEnum("Mode", "The type of fly", { "Vanilla", "Flareon", "Predictions" }, &mode);
        addSlider("Speed", "the delay between placing with ticks.", &speed, 0.3, 10);
        addSlider("Timer Value", "How fast you will slowdown while boosting", &timer, 1, 40);
        addBool("Halt", "Reset speed on disabled", &halt);
    }
private:
    int mode = 0;
    float speed = 1.f;
    float friction = 2.3f;
    float glide = 0.f;

    bool halt = false;

    float timer = 20;
    float value = 0.55;

    int jumpY = 0;
    float StartPosition = 0;

    Vector3<float> oldPosition;

    Vector3<float> desyncPosition;
    Vector3<float> desyncRotation;

    bool shouldResetFly = false;
public:

    void onEnabled() override {
        if (!Address::getLocalPlayer()) return;

        StartPosition = Address::getLocalPlayer()->getRenderPosition().y;

        oldPosition = Address::getLocalPlayer()->getRenderPosition();

        desyncPosition = Address::getLocalPlayer()->getRenderPosition();
        desyncRotation.x = Address::getLocalPlayer()->getComponent<ActorRotationComponent>()->rotation.x; // Pitch
        desyncRotation.y = Address::getLocalPlayer()->getComponent<MobBodyRotationComponent>()->bodyRot; // Body
        desyncRotation.z = Address::getLocalPlayer()->getComponent<ActorHeadRotationComponent>()->rotation.x; // Head


        switch (mode) {
        case 0: // Vanilla
            break;
        case 1:
            shouldResetFly = false;
            break;
        }

        if (mode == 1) {
            for (int x = -2; x <= 2; x++) {
                for (int z = -2; z <= 2; z++) {
                    for (int y = -2; y <= 2; y++) {
                        jumpY = Address::getLocalPlayer()->getStateVector()->Position.y;
                    }
                }
            }
        }
    }

    void onEvent(ActorBaseTickEvent* event) override {
        if (!Address::getClientInstance())
            return;

        if (!Address::getClientInstance()->getTimerClass())
            return;

        if (!Address::getLocalPlayer() || !Address::getLocalPlayer()->getStateVector())
            return;

        Player* player = Address::getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        if (!shouldResetFly) {
            desyncPosition = player->getRenderPosition();
            desyncRotation.x = player->getComponent<ActorRotationComponent>()->rotation.x; // Pitch
            desyncRotation.y = player->getComponent<MobBodyRotationComponent>()->bodyRot; // Body
            desyncRotation.z = player->getComponent<ActorHeadRotationComponent>()->rotation.x; // Head
        }

        bool keyPressed = player->getMoveInputHandler()->isPressed();

        float yaw = (float)player->getMovementInterpolator()->Rotations.y;

        switch (mode) {
        case 0: // Vanilla
            Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timer);
            MovementUtils::setSpeed(speed);
            state->Velocity.y = glide / 10;
            if (player->getMoveInputHandler()->mJumping) {
                player->getStateVector()->Velocity.y += speed;
            }
            else if (player->getMoveInputHandler()->mSneaking) {
                player->getStateVector()->Velocity.y -= speed;
            }

            break;
        case 1: // Flareon
            if (keyPressed)
                player->setSprinting(true);

            static float currentSpeed;
            static float lastSpeed;

            Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timer);

            if (keyPressed) {
                if (state->Position.y < jumpY) {
                    if (TimeUtils::hasTimeElapsed("FlyJumpDelay", 100, true)) {
                        currentSpeed = 0.97f;
                        lastSpeed = 0.97f;
                        player->jumpFromGround();
                    }
                }
                else if (!player->isOnGround()) {
                    Vector3<float> velocity = state->Velocity;
                    if (abs(velocity.magnitudexz()) > lastSpeed) {
                        currentSpeed = velocity.magnitudexz();
                    }
                    lastSpeed = velocity.magnitudexz();
                    currentSpeed = currentSpeed * 0.9f;
                    MovementUtils::setSpeed(currentSpeed);
                }
            }
            break;
        case 3: // Predictions
            Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timer);
            player->jumpFromGround();
            player->setIsOnGround(true);
            state->Velocity.y = 0.f;

            Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timer);
            //MovementUtils::setSpeed(speed / 6.89);
            break;
        }

        /*if (mode == 0) {
            state->Velocity.y = 0;

            if (player->getMoveInputHandler()->forward) {
                state->Velocity.z = sin((yaw + 90) * 0.01745329251f) * speed; // 0.01745329251f
                state->Velocity.x = cos((yaw + 90) * 0.01745329251f) * speed;
            }
            if (player->getMoveInputHandler()->left) {
                state->Velocity.z = sin((yaw + 0) * 0.01745329251f) * speed;
                state->Velocity.x = cos((yaw + 0) * 0.01745329251f) * speed;
            }
            if (player->getMoveInputHandler()->backward) {
                state->Velocity.z = sin((yaw - 90) * 0.01745329251f) * speed;
                state->Velocity.x = cos((yaw - 90) * 0.01745329251f) * speed;
            }
            if (player->getMoveInputHandler()->right) {
                state->Velocity.z = sin((yaw + 180) * 0.01745329251f) * speed;
                state->Velocity.x = cos((yaw + 180) * 0.01745329251f) * speed;
            }

            if (player->getMoveInputHandler()->jumping) {
                player->getStateVector()->Velocity.y += 0.3;
            }
            else if (player->getMoveInputHandler()->sneaking) {
                player->getStateVector()->Velocity.y -= 0.3;
            }
        }*/
    }

    void onEvent(PacketEvent* event) override
    {
        PlayerAuthInputPacket* packet = nullptr;

        if (event->Packet->getId() == PacketID::PlayerAuthInput) {
            packet = (PlayerAuthInputPacket*)event->Packet;
        }

        if (mode == 1) {
            if (packet != nullptr) {
                if (shouldResetFly) {
                    packet->position = desyncPosition;

                    packet->rotation.x = desyncRotation.x;
                    packet->rotation.y = desyncRotation.y;
                    packet->headYaw = desyncRotation.z;

                    Global::testPosition = desyncPosition;
                }
            }
        }
    }

    void onDisabled() override {
        if (halt) {
            Player* player = Address::getLocalPlayer();
            StateVectorComponent* state = player->getStateVector();
            state->Velocity.z = 0;
            state->Velocity.x = 0;
            if (player->getMoveInputHandler()->mJumping && mode == 0) {
                state->Velocity.y = 0;
            }
        }

        if (!Address::getClientInstance())
            return;

        if (!Address::getClientInstance()->getTimerClass())
            return;

        Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
        StartPosition = 0;
    }
};
