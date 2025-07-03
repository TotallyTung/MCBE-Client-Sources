#include "../Misc/Regen.hpp"

#pragma once

class Killaura : public Module
{
public:
    Killaura() :
        Module("Killaura", "Combat", "Attack players in your radius")
    {
        EnumSetting("Target Mode", "How many entities should be attacked", { "Single", "Multi" }, &mSwitchMode);
        EnumSetting("Rotations", "The style of the rotations", { "None", "Normal", "Flick" }, &mRotationMode);
        EnumSetting("Bypass", "The style of the bypassing", { "None", "Raycast" }, &mBypassMode);

        EnumSetting("Swing", "Controls how it will send the swing action", { "Normal", "None" }, &mSwing);
        EnumSetting("AutoSlot", "The way the players arm swing", { "Disabled", "Switch", "Spoof", "Silent" }, &mAutoSlot);
        SliderSetting("Range", "Controls the attacks per second", &mRange, 3, 10);
        SliderSetting("APS", "attack per second", &mAPS, 0, 20);
        BoolSetting("Strafe", "Strafe the player towards the entity", &mStrafe);
        BoolSetting("Timed swing", "Times the swing with the attack", &mTimedSwing);
        BoolSetting("Hotbar Only", "Bypass the Invalid Packet! kick", &mHotbarOnly);
    }

private:
    int mSwitchMode = 0;
    int mRotationMode = 0;
    int mBypassMode = 0;
    int mSwing = 0; // enum
    int mAutoSlot = 0; // enum
    float mRange = 5;
    float mAPS = 10;
    bool mStrafe = false;
    bool mTimedSwing = true;
    bool mHotbarOnly = true;

    enum RotateMode {
        None   = 0,
        Normal = 1,
        Flick  = 2
    };
    
    enum SwitchMode {
        Single = 0,
        Multi  = 1,
        Switch = 2
    };

    int mBetterSwordSlot = 0;

    Vector3<float> mTargetedActor;
    bool mRotating = false;
public:
    void onDisabled() override {
        GlobalVariables::mShouldBlock = false;
    }

    bool findSword() {
        PlayerInventory* playerInventory = InstanceManager::getLocalPlayer()->getSupplies();
        Inventory* inventory = playerInventory->getInventory();
        auto previousSlot = playerInventory->mSelectedSlot;
        int slot = previousSlot;

        float currentSwordValue = 0;

        for (int i = 0; i < (mHotbarOnly ? 9 : 36); i++) {
            ItemStack* stack = inventory->getItemStack(i);
            if (stack->mItem != nullptr) {
                float itemDamage = stack->getItem()->getSwordValueFromName() + stack->getEnchantValue(EnchantType::Sharpness);
                if (itemDamage > currentSwordValue) {
                    currentSwordValue = itemDamage;
                    mBetterSwordSlot = i;
                }
            }
        }

        return currentSwordValue != 0;
    }

    void AutoSlotManagement() {
        if (mAutoSlot == 1 && findSword()) {
            InstanceManager::getLocalPlayer()->getSupplies()->mSelectedSlot = mBetterSwordSlot;
        }

        if (mAutoSlot == 2 && findSword()) {
            PacketUtil::spoofSwitch(mBetterSwordSlot);
        }

        if (mAutoSlot == 3 && findSword()) {
            InstanceManager::getLocalPlayer()->getSupplies()->mSelectedSlot = mBetterSwordSlot;
        }
    }

    void rotate(Actor* mTarget) 
    {
        if (mRotationMode == RotateMode::None) return;
        auto player = InstanceManager::getLocalPlayer();
        if (!player) return;

        mTargetedActor = mTarget->getPosition();
        mRotating = true;
    }

    void onEvent(BaseTickEvent* event) override {
        Player* mPlayer = InstanceManager::getLocalPlayer();
        GameMode* mGameMode = mPlayer->getGameMode();

        if (!mPlayer || !mGameMode || !InstanceManager::isAllowedToUseKeys())
        {
            return;
        }

        if (getModuleByType<Regen>()->isEnabled() && GlobalVariables::mIsMiningRedstone) {
            return;
        }

        if (!ActorUtils::getActorList(mRange, false).empty()) {
            Vector3<float> mLocalPos = mPlayer->getPosition();
            Vector3<float> mPlayerPos = ActorUtils::getActorList(mRange, false)[0]->getPosition();

            Vector2<float> angle = MathUtil::getRots(mLocalPos, mPlayerPos);

            GlobalVariables::mShouldBlock = true;

            if (TimeUtil::hasTimeElapsed("kaTimer", 1000 / mAPS, true)) {
                int mSelectedSlot = mPlayer->getSupplies()->mSelectedSlot;

                AutoSlotManagement();

                if (mSwing == 0) { mPlayer->swing(); }

                for (auto mActor : ActorUtils::getActorList(mRange, false)) {
                    if (mSwitchMode == SwitchMode::Single) 
                    {
                        mActor = ActorUtils::getActorList(mRange, false)[0];
                    }
                    else if (mSwitchMode == SwitchMode::Multi)
                    {

                    }

                    if (mRotationMode == RotateMode::Normal)
                        rotate(mActor);

                    auto oldActor = mActor;
                    mActor = ActorUtils::getObstructingActor(mPlayer, mActor, mRange, mBypassMode);

                    if (mRotationMode == RotateMode::Flick)
                    {
                        rotate(mActor);
                    }

                    mGameMode->attack(*mActor);
                }
                

                /*if (mSwitchMode == 0) {
                    mGameMode->attack();
                }

                if (mSwitchMode == 1) {
                    for (auto* actor : ActorUtils::getActorList(mRange, false)) {
                        mGameMode->attack(*actor);
                    }
                }*/

                if (mAutoSlot == 3) {
                    mPlayer->getSupplies()->mSelectedSlot = mSelectedSlot;
                }
            }

            if (mStrafe) {
                mPlayer->getComponent<ActorRotationComponent>()->mRotation = angle;
            }
        }
        else {
            mRotating = false;
            GlobalVariables::mShouldBlock = false;
        }
    }

    void onEvent(PacketEvent* event) override 
    {
        auto player = InstanceManager::getLocalPlayer();
        if (!player || !InstanceManager::isAllowedToUseKeys || !mRotating)
        {
            return;
        }

        if (getModuleByName("regen")->isEnabled() && GlobalVariables::mIsMiningRedstone) 
        {
            return;
        }

        PlayerAuthInputPacket* mPacket = nullptr;
        MovePlayerPacket* mMovePacket = nullptr;

        if (event->mPacket->getId() == PacketID::PlayerAuthInput) 
        {
            mPacket = (PlayerAuthInputPacket*)event->mPacket;
        }
        
        if (event->mPacket->getId() == PacketID::MovePlayer) 
        {
            mMovePacket = (MovePlayerPacket*)event->mPacket;
        }

        if (!ActorUtils::getActorList(mRange, false).empty()) {
            Vector3<float> mLocalPos = player->getPosition();
            Vector3<float> mPlayerPos = ActorUtils::getActorList(mRange, false)[0]->getPosition();
            Vector2<float> mAngle = MathUtil::getRots(mLocalPos, mTargetedActor);

            PacketUtil::setPlayerAuthInputRotations(mAngle, mPacket);
            PacketUtil::setMovePlayerRotations(mAngle, mMovePacket);
        }
    }
};