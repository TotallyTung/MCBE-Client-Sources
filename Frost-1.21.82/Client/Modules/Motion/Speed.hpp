#include "../Misc/Scaffold.hpp"

#pragma once

/*struct FrictionPreset
{
    float mSpeed = 0.5;
    bool mStrafe = true;
    bool mUseStrafeSpeed = true;
    float mStrafeSpeed = 0.5;
    float mFriction = 0.975;
    bool mTimerBoost = false;
    float mTimerSpeed = 20;
    FastfallMode mFastFall = FastfallMode::NoneFall;
    int mFallTicks = 5;
    float mFallSpeed = 1.00;
    bool mFastFall2 = false;
    int mFallTicks2 = 5;
    float mFallSpeed2 = 1.00;
    JumpTypeSet mJumpType = JumpTypeSet::Vanilla;
    float mJumpHeight = 0.42f;

    FrictionPreset() = default;
    FrictionPreset(float speed, bool strafe, bool useStrafeSpeed, float strafeSpeed, float friction, bool timerBoost, float timerSpeed, FastfallMode fastFall, int fallTicks, float fallSpeed, bool fastFall2, int fallTicks2, float fallSpeed2, JumpTypeSet jumpType, float jumpHeight)
        : mSpeed(speed), mStrafe(strafe), mUseStrafeSpeed(useStrafeSpeed), mStrafeSpeed(strafeSpeed), mFriction(friction), mTimerBoost(timerBoost), mTimerSpeed(timerSpeed), mFastFall(fastFall), mFallTicks(fallTicks), mFallSpeed(fallSpeed), mFastFall2(fastFall2), mFallTicks2(fallTicks2), mFallSpeed2(fallSpeed2), mJumpType(jumpType), mJumpHeight(jumpHeight)
    {}
};*/

