//
// Created by alteik on 04/10/2024.
//

#include "Criticals.hpp"
#include <Features/Events/PacketOutEvent.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/PacketID.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Components/StateVectorComponent.hpp>

void Criticals::onEnable() {
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Criticals::onPacketOutEvent>(this);
}

void Criticals::onDisable() {
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Criticals::onPacketOutEvent>(this);
}

void Criticals::onPacketOutEvent(PacketOutEvent &event) {
    auto player = ClientInstance::get()->getLocalPlayer();
    if(!player) return;

    if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
        auto paip = event.getPacket<PlayerAuthInputPacket>();

        if(mMode.mValue == Mode::Sentinel) {
            auto mStateVector = player->getStateVectorComponent();
            if(mStateVector->mPosOld.y == mStateVector->mPos.y) {

                if(mSendJumping.mValue) {
                    paip->mInputData |= AuthInputAction::JUMP_DOWN;
                    paip->mInputData |= AuthInputAction::JUMPING;
                    paip->mInputData |= AuthInputAction::WANT_UP;
                }

                switch (mAnimationState) {
                    case AnimationState::START:
                        if(mVelocity.mValue)
                            paip->mPosDelta.y = -0.07840000092983246f;
                        mAnimationState = AnimationState::MID_AIR;
                        break;

                    case AnimationState::MID_AIR:

                        if(mPositionChange.mValue)
                            paip->mPosDelta.y += mBiggerPositionChange.mValue ? mJumpPositions[1] * mPositionChangePersent.mValue : mJumpPositionsMini[1] * mPositionChangePersent.mValue;
                        if(mVelocity.mValue)
                            paip->mPosDelta.y = -0.07840000092983246f;
                        mAnimationState = AnimationState::MID_AIR2;
                        break;

                    case AnimationState::MID_AIR2:
                        if (mSendJumping.mValue) {
                            paip->mInputData &= ~AuthInputAction::JUMP_DOWN;
                            paip->mInputData &= ~AuthInputAction::JUMPING;
                            paip->mInputData &= ~AuthInputAction::WANT_UP;
                        }

                        if(mPositionChange.mValue)
                            paip->mPosDelta.y += mBiggerPositionChange.mValue ? mJumpPositions[2] * mPositionChangePersent.mValue : mJumpPositionsMini[2] * mPositionChangePersent.mValue;
                        if(mVelocity.mValue)
                            paip->mPosDelta.y = -0.1552319973707199f;
                        mAnimationState = AnimationState::LANDING;
                        break;

                    case AnimationState::LANDING:
                        if(mVelocity.mValue)
                            paip->mPosDelta.y = -0.07840000092983246f;
                        mAnimationState = AnimationState::FINISHED;
                        break;

                    case AnimationState::FINISHED:
                        mAnimationState = AnimationState::START;
                        break;
                }
            }
        }

        if (mOffSprint.mValue) {
            paip->mInputData &= ~AuthInputAction::START_SPRINTING;
            paip->mInputData &= ~AuthInputAction::SPRINTING;
            paip->mInputData &= ~AuthInputAction::SPRINT_DOWN;
            paip->mInputData &= ~AuthInputAction::STOP_SPRINTING;
        }
    }
}
