#pragma once

class MotionUtil
{
public:
    static Vector2<float> getMoveVec()
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();
        Vector2<float> mMoveVec = { 0.0f, 0.0f };

        if (!mPlayer)
            return mMoveVec;

        if (mPlayer->getComponent<MoveInputComponent>()->mForward)
            mMoveVec.x += 1.0f;

        if (mPlayer->getComponent<MoveInputComponent>()->mBackward)
            mMoveVec.x -= 1.0f;

        if (mPlayer->getComponent<MoveInputComponent>()->mLeft)
            mMoveVec.y -= 1.0f;

        if (mPlayer->getComponent<MoveInputComponent>()->mRight)
            mMoveVec.y += 1.0f;

        return mMoveVec;
    }

    static void setSpeed(float speed) {
        Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();

        if (!mPlayer)
            return;

        float mCalcYaw = (mPlayer->getComponent<ActorRotationComponent>()->mRotation.y + 90) * (PI / 180);
        float c = cos(mCalcYaw);
        float s = sin(mCalcYaw);

        Vector2<float> mMoveVec = getMoveVec().normalize();
        mMoveVec = { mMoveVec.x * c - mMoveVec.y * s, mMoveVec.x * s + mMoveVec.y * c };

        Vector3<float> mMovement;
        mMovement.x = mMoveVec.x * speed;
        mMovement.y = mPlayer->getComponent<StateVectorComponent>()->mVelocity.y;
        mMovement.z = mMoveVec.y * speed;

        mPlayer->getComponent<StateVectorComponent>()->mVelocity.z = mMovement.z;
        mPlayer->getComponent<StateVectorComponent>()->mVelocity.x = mMovement.x;
    }

    static bool isMoving() {
        Player* mPlayer = InstanceManager::get<ClientInstance>() == nullptr ? nullptr : InstanceManager::get<ClientInstance>()->getLocalPlayer();

        if (!mPlayer)
            return false;

        return mPlayer->getComponent<StateVectorComponent>()->mVelocity.magnitudexz() >= 0.045f;
    }
};