class Speed : public Module
{
public:
    Speed() : 
        Module("Speed", "Motion", "Bhop like the flash.")
    {
        // Mode Enum
        EnumSetting("Mode", "The mode of speed", { "Friction", "Legit", "Flareon V2" }, &mMode);

        // Swiftness Settings
        BoolSetting("Swiftness", "Whether or not to apply swiftness when space is pressed (will not be applied when scaffold is enabled)", &mSwiftness);
        BoolSetting("Swiftness Hotbar", "Only uses swiftness from hotbar", &mSwiftnessHotbar, [this] { return mSwiftness; });
        BoolSetting("Hold Space", "Only applies swiftness effect while holding space", &mHoldSpace, [this] { return mSwiftness; });
        SliderSetting("Swiftness Speed", "The speed to apply when swiftness is active", &mSwiftnessSpeed, 0, 1, SliderType::DoubleFloat, [this] { return mSwiftness; });
        SliderSetting("Swiftness Friction", "The friction to apply when swiftness is active", &mSwiftnessFriction, 0, 1, SliderType::DoubleFloat, [this] { return mSwiftness; });
        SliderSetting("Swiftness Timer", "Whether or not to boost timer speed when swiftness is active", &mSwiftnessTimer, 0, 40, SliderType::DoubleFloat, [this] { return mSwiftness; });

        // Damage Boost Settings
        BoolSetting("Damage Boost", "Whether or not to boost speed when taking damage", &mDamageBoost);
        SliderSetting("Damage Boost Speed", "The speed to boost when taking damage", &mDamageBoostSpeed, 1, 10, SliderType::DoubleFloat, [this] { return mDamageBoost; });
        SliderSetting("Damage Boost Slowdown", "The friction to apply when taking damage", &mDamageBoostSlowdown, 0, 1, SliderType::DoubleFloat, [this] { return mDamageBoost; });
        BoolSetting("Damage Timer", "Whether or not to boost timer speed when taking damage", &mDamageTimer, [this] { return mDamageBoost; });
        SliderSetting("Damage Timer Speed", "The speed to boost timer by when taking damage", &mDamageTimerSpeed, 0, 40, SliderType::DoubleFloat, [this] { return mDamageBoost && mDamageTimer; });
        BoolSetting("Ignore Strafe", "Dont increase strafe speed", &mDontBoosStrafeSpeed, [this] { return mDamageBoost; });

        // Speed and Strafe Settings
        SliderSetting("Speed", "The speed to move at", &mSpeed, 0, 10, SliderType::DoubleFloat, [this] { return mMode != Mode::FlareonV2; });
        BoolSetting("Strafe Only", "Whether or not to allow strafing", &mStrafe, [this] { return mMode != Mode::Legit && mMode != Mode::FlareonV2; });
        BoolSetting("Test", "test", &mTest, [this] { return mMode != Mode::Legit && mMode != Mode::FlareonV2; });
        BoolSetting("Custom Strafe Speed", "Whether or not to apply custom speed when strafing", &mUseStrafeSpeed, [this] { return mMode != Mode::Legit; });
        SliderSetting("Strafe Speed", "The speed to strafe at", &mStrafeSpeed, 0, 10, SliderType::DoubleFloat, [this] { return mMode != Mode::Legit; });
        SliderSetting("Friction", "The friction to apply", &mFriction, 0, 1, SliderType::DoubleFloat, [this] { return mMode == Mode::Friction || mMode == Mode::Legit; });
        BoolSetting("Timer Boost", "Whether or not to boost timer speed", &mTimerBoost, [this] { return mMode == Mode::Friction || mMode == Mode::Legit; });
        SliderSetting("Timer Speed", "The speed to boost timer by", &mTimerSpeed, 0, 40, SliderType::DoubleFloat, [this] { return (mMode == Mode::Friction && mTimerBoost) || mMode == Mode::Legit; });

        // Fast Fall Settings
        EnumSetting("Fast Fall", "The mode of fast fall", { "None", "Predict", "Set Vel" }, &mFastFall, [this] { return mMode == Mode::Friction || mMode == Mode::Legit; });
        SliderSetting("Fall Ticks", "The tick to apply down motion at", &mFallTicks, 0, 20, SliderType::Int, [this] { return (mMode == Mode::Friction && mFastFall != FastfallMode::NoneFall) || (mMode == Mode::Legit && mFastFall != FastfallMode::NoneFall); });
        SliderSetting("Fall Speed", "The speed to fall down at", &mFallSpeed, 0, 10, SliderType::DoubleFloat, [this] { return (mMode == Mode::Friction && mFastFall != FastfallMode::NoneFall) || (mMode == Mode::Legit && mFastFall != FastfallMode::NoneFall); });
        BoolSetting("Fast Fall 2", "Whether or not to fast fall again", &mFastFall2, [this] { return (mMode == Mode::Friction && mFastFall != FastfallMode::NoneFall) || (mMode == Mode::Legit && mFastFall != FastfallMode::NoneFall); });
        SliderSetting("Fall Ticks 2", "The tick to apply down motion at", &mFallTicks2, 0, 20, SliderType::Int, [this] { return (mMode == Mode::Friction && mFastFall != FastfallMode::NoneFall && mFastFall2) || (mMode == Mode::Legit && mFastFall != FastfallMode::NoneFall && mFastFall2); });
        SliderSetting("Fall Speed 2", "The speed to fall down at", &mFallSpeed2, 0, 10, SliderType::DoubleFloat, [this] { return (mMode == Mode::Friction && mFastFall != FastfallMode::NoneFall && mFastFall2) || (mMode == Mode::Legit && mFastFall != FastfallMode::NoneFall && mFastFall2); });

        // Jump Settings
        EnumSetting("Jump Type", "The type of jump to use", { "Vanilla", "Velocity", "None" }, &mJumpType, [this] { return mMode == Mode::Friction || mMode == Mode::Legit; });
        SliderSetting("Jump Height", "The height to jump at", &mJumpHeight, 0, 1, SliderType::DoubleFloat, [this] { return (mMode == Mode::Friction && mJumpType != JumpTypeSet::None) || (mMode == Mode::Legit && mJumpType != JumpTypeSet::None); });
        BoolSetting("Apply Netskip", "Apply Netskip", &mApplyNetskip, [this] { return mMode == Mode::Friction || mMode == Mode::Legit; });
    }

private:
    enum Mode {
        Friction  = 0,
        Legit     = 1,
        FlareonV2 = 2
    };

    enum JumpTypeSet {
        Vanilla  = 0,
        Velocity = 1,
        None     = 2
    };


    enum FastfallMode {
        NoneFall = 0,
        Predict  = 1,
        SetVel   = 2
    };

    int mMode = 0;
    bool mSwiftness = false;
    bool mSwiftnessHotbar = false;
    bool mHoldSpace = false;
    float mSwiftnessSpeed = 0.55f;
    float mSwiftnessFriction = 0.975f;
    float mSwiftnessTimer = 35.0f;

