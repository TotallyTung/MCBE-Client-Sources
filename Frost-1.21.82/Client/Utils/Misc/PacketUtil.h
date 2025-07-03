#pragma once

class PacketUtil {
public:
    static bool spoofSwitch(int slot) {
        auto mPlayer = InstanceManager::get<ClientInstance>()->getLocalPlayer();
        if (!mPlayer) return false;

        auto mItemStack = mPlayer->getSupplies()->getInventory()->getItemStack(slot);
        if (!mItemStack)
        {
            return false;
        }

        auto mep = MinecraftPackets::createPacket(PacketID::MobEquipment);
        auto* pkt = reinterpret_cast<MobEquipmentPacket*>(mep.get());

        pkt->mSlot = slot;
        pkt->mSelectedSlot = slot;
        pkt->mContainerId = 0;
        pkt->mSlotByte = slot;
        pkt->mSelectedSlotByte = slot;
        pkt->mContainerIdByte = 0;

        InstanceManager::get<ClientInstance>()->getLoopbackPacketSender()->sendToServer(pkt);

        return true;
    }

    static void setPlayerAuthInputRotations(Vector2<float> mAngle, PlayerAuthInputPacket* mPacket) {
        if (mPacket != nullptr) {
            mPacket->mRotation.x = mAngle.x;
            mPacket->mRotation.y = mAngle.y;
            mPacket->mYHeadYaw = mAngle.y;
        }
    }
    
    static void setMovePlayerRotations(Vector2<float> mAngle, MovePlayerPacket* mPacket) {
        if (mPacket != nullptr) {
            mPacket->mRot.x = mAngle.x;
            mPacket->mRot.y = mAngle.y;
            mPacket->mYHeadRot = mAngle.y;
        }
    }
};