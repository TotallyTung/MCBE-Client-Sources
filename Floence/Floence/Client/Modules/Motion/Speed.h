#pragma once

class Speed : public Module
{
public:
    Speed(int keybind = Keys::X, bool enabled = false) :
        Module("Speed", "Motion", "Sped up and jump!", keybind, enabled)
    {
        addEnum("Mode", "The mode of the speed", { "Flareon", "PredictionCollect", "DesyncBoost" }, &mode);
        addSlider("Speed", "How fast you will go", &speed, 1, 10);
        //addSlider("Height", "Jump height!", &height, 0, 0.42);
        addSlider("Timer", "Increase the game timer", &timerSpeed, 0, 50);
        addBool("DamageBoost", "Boost speed when you knockback", &damageBoost);
        addSlider("BoostSpeed", "How fast you will go when you get damage", &damageBoostSpeed, 0, 5);
        addBool("Halt", "Reset playerspeed when u toggle offed this", &Halt);

        addSlider("Desync amount", "Delay to wait before resending packets (ms)", &desyncamount, 0.f, 2600);
        addSlider("Desync speed", "Increase the game timer", &desyncspeed, 0, 50);
    }

    int mode = 0;
    float speed = 3.5f;
    float height = 4.2f;
    float friction = 1.f;
    bool Halt = false;

    float desyncspeed = 20;
    float desyncamount = 500.f;
        
    bool autoswiftness = false;

    float timerSpeed = 20;

    bool fastfall = false;
    float fastfallspeed = 20;
    bool gotBoost = false;

    // DamageBoost
    bool damageBoost = true;
    float damageBoostSpeed = 1.5f;


    static inline bool shouldDoSwiftness = true;

    void onEnabled() override {
        Global::lastLerpVelocity = NULL;
    }

    void doSwiftness(Player* player) {
        PlayerInventory* playerInventory = player->getSupplies();
        Inventory* inventory = playerInventory->inventory;
        auto previousSlot = playerInventory->hotbarSlot;

        for (int n = 0; n < 36; n++) {
            ItemStack* stack = inventory->getItem(n);
            if (stack->item != nullptr) {
                std::string ItemName = stack->getItem()->name;
                if (stack->customNameContains("Spell of Swiftness") && !Global::effectSwiftness) {
                    if (previousSlot != n && !shouldDoSwiftness) {
                        playerInventory->hotbarSlot = n;
                        player->getGamemode()->baseUseItem(*stack);
                        playerInventory->hotbarSlot = previousSlot;
                        TimeUtils::resetTime("Swiftness");
                        shouldDoSwiftness = true;
                    }
                }
            }
        }
    }

    void onEvent(ActorBaseTickEvent* event) override {
        if (!Address::getClientInstance() || !Address::getLocalPlayer() || !Address::getLocalPlayer()->getStateVector() || !Address::getClientInstance()->getTimerClass() || !Address::canUseKeys())
            return;

        Player* player = Address::getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        if (getModuleByName("fly")->isEnabled()) {
            return;
        }

        auto onGround = player->isOnGround();

        bool keyPressed = player->getMoveInputHandler()->isPressed();
        //if (keyPressed) player->setSprinting(true);

        static float currentSpeed;
        static float lastSpeed;

        float yaw = (float)player->getMovementInterpolator()->Rotations.y;

        if (TimeUtils::hasTimeElapsed("Swiftness", 5000, false)) {
            shouldDoSwiftness = false;
        }

        switch (mode) {
        case 0: // Flareon
            // Damage Boost
            if (damageBoost && Global::lastLerpVelocity != NULL) {
                float boostedSpeed = Global::lastLerpVelocity.magnitudexz() * damageBoostSpeed;
                currentSpeed = boostedSpeed;
                MovementUtils::setSpeed(currentSpeed);
                Global::lastLerpVelocity = NULL;
            }
            if (keyPressed)
            {
                Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timerSpeed);
                if (onGround)
                {
                    //state->Velocity.y += 1;
                    //state->Velocity.y = 0.01 / 9.65;
                    player->jumpFromGround();
                    //state->Velocity.y = 0.42f;

                    MovementUtils::setSpeed(speed / 6.89);
                    //state->Velocity.y += 1;
                    //state->Velocity.y = height / 9.65;
                }
            }
            break;
        case 1: // Predictions
            Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timerSpeed);
            if (keyPressed)
            {
                if (onGround)
                {
                    player->jumpFromGround();
                    state->Velocity.y = 0.f;
                }
                MovementUtils::setSpeed(speed * 0.05);
            }
            break;

        case 2: //Desync Boost
            if (keyPressed)
            {
                MovementUtils::setSpeed(speed / 6.89);

                if (player->getMoveInputHandler()->mJumping) {
                    Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(desyncspeed);

                    static auto lastTime = std::chrono::high_resolution_clock::now();
                    auto currentTime = std::chrono::high_resolution_clock::now();
                    if (std::chrono::duration_cast<std::chrono::milliseconds>(currentTime - lastTime).count() < desyncamount) {
                        Global::shouldLagTicks = true;
                    }
                    else {
                        Global::shouldLagTicks = false;
                        lastTime = std::chrono::high_resolution_clock::now();
                    }

                }
                else {
                    Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(timerSpeed);
                    Global::shouldLagTicks = false;
                }

            }
            break;
        }
    };


    void onDisabled() override {
        Global::shouldLagTicks = false;

        if (!Address::getClientInstance())
            return;

        if (!Address::getClientInstance()->getTimerClass())
            return;

        Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);

        void onEvent(ActorBaseTickEvent * event)
            ; {
            auto player = Address::getLocalPlayer();
            if (player == nullptr) return;
            if (Halt) {
                player->getStateVector()->Velocity.x = 0.f;
                player->getStateVector()->Velocity.z = 0.f;
            }
        }
    }
};