    bool mDamageBoost = false;
    float mDamageBoostSpeed = 3.0f;
    float mDamageBoostSlowdown = 0.15f;
    bool mDamageTimer = false;
    float mDamageTimerSpeed = 20.0f;
    bool mDontBoosStrafeSpeed = true;

    float mSpeed = 0.5f;
    bool mStrafe = true;
    bool mTest = true;
    bool mUseStrafeSpeed = true;
    float mStrafeSpeed = 0.5f;
    float mFriction = 0.975f;
    bool mTimerBoost = false;
    float mTimerSpeed = 20.0f;

    int mFastFall = 0;
    float mFallTicks = 5;
    float mFallSpeed = 1.0f;
    bool mFastFall2 = false;
    float mFallTicks2 = 5;
    float mFallSpeed2 = 1.0f;

    int mJumpType = 0;
    float mJumpHeight = 0.42f;
    bool mApplyNetskip = false;



    std::map<MobEffect::EffectType, uint64_t> mEffectTimers = {};
    float mDamageBoostVal = 1.f;
    bool mDamageTimerApplied = false;
public:

    void onEnabled() override 
    {
        mDamageBoostVal = 1.f;
        mDamageTimerApplied = false;
    }

    void onDisabled() override 
    {
        InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(20.f);
        mDamageBoostVal = 1.f;
        mDamageTimerApplied = false;
    }

    void onEvent(RunUpdateCycleEvent* event) 
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;

        auto mScaffold = getModuleByType<Scaffold>();
        if (mScaffold->isEnabled()) return;

        bool mApplyNetSkipBoolean = false;

        if (mMode != Mode::Friction || mApplyNetskip) {
            mApplyNetSkipBoolean = true;
        }

        if (!mApplyNetSkipBoolean) return;

