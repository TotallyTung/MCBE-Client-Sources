#pragma once

class Disabler : public Module
{
public:
    Disabler() :
        Module("Disabler", "Misc", "Disables Anti Cheat checks by manipulating packets and immediate sending.")
    {
        EnumSetting("Mode", "The Anti-Cheat mode to disable flags based on shown booleans under.", { "Sentinel", "Flareon", "Flareon V2", "Cancel" }, &mMode);
        BoolSetting("Duplicate", "Duplicate sending packets.", &mDuplicate, [this] { return mMode == 0; });
        BoolSetting("Ping Holder", "Edits Send Immediate packets.", &mSendImmediate, [this] { return mMode == 1 || mMode == 2; });
        BoolSetting("Movement Packets", "Fixes and edits Movement packets in PlayerAuthInput being sent to server.", &mMovementFix, [this] { return mMode == 2; });
        BoolSetting("Interact", "Sends interact packets before attacking.", &mInteract, [this] { return mMode == 1 || mMode == 2; });
        BoolSetting("Click Pos", "Fixes and edits ClickPos in transactions packets.", &mClickPosFix, [this] { return mMode == 2; });
        BoolSetting("Cancel", "Cancels your NetworkStackLatency packet (Cancel disabler mode).", &mCancel, [this] { return mMode == 2; });
    }

private:
    int mMode = 0;

    bool mDuplicate = false;
    bool mSendImmediate = false;
    bool mMovementFix = false;
    bool mInteract = false;
    bool mClickPosFix = false;
    bool mCancel = false;

    enum Mode {
        Sentinel = 0,
        FlareonV1 = 1,
        FlareonV2 = 2,
        CancelM = 3,
    };
public:
    void onEnabled() override 
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;
    }

    void onEvent(BaseTickEvent* event)
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return;
    }

    void onEvent(PacketEvent* event)
    {
        Player* mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        static int setTick = 0;
        static bool ignore = false;

        static Vector3<float> savedPosition = Vector3<float>(0, 0, 0);
        static Vector3<float> oldPosition = Vector3<float>(0, 0, 0);
        static bool firstRun = true;

        if (event->mPacket->getId() == PacketID::MovePlayer) {
            static std::string 🤩 = "You have succefully sent MovePlayerPacket";
            ChatUtil::sendMessage(🤩);
        }

        if (event->mPacket->getId() == PacketID::PlayerAuthInput)
        {
            auto* pkt = reinterpret_cast<PlayerAuthInputPacket*>(event->mPacket);

            if (mMode == 0 && mDuplicate)
            {
                if (ignore)
                {
                    ignore = false;
                    return;
                }

                if (firstRun)
                {
                    firstRun = false;
                    setTick = pkt->mClientTick.mInputTick;
                    oldPosition = pkt->mPosition;
                }
                else
                {
                    setTick++;
                }

                savedPosition = pkt->mPosition;

                auto calculateNewPosition = [](const Vector3<float>& savedPos, const Vector3<float>& oldPos) -> Vector3<float> {
                    Vector3<float> averagedPosition = savedPos.average(oldPos);
                    return (averagedPosition.distance(oldPos) > 3) ? savedPos : averagedPosition;
                    };

                Vector3<float> newPosition = calculateNewPosition(savedPosition, oldPosition);

                pkt->mClientTick.mInputTick = setTick;
                pkt->mPosition = newPosition;

                std::shared_ptr<Packet> packet = MinecraftPackets::createPacket(PacketID::PlayerAuthInput);

                auto* duplicatePkt = reinterpret_cast<PlayerAuthInputPacket*>(packet.get());
                duplicatePkt = pkt;
                duplicatePkt->mClientTick.mInputTick = ++setTick;
                duplicatePkt->mPosition = savedPosition;
                ignore = true;

                auto sendPacketToServer = [](PlayerAuthInputPacket* packet) {
                    InstanceManager::get<ClientInstance>()->getLoopbackPacketSender()->sendToServer(packet);
                    };

                sendPacketToServer(duplicatePkt);

                oldPosition = savedPosition;
            }

            if (mMode == 2 && mMovementFix) {
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
                return;
            }
        }

        if (event->mPacket->getId() == PacketID::InventoryTransaction)
        {
            auto* pkt = reinterpret_cast<InventoryTransactionPacket*>(event->mPacket);

            auto* itemUseOnActorTrans = reinterpret_cast<ItemUseOnActorInventoryTransaction*>(pkt->mTransaction.get());

            if (itemUseOnActorTrans->mActionType == ItemUseOnActorInventoryTransaction::ActionType::Attack && mInteract && mMode == 1 || mMode == 2) {
                // add interaction here or make it interact on killaura before attacking when interact is enabled here
            }

            if (pkt->mTransaction->mType == ComplexInventoryTransaction::Type::ItemUseTransaction)
            {
                const auto transac = reinterpret_cast<ItemUseInventoryTransaction*>(pkt->mTransaction.get());
                if (transac->mActionType == ItemUseInventoryTransaction::ActionType::Place && mClickPosFix && mMode == 2)
                {
                    if (transac->mFace == 0) // Down
                    {
                        transac->mClickPos = Vector3<float>(0.5, -0, 0.5);
                    }
                    else if (transac->mFace == 1) // Up
                    {
                        transac->mClickPos = Vector3<float>(0.5, 1, 0.5);
                    }
                    else if (transac->mFace == 2) // North
                    {
                        transac->mClickPos = Vector3<float>(0.5, 0.5, 0);
                    }
                    else if (transac->mFace == 3) // South
                    {
                        transac->mClickPos = Vector3<float>(0.5, 0.5, 1);
                    }
                    else if (transac->mFace == 4) // West
                    {
                        transac->mClickPos = Vector3<float>(0, 0.5, 0.5);
                    }
                    else if (transac->mFace == 5) // East
                    {
                        transac->mClickPos = Vector3<float>(1, 0.5, 0.5);
                    }
                    //ChatUtils::displayClientMessage("Fixed Click Pos");
                }
            }
        }

        if (event->mPacket->getId() == PacketID::NetworkStackLatency)
        {
            auto* pkt = reinterpret_cast<NetworkStackLatencyPacket*>(event->mPacket);

            if (mMode == 2 && mCancel) {
                return;
            }

            if (mMode == 3) {
                return;
            }
        }

        
    }
};