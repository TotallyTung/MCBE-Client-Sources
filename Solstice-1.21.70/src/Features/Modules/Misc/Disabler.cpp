//
// Created by vastrakai on 8/9/2024.
//

#include "Disabler.hpp"

#include <Features/Events/PacketInEvent.hpp>
#include <Features/Events/PacketOutEvent.hpp>
#include <Features/Events/RunUpdateCycleEvent.hpp>
#include <Features/Events/PingUpdateEvent.hpp>
#include <Features/Events/SendImmediateEvent.hpp>
#include <Hook/Hooks/NetworkHooks/PacketReceiveHook.hpp>
#include <SDK/Minecraft/ClientInstance.hpp>
#include <SDK/Minecraft/Actor/Actor.hpp>
#include <SDK/Minecraft/Actor/GameMode.hpp>
#include <SDK/Minecraft/Network/LoopbackPacketSender.hpp>
#include <SDK/Minecraft/Network/MinecraftPackets.hpp>
#include <SDK/Minecraft/Network/Packets/PlayerAuthInputPacket.hpp>
#include <SDK/Minecraft/Network/Packets/NetworkStackLatencyPacket.hpp>

void Disabler::onEnable()
{
    gFeatureManager->mDispatcher->listen<PacketOutEvent, &Disabler::onPacketOutEvent, nes::event_priority::VERY_LAST>(this);
    gFeatureManager->mDispatcher->listen<RunUpdateCycleEvent, &Disabler::onRunUpdateCycleEvent, nes::event_priority::VERY_FIRST>(this);
    gFeatureManager->mDispatcher->listen<SendImmediateEvent, &Disabler::onSendImmediateEvent>(this);
    gFeatureManager->mDispatcher->listen<PingUpdateEvent, &Disabler::onPingUpdateEvent, nes::event_priority::FIRST>(this);
    gFeatureManager->mDispatcher->listen<PacketInEvent, &Disabler::onPacketInEvent>(this);

    mShouldUpdateClientTicks = true;
}

void Disabler::onDisable()
{
    gFeatureManager->mDispatcher->deafen<PacketOutEvent, &Disabler::onPacketOutEvent>(this);
    gFeatureManager->mDispatcher->deafen<RunUpdateCycleEvent, &Disabler::onRunUpdateCycleEvent>(this);
    gFeatureManager->mDispatcher->deafen<SendImmediateEvent, &Disabler::onSendImmediateEvent>(this);
    gFeatureManager->mDispatcher->deafen<PingUpdateEvent, &Disabler::onPingUpdateEvent>(this);
    gFeatureManager->mDispatcher->deafen<PacketInEvent, &Disabler::onPacketInEvent>(this);

    mPacketQueue.clear();
}

void Disabler::onPacketInEvent(class PacketInEvent& event)
{
#ifdef __PRIVATE_BUILD__
    // don't release this im not allowed to

    if (mMode.mValue == Mode::SentinelNew && event.mPacket->getId() == PacketID::NetworkStackLatency)
    {
        auto latency = event.getPacket<NetworkStackLatencyPacket>();
        if (!latency->mFromServer)
        {
            latency->mFromServer = true;
            return;
        }

        if (mPacketQueue.size() >= static_cast<int>(mQueuedPackets.mValue))
        {
            for (auto& [first, second]: mPacketQueue)
            {
                auto newLatency = MinecraftPackets::createPacket<NetworkStackLatencyPacket>();
                newLatency->mCreateTime = second;
                newLatency->mFromServer = false; // To prevent infinite loop lol
                ClientInstance::get()->getPacketSender()->sendToServer(newLatency.get());;
            }

            mPacketQueue.clear();
        }

        mPacketQueue[NOW] = latency->mCreateTime;
        event.cancel();
    }
#endif
}