        Sleep(101);
    }

    // SwiftNess later
    bool tickSwiftness() 
    {
        return false;
    }

    void onEvent(BaseTickEvent* event) override 
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;

        float mDmgSlowDown = mDamageBoostSlowdown;
        mDamageTimer = MathUtil::lerp(mDamageBoostVal, 1.f, mDmgSlowDown);

        if (mDamageTimer && mDamageBoostVal > 1.04f) {
            mDamageTimerApplied = true;
            InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(mDamageTimerSpeed);
        }
        else {
            mDamageTimerApplied = false;
        }

        if (mPlayer->hasComponent<RedirectCameraInputComponent>()) {
            return;
        }

        if (mSwiftness) {
            /*if (tickSwiftness()) {
                return;
            }*/
        }

        switch (mMode) 
        {
        case 0:
            tickFriction(mPlayer);
            break;
        case 1:
            tickLegit(mPlayer);
            break;
        case 2:
            mPlayer->getComponent<MoveInputComponent>()->mSprinting = true;

            if (mPlayer->hasComponent<OnGroundFlagComponent>() && MotionUtil::isMoving()) 
            {
                mPlayer->getComponent<MoveInputComponent>()->mJumping = true;
            }
            else 
            {
                mPlayer->getComponent<MoveInputComponent>()->mJumping = false;
            }
            
            break;
        }
    }

    void onEvent(ActorSetMotionEvent* event) override 
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;

        mDamageBoostVal = mDamageBoostSpeed;
    }

    void onEvent(PacketEvent* event) override 
    {
        if (event->mPacket->getId() == PacketID::PlayerAuthInput) {
            auto* pkt = reinterpret_cast<PlayerAuthInputPacket*>(event->mPacket);
            Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
            if (!mPlayer) return;

            if (mMode == Mode::FlareonV2) return;

            if (mPlayer->getComponent<MoveInputComponent>()->isPressed()) {
                pkt->mInputData |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN;
            }

            if (!mPlayer->hasComponent<OnGroundFlagComponent>() && mPlayer->hasComponent<WasOnGroundFlagComponent>() && mPlayer->getComponent<MoveInputComponent>()->isPressed()) {
                pkt->mInputData |= AuthInputAction::START_JUMPING;
            }

            if (mStrafe && mTest) {
                Vector2<float> moveVec = pkt->mMove;
                Vector2<float> xzVel = { pkt->mPosDelta.x, pkt->mPosDelta.z };
                float yaw = pkt->mRotation.y;
                yaw = -yaw;

                if (moveVec.x == 0 && moveVec.y == 0 && xzVel.x == 0 && xzVel.y == 0) return;

                float moveVecYaw = atan2(moveVec.x, moveVec.y);
                moveVecYaw = glm::degrees(moveVecYaw);

                float movementYaw = atan2(xzVel.x, xzVel.y);
                float movementYawDegrees = movementYaw * (180.0f / PI);

                float yawDiff = movementYawDegrees - yaw;

                float newMoveVecX = sin(glm::radians(yawDiff));
                float newMoveVecY = cos(glm::radians(yawDiff));
                Vector2<float> newMoveVec = { newMoveVecX, newMoveVecY };

                if (abs(newMoveVec.x) < 0.001) newMoveVec.x = 0;
                if (abs(newMoveVec.y) < 0.001) newMoveVec.y = 0;
                if (moveVec.x == 0 && moveVec.y == 0) newMoveVec = { 0, 0 };

                // Remove all old flags
                pkt->mInputData &= ~AuthInputAction::UP;
                pkt->mInputData &= ~AuthInputAction::DOWN;
                pkt->mInputData &= ~AuthInputAction::LEFT;
                pkt->mInputData &= ~AuthInputAction::RIGHT;
                pkt->mInputData &= ~AuthInputAction::UP_RIGHT;
                pkt->mInputData &= ~AuthInputAction::UP_LEFT;

                pkt->mMove = newMoveVec;
                pkt->mVehicleRotation = newMoveVec; // ???? wtf mojang
                pkt->mInputMode = InputMode::MotionController;

                // Get the move direction
                bool forward = newMoveVec.y > 0;
                bool backward = newMoveVec.y < 0;
                bool left = newMoveVec.x < 0;
                bool right = newMoveVec.x > 0;

                static bool isSprinting = false;
                bool startedThisTick = false;
                // if the flags contain isSprinting, set the flag
                if (pkt->hasInputData(AuthInputAction::START_SPRINTING)) {
                    isSprinting = true;
                    startedThisTick = true;
                }
                else if (pkt->hasInputData(AuthInputAction::STOP_SPRINTING)) {
                    isSprinting = false;
                }

                if (!forward) {
                    // Remove all sprint flags
                    pkt->mInputData &= ~AuthInputAction::START_SPRINTING;
                    if (isSprinting && !startedThisTick) {
                        pkt->mInputData |= AuthInputAction::STOP_SPRINTING;
                    }

                    pkt->mInputData &= ~AuthInputAction::SPRINTING;
                    pkt->mInputData &= ~AuthInputAction::START_SNEAKING;

                    // Stop the player from sprinting
                    mPlayer->getComponent<MoveInputComponent>()->mSprinting = false;
                }
            }
        }
    }

    void tickLegit(Actor* mPlayer) 
    {
        if (!mDamageTimerApplied)
        {
            if (mTimerBoost) InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(mTimerSpeed);
            else InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(20.f);
        }

        static int mTick = 0;

        if (mPlayer->hasComponent<OnGroundFlagComponent>())
            mTick = 0;

        else mTick++;

        if (mFastFall == FastfallMode::Predict)
        {
            if (mTick == mFallTicks)
            {
                auto fallSpeed = mFallSpeed;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                for (int i = 0; i < fallSpeed; i++)
                {
                    stateVector->mVelocity.y = (stateVector->mVelocity.y - 0.08f) * 0.98f;
                }
            }
            if (mFastFall2 && mTick == mFallTicks2)
            {
                auto fallSpeed = mFallSpeed2;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                for (int i = 0; i < fallSpeed; i++)
                {
                    stateVector->mVelocity.y = (stateVector->mVelocity.y - 0.08f) * 0.98f;
                }
            }
        } // Sets the velocity directly
        else if (mFastFall == FastfallMode::SetVel)
        {
            if (mTick == mFallTicks)
            {
                auto fallSpeed = mFallSpeed / 10;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                stateVector->mVelocity.y = -fallSpeed;
            }
            if (mFastFall2 && mTick == mFallTicks2)
            {
                auto fallSpeed = mFallSpeed2 / 10;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                stateVector->mVelocity.y = -fallSpeed;
            }
        }

        Vector3<float> velocity = mPlayer->getComponent<StateVectorComponent>()->mVelocity;
        float movementSpeed = sqrt(velocity.x * velocity.x + velocity.z * velocity.z);

        if (!mPlayer->hasComponent<OnGroundFlagComponent>() && mPlayer->getComponent<MoveInputComponent>()->isPressed() && mPlayer->hasComponent<WasOnGroundFlagComponent>())
        {
            movementSpeed += (mSpeed / 10 / 10) * mDamageBoostVal;
        }
        else if (mPlayer->getComponent<MoveInputComponent>()->isPressed() && !mPlayer->hasComponent<OnGroundFlagComponent>() && !mPlayer->hasComponent<WasOnGroundFlagComponent>() && mPlayer->getComponent<FallDistanceComponent>()->mFallDistance > 0) {
            movementSpeed *= mFriction;
        }

        float movementYaw = atan2(velocity.z, velocity.x);
        float movementYawDegrees = movementYaw * (180.0f / PI) - 90.f;
        float newMovementYaw = movementYaw;

        Vector3<float> newVelocity = { cos(newMovementYaw) * movementSpeed, velocity.y, sin(newMovementYaw) * movementSpeed };
        if (mSpeed != 0)
            mPlayer->getComponent<StateVectorComponent>()->mVelocity = newVelocity;

        bool usingMoveKeys = mPlayer->getComponent<MoveInputComponent>()->isPressed();
        if (usingMoveKeys && mJumpType == JumpTypeSet::Vanilla) {
            if (mPlayer->hasComponent<OnGroundFlagComponent>())
            {
                mPlayer->jumpFromGround();
                mPlayer->getComponent<StateVectorComponent>()->mVelocity.y = mJumpHeight;
            }
        }
        else if (usingMoveKeys && mJumpType == JumpTypeSet::Velocity) {
            if (mPlayer->hasComponent<OnGroundFlagComponent>())
            {
                mPlayer->getComponent<StateVectorComponent>()->mVelocity.y = mJumpHeight;
            }
        }
    }

    void tickFriction(Actor* mPlayer)
    {
        if (!mDamageTimerApplied)
        {
            if (mTimerBoost) InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(mTimerSpeed);
            else InstanceManager::get<ClientInstance>()->getMinecraft()->setMainTimer(20.f);
        }

        static auto friction = mFriction;
        static int mTick = 0;

        if (mPlayer->hasComponent<OnGroundFlagComponent>())
        {
            friction = 1.f;
            mTick = 0;
        }
        else
        {
            friction *= mFriction;
            mTick++;
        }

        // Predicts the next y velocity and applies it
        if (mFastFall == FastfallMode::Predict)
        {
            if (mTick == mFallTicks)
            {
                auto fallSpeed = mFallSpeed;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                for (int i = 0; i < fallSpeed; i++)
                {
                    stateVector->mVelocity.y = (stateVector->mVelocity.y - 0.08f) * 0.98f;
                }
            }
            if (mFastFall2 && mTick == static_cast<int>(mFallTicks2))
            {
                auto fallSpeed = mFallSpeed2;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                for (int i = 0; i < fallSpeed; i++)
                {
                    stateVector->mVelocity.y = (stateVector->mVelocity.y - 0.08f) * 0.98f;
                }
            }
        } // Sets the velocity directly
        else if (mFastFall == FastfallMode::SetVel)
        {
            if (mTick == static_cast<int>(mFallTicks))
            {
                auto fallSpeed = mFallSpeed / 10;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                stateVector->mVelocity.y = -fallSpeed;
            }
            if (mFastFall2 && mTick == static_cast<int>(mFallTicks2))
            {
                auto fallSpeed = mFallSpeed2 / 10;
                auto stateVector = mPlayer->getComponent<StateVectorComponent>();
                stateVector->mVelocity.y = -fallSpeed;
            }
        }

        float speed = mSpeed;

        if (mPlayer->getComponent<MoveInputComponent>()->isStrafing() && mUseStrafeSpeed) speed = mStrafeSpeed;

        if (mDontBoosStrafeSpeed) {
            MotionUtil::setSpeed((speed / 10) * friction);
        }
        else {
            MotionUtil::setSpeed(((speed * mDamageBoostVal) / 10) * friction);
        }

        bool usingMoveKeys = mPlayer->getComponent<MoveInputComponent>()->isPressed();
        if (usingMoveKeys && mJumpType == JumpTypeSet::Vanilla) {
            if (mPlayer->hasComponent<OnGroundFlagComponent>())
            {
                mPlayer->jumpFromGround();
                mPlayer->getComponent<StateVectorComponent>()->mVelocity.y = mJumpHeight;
            }
        }
        else if (usingMoveKeys && mJumpType == JumpTypeSet::Velocity) {
            if (mPlayer->hasComponent<OnGroundFlagComponent>()) {
                mPlayer->getComponent<StateVectorComponent>()->mVelocity.y = mJumpHeight;
            }
        }
    };
};
