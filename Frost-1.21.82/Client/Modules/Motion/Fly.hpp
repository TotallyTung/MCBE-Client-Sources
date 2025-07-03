#pragma once

class Fly : public Module
{
public:
    Fly() :
        Module("Fly", "Motion", "Fly like a superhero.")
    {
        EnumSetting("Mode", "The type of fly", { "Vanilla" }, &mMode);
        BoolSetting("Halt", "Halts the fly on disabling.", &mHalt);
        SliderSetting("Speed", "the delay between placing with ticks.", &mSpeed, 0.3, 2);
    }
private:
    int mMode = 0;
    bool mHalt = true;
    float mSpeed = 0.6;
public:
    void onEvent(BaseTickEvent* event) override 
    {
        if (!InstanceManager::getLocalPlayer())
            return;

        Player* mPlayer = InstanceManager::getLocalPlayer();
        StateVectorComponent* mState = mPlayer->getStateVectorComponent();

        switch (mMode) {
        case 0: // Vanilla
            mState->mVelocity.y = 0;

            MotionUtil::setSpeed(mSpeed);

            if (mPlayer->getMoveInputHandler()->mJumping) {
                mState->mVelocity.y += 0.3;
            }
            else if (mPlayer->getMoveInputHandler()->mSneaking) {
                mState->mVelocity.y -= 0.3;
            }
            break;
        }
    }

    void onDisabled() override 
    {
        if (mHalt) {
            MotionUtil::setSpeed(0);
        }
    }
};