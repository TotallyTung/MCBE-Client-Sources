#pragma once

class ServerRotations : public Module
{
public:
    ServerRotations() : 
        Module("ServerRotations", "Visual", "Shows your serversided player state.", Keys::NONE, true)
    {
        EnumSetting("Body Yaw", "Determines the synchronization state of body yaw rotation with movement. Select between standard behavior or a synchronized orientation.", { "Normal", "Synced" }, &mBodyYawState);
        BoolSetting("Reset", "Resets the easing transition parameters to their default state, restoring initial values for interpolation control.", &mReset);
        SliderSetting("Ease Speed", "Adjusts the rate of transition smoothing, controlling the interpolation velocity between motion states. Ranges from minimal to maximum easing speed.", &mEase, 1, 10);
    }

    int mBodyYawState = 0;

    float mHeadYaw = 0.f;
    float mBodyYaw = 0.f;
    float mPitch = 0.f;

    float mEasedHeadYaw = 0.f;
    float mEasedBodyYaw = 0.f;
    float mEasedPitch = 0.f;

    bool mShouldEase = true;

    float mEase = 2.07;

    bool mReset = false;

    void onEvent(RenderContextEvent* event) override 
    {
        auto mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;

        if (mReset) {
            mEase = 2.07;

            mReset = false;
        }

        // Rotations
        mEasedHeadYaw = MathUtil::wrap(mEasedHeadYaw, mHeadYaw - 180.f, mHeadYaw + 180.f);
        mEasedHeadYaw = MathUtil::lerp(mEasedHeadYaw, mHeadYaw, ImRenderUtil::getDeltaTime() * mEase);

        if (mBodyYawState == 1) {
            mEasedBodyYaw = MathUtil::wrap(mEasedBodyYaw, mEasedHeadYaw - 180.f, mEasedHeadYaw + 180.f);
            mEasedBodyYaw = MathUtil::lerp(mEasedBodyYaw, mEasedHeadYaw, ImRenderUtil::getDeltaTime() * (mEase - 1.03));
        }
        else {
            mEasedBodyYaw = MathUtil::wrap(mEasedBodyYaw, mBodyYaw - 180.f, mBodyYaw + 180.f);
            mEasedBodyYaw = MathUtil::lerp(mEasedBodyYaw, mBodyYaw, ImRenderUtil::getDeltaTime() * (mEase - 1.03));
        }

        mEasedPitch = MathUtil::lerp(mEasedPitch, mPitch, ImRenderUtil::getDeltaTime() * mEase);
    }
};