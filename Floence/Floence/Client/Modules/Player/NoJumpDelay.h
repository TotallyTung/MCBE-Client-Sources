#pragma once

class NoJumpDelay : public Module
{
public:
    NoJumpDelay(int keybind = Keys::X, bool enabled = false) :
        Module("NoJumpDelay", "Player", "Sped up and jump!", keybind, enabled)
    {
        addEnum("Mode", "The mode of NoJumpDelay", { "Normal" }, &mode);
        //addSlider("JumpDelay", "just jumpdelay", &JumpDelay, 0, 10000);
        addBool("JumpBoost(cant bypass)", "Nojumpdelay boost", &JumpFromGroundNotSafeUDHalalNigger);
        addBool("Enableboost", "Boost or no yuh", &EnableAhhjumpboostAhhfuck);
        addSlider("BoostTest", "Boost speed when u jumping(cant bypass flareon)", &AhhBoostSpeedHalal, 0, 3.5);
        //JumpFromGroundNotSafeUDHalalNigger
    }
    int mode = 0;
    float height = 4.2f;
    //float JumpDelay = 50;
    bool EnableAhhjumpboostAhhfuck = false;
    bool JumpFromGroundNotSafeUDHalalNigger = false;
    float AhhBoostSpeedHalal = 3.5f;

    void onEnabled() override {
        Global::lastLerpVelocity = NULL;
    }

    void onEvent(ActorBaseTickEvent* event) override {
        if (!Address::getClientInstance() || !Address::getLocalPlayer() || !Address::getLocalPlayer()->getStateVector() || !Address::getClientInstance()->getTimerClass() || !Address::canUseKeys())
            return;

        Player* player = Address::getLocalPlayer();
        StateVectorComponent* state = player->getStateVector();

        auto onGround = player->isOnGround();

        // if (player->getMoveInputHandler()->jumping) return;

        bool keyPressed = player->getMoveInputHandler()->isPressed();
        if (keyPressed) player->setSprinting(true);

        static float currentSpeed;
        static float lastSpeed;

        float yaw = (float)player->getMovementInterpolator()->Rotations.y;

        switch (mode) {
        case 0:
            // Are we holding movement keys?
            bool hm = Global::Keymap[VK_SPACE];
            if (hm) {
                // Should we jump?
                if (onGround) {
                    if (TimeUtils::hasTimeElapsed/*("jumpDelay", -999999999999999, true)*/) {
                        state->Velocity.y += 1;
                        state->Velocity.y = height / 9.65;
                        if (EnableAhhjumpboostAhhfuck) {
                            if (player->getMoveInputHandler()->mInputState.mUp)
                            MovementUtils::setSpeed(AhhBoostSpeedHalal / 6.89);
                        }
                        if (JumpFromGroundNotSafeUDHalalNigger){
                            player->jumpFromGround();
                        }
                    }
                }
            }
            break;
        }

    }

    void onDisabled() override {
        Global::shouldLagTicks = false;

        if (!Address::getClientInstance())
            return;

        if (!Address::getClientInstance()->getTimerClass())
            return;

        Address::getClientInstance()->getTimerClass()->setMainTimerSpeed(20);
    }
};