/*glm::vec2 MathUtils::getMovement() {
    glm::vec2 ret = glm::vec2(0, 0);
    float forward = 0.0f;
    float side = 0.0f;

    bool w = Keyboard::mPressedKeys['W'];
    bool a = Keyboard::mPressedKeys['A'];
    bool s = Keyboard::mPressedKeys['S'];
    bool d = Keyboard::mPressedKeys['D'];

    if (!w && !a && !s && !d)
        return ret;

    static constexpr float forwardF = 1;
    static constexpr float sideF = 0.7071067691f;

    if (w) {
        if (!a && !d)
            forward = forwardF;
        if (a) {
            forward = sideF;
            side = sideF;
        }
        else if (d) {
            forward = sideF;
            side = -sideF;
        }
    }
    else if (s) {
        if (!a && !d)
            forward = -forwardF;
        if (a) {
            forward = -sideF;
            side = sideF;
        }
        else if (d) {
            forward = -sideF;
            side = -sideF;
        }
    }
    else if (!w && !s) {
        if (!a && d) side = -forwardF;
        else side = forwardF;
    }

    ret.x = side;
    ret.y = forward;
    return ret;
}*/

void Disabler::onPacketOutEvent(PacketOutEvent& event)
{
    auto player = ClientInstance::get()->getLocalPlayer();
    if (!player) return;

    if (mMode.mValue == Mode::Test && event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto packet = event.getPacket<PlayerAuthInputPacket>();
        ClientInstance::get()->getPacketSender()->sendToServer(packet);
        ChatUtils::displayClientMessage("Called");
        return;
    }

    if (mMode.mValue == Mode::Lifeboat && event.mPacket->getId() == PacketID::PlayerAuthInput && (player->getFlag<SetMovingFlagRequestComponent>() || !player->isOnGround()))
    {
        auto packet = event.getPacket<PlayerAuthInputPacket>();

        packet->mInputData &= ~AuthInputAction::JUMPING;
        packet->mInputData &= ~AuthInputAction::WANT_UP;
        packet->mInputData &= ~AuthInputAction::JUMP_DOWN;
        packet->mInputData &= ~AuthInputAction::START_JUMPING;

        packet->mInputData |= AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN;

        float targetY = packet->mPos.y - 1.f;

        static int jumpTicks = 0;
        static int maxTicks = 11;

        if (jumpTicks < maxTicks)
        {
            packet->mInputData |= AuthInputAction::START_JUMPING;
            jumpTicks++;
        }
        else
        {
            packet->mInputData &= ~AuthInputAction::START_JUMPING;
            jumpTicks = 0;
        }

        float tickPerc = (float)jumpTicks / (float)maxTicks;
        float newY = MathUtils::lerp(packet->mPos.y, targetY, tickPerc);
        packet->mPos.y = newY;
    }

#ifdef __PRIVATE_BUILD__
    if (mMode.mValue == Mode::SentinelNew && event.mPacket->getId() == PacketID::PlayerAuthInput)
    {
        auto paip = event.getPacket<PlayerAuthInputPacket>();
        //paip->mClientTick -= mQueuedPackets.mValue;
    }

    if (mMode.mValue == Mode::Flareon) {
        if (mDisablerType.mValue == DisablerType::MoveFix) {
            if (event.mPacket->getId() != PacketID::PlayerAuthInput) return;

            // I hate math so much

            auto pkt = event.getPacket<PlayerAuthInputPacket>();
            glm::vec2 moveVec = pkt->mMove;
            glm::vec2 xzVel = { pkt->mPosDelta.x, pkt->mPosDelta.z };
            float yaw = pkt->mRot.y;
            yaw = -yaw;

            if (moveVec.x == 0 && moveVec.y == 0 && xzVel.x == 0 && xzVel.y == 0) return;

            float moveVecYaw = atan2(moveVec.x, moveVec.y);
            moveVecYaw = glm::degrees(moveVecYaw);

            float movementYaw = atan2(xzVel.x, xzVel.y);
            float movementYawDegrees = movementYaw * (180.0f / M_PI);

            float yawDiff = movementYawDegrees - yaw;

            float newMoveVecX = sin(glm::radians(yawDiff));
            float newMoveVecY = cos(glm::radians(yawDiff));
            glm::vec2 newMoveVec = { newMoveVecX, newMoveVecY };

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

            spdlog::info("Forward: {}, Backward: {}, Left: {}, Right: {}", forward ? "true" : "false", backward ? "true" : "false", left ? "true" : "false", right ? "true" : "false");

            if (!forward)
            {
                // Remove all sprint flags
                pkt->mInputData &= ~AuthInputAction::START_SPRINTING;
                if (isSprinting && !startedThisTick) {
                    pkt->mInputData |= AuthInputAction::STOP_SPRINTING;
                    spdlog::info("Stopping sprint");
                }

                spdlog::info("Not moving forward");

                pkt->mInputData &= ~AuthInputAction::SPRINTING;
                pkt->mInputData &= ~AuthInputAction::START_SNEAKING;

                spdlog::info("Removed sprinting and sneaking flags");

                // Stop the player from sprinting
                player->getMoveInputComponent()->setmIsSprinting(false);
            }
            //ChatUtils::displayClientMessage("Move: " + to_string(pkt->mMove.x) + " | " + to_string(pkt->mMove.y));
            return;
        }
        else if (mDisablerType.mValue == DisablerType::MoveFixV2) {
            if (event.mPacket->getId() != PacketID::PlayerAuthInput) return;
            
            auto pkt = event.getPacket<PlayerAuthInputPacket>();
            glm::vec2 moveVec = pkt->mMove;
            glm::vec2 xzVel = MathUtils::getMotion(player->getActorRotationComponent()->mYaw, 1);
            float yaw = pkt->mRot.y;
            yaw = -yaw;

            if (moveVec.x == 0 && moveVec.y == 0 && xzVel.x == 0 && xzVel.y == 0) return;

            float moveVecYaw = atan2(moveVec.x, moveVec.y);
            moveVecYaw = glm::degrees(moveVecYaw);

            float movementYaw = atan2(xzVel.x, xzVel.y);
            float movementYawDegrees = movementYaw * (180.0f / M_PI);

            float yawDiff = (roundf(movementYawDegrees / 45.f) * 45) - (roundf(yaw / 45.f) * 45);

            float newMoveVecX = sin(glm::radians(yawDiff));
            float newMoveVecY = cos(glm::radians(yawDiff));
            glm::vec2 newMoveVec = { newMoveVecX, newMoveVecY };

            if (abs(newMoveVec.x) < 0.001) newMoveVec.x = 0;
            if (abs(newMoveVec.y) < 0.001) newMoveVec.y = 0;
            if (moveVec.x == 0 && moveVec.y == 0) newMoveVec = { 0, 0 };

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

            // Remove all old flags
            pkt->mInputData &= ~AuthInputAction::UP;
            pkt->mInputData &= ~AuthInputAction::DOWN;
            pkt->mInputData &= ~AuthInputAction::LEFT;
            pkt->mInputData &= ~AuthInputAction::RIGHT;

            bool forward = newMoveVec.y > 0;
            bool backward = newMoveVec.y < 0;
            bool left = newMoveVec.x > 0;
            bool right = newMoveVec.x < 0;
            if (!forward && (backward || left || right) && isSprinting) {
                static bool sprint = false;
                if (!player->isOnGround() && !player->wasOnGround()) sprint = false;
                if (sprint) {
                    pkt->mInputData |= AuthInputAction::SPRINT_DOWN | AuthInputAction::SPRINTING | AuthInputAction::START_SPRINTING;
                    pkt->mInputData &= ~AuthInputAction::STOP_SPRINTING;
                }
                else {
                    pkt->mInputData |= AuthInputAction::STOP_SPRINTING;
                    pkt->mInputData &= ~AuthInputAction::SPRINT_DOWN;
                    pkt->mInputData &= ~AuthInputAction::SPRINTING;
                    pkt->mInputData &= ~AuthInputAction::START_SPRINTING;
                }
                sprint = !sprint;
            }
            else if (forward && isSprinting) {
                pkt->mInputData |= AuthInputAction::SPRINT_DOWN | AuthInputAction::SPRINTING | AuthInputAction::START_SPRINTING;
                pkt->mInputData &= ~AuthInputAction::STOP_SPRINTING;
            }
            if (forward) pkt->mInputData |= AuthInputAction::UP;
            if (backward) pkt->mInputData |= AuthInputAction::DOWN;
            if (left) pkt->mInputData |= AuthInputAction::LEFT;
            if (right) pkt->mInputData |= AuthInputAction::RIGHT;

            pkt->mMove = newMoveVec;
            pkt->mVehicleRotation = newMoveVec; // ???? wtf mojang

            spdlog::info("Forward: {}, Backward: {}, Left: {}, Right: {}", forward ? "true" : "false", backward ? "true" : "false", left ? "true" : "false", right ? "true" : "false");

            //ChatUtils::displayClientMessage("Move: " + to_string(pkt->mMove.x) + " | " + to_string(pkt->mMove.y));
            return;
        }
    }
#endif
    if (mMode.mValue == Mode::Sentinel) {
        if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
            static bool desyncY = false;
            auto pkt = event.getPacket<PlayerAuthInputPacket>();
            if (mShouldUpdateClientTicks) mClientTicks = pkt->mClientTick;
            pkt->mInputData |= AuthInputAction::START_GLIDING | AuthInputAction::JUMPING | AuthInputAction::WANT_UP | AuthInputAction::JUMP_DOWN | AuthInputAction::NORTH_JUMP_DEPRECATED | AuthInputAction::START_JUMPING;
            pkt->mInputData &= ~AuthInputAction::STOP_GLIDING;
            pkt->mInputData |= AuthInputAction::SPRINT_DOWN | AuthInputAction::SPRINTING | AuthInputAction::START_SPRINTING;
            pkt->mInputData &= ~AuthInputAction::STOP_SPRINTING;
            pkt->mInputData &= ~AuthInputAction::SNEAK_DOWN;
            pkt->mInputData &= ~AuthInputAction::SNEAKING;
            pkt->mInputData &= ~AuthInputAction::START_SNEAKING;
            glm::vec3 pos = pkt->mPos;
            glm::vec3 deltaPos = pkt->mPosDelta;
            glm::vec3 lastPos = pkt->mPos - deltaPos;

            // send tampered packet
            pkt->mClientTick = mClientTicks;
            pkt->mPos = { INT_MAX, INT_MAX, INT_MAX };
            ClientInstance::get()->getPacketSender()->sendToServer(pkt);
            mClientTicks++;

            pkt->mClientTick = mClientTicks;
            pkt->mPos = { -INT_MAX, -INT_MAX, -INT_MAX };
            ClientInstance::get()->getPacketSender()->sendToServer(pkt);
            mClientTicks++;

            // send normal packet
            pkt->mClientTick = mClientTicks;
            pkt->mPos = pos;
            mClientTicks++;
        }
        else if (event.mPacket->getId() == PacketID::NetworkStackLatency) {
            spdlog::info("NetworkStackLatency packet cancelled");
        }
        else if (event.mPacket->getId() == PacketID::InventoryTransaction)
        {
            auto packet = event.getPacket<InventoryTransactionPacket>();

            auto* ait = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(packet->mTransaction.get());
            if (ait->mActionType == ItemUseOnActorInventoryTransaction::ActionType::Attack)
            {
                auto actor = ActorUtils::getActorFromUniqueId(ait->mActorId);
                if (!actor) return;
                if (mFirstAttackedActor && actor != mFirstAttackedActor) {
                    event.mCancelled = true;
                    return;
                }
                mFirstAttackedActor = actor;
            }
        }
    }
    else if (mMode.mValue == Mode::Custom)
    {
        if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
            auto packet = event.getPacket<PlayerAuthInputPacket>();
            if (mGlide.mValue) {
                packet->mInputData |= AuthInputAction::START_GLIDING;
                packet->mInputData &= ~AuthInputAction::STOP_GLIDING;
            }
            if (mOnGroundSpoof.mValue) {
                packet->mPosDelta.y = -0.0784000015258789f;
            }
            if (mInputSpoof.mValue) {
                if (packet->mInputMode == InputMode::Mouse) packet->mInputMode = InputMode::Touch;
            }
        }
        else if (event.mPacket->getId() == PacketID::NetworkStackLatency) {
            if (mCancel.mValue)
            {
                spdlog::info("NetworkStackLatency packet cancelled");
                event.mCancelled = true;
            }
        }
        else if (event.mPacket->getId() == PacketID::InventoryTransaction)
        {
            auto packet = event.getPacket<InventoryTransactionPacket>();

            auto* ait = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(packet->mTransaction.get());
            if (ait->mActionType == ItemUseOnActorInventoryTransaction::ActionType::Attack && mInteract.mValue)
            {
                auto actor = ActorUtils::getActorFromUniqueId(ait->mActorId);
                if (!actor) return;
                player->getGameMode()->interact(actor, *actor->getPos());
                //ChatUtils::displayClientMessage("Interacted");
            }

            if (const auto it = event.getPacket<InventoryTransactionPacket>(); it->mTransaction->type ==
                ComplexInventoryTransaction::Type::ItemUseTransaction)
            {
                const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(it->mTransaction.get());
                if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place)
                {
                    transac->mClickPos = BlockUtils::clickPosOffsets    [transac->mFace];
                    for (int i = 0; i < 3; i++)
                    {
                        if (transac->mClickPos[i] == 0.5)
                        {
                            transac->mClickPos[i] = MathUtils::randomFloat(-0.49f, 0.49f);
                        }
                    }
                }
            }
        }
    }
    else if (mMode.mValue == Mode::BDSPrediction) {
        if (event.mPacket->getId() == PacketID::PlayerAuthInput) {
            auto packet = event.getPacket<PlayerAuthInputPacket>();
            packet->mInputData |= AuthInputAction::START_SNEAKING;
            if (!packet->hasInputData(AuthInputAction::SNEAKING)) packet->mInputData |= AuthInputAction::STOP_SNEAKING;
            //packet->mInputData |= AuthInputAction::START_GLIDING;
        }
    }
}

void Disabler::onRunUpdateCycleEvent(RunUpdateCycleEvent& event) {
    if (mMode.mValue != Mode::Sentinel) return;

    if (event.isCancelled() || event.mApplied) return;

    Sleep(101);
    mFirstAttackedActor = nullptr;
}

static constexpr unsigned char newTimestamp[9] = { 0x3, 0x0, 0x0, 0x0, 0x0, 0xFF, 0xFF, 0xFF, 0xFF };

int64_t Disabler::getDelay() const
{
    if (mRandomizeDelay.mValue)
    {
        return MathUtils::random(mMinDelay.mValue, mMaxDelay.mValue);
    }
    return mDelay.mValue;
}

void Disabler::onPingUpdateEvent(PingUpdateEvent& event)
{
    if (mMode.mValue != Mode::Flareon) return;
    if (mDisablerType.mValue == DisablerType::PingSpoof)
    {
        //event.mPing = 0; // hide ping visually
    }
}

void Disabler::onSendImmediateEvent(SendImmediateEvent& event)
{

    if (mMode.mValue != Mode::Flareon) return;
#ifdef __PRIVATE_BUILD__
    if (mDisablerType.mValue == DisablerType::MoveFix) return;
#endif
    uint8_t packetId = event.send[0];
    if (packetId == 0)
    {
        if (mDisablerType.mValue == DisablerType::PingHolder) {
            event.cancel();
            return;
        }

        uint64_t timestamp = *reinterpret_cast<uint64_t*>(&event.send[1]);
        timestamp = _byteswap_uint64(timestamp);
        timestamp -= getDelay();
        uint64_t now = NOW;
        timestamp = _byteswap_uint64(timestamp);
        *reinterpret_cast<uint64_t*>(&event.send[1]) = timestamp;

        event.mModified = true;
    }
}

