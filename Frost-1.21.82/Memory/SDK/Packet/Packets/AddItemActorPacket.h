#pragma once

class AddItemActorPacket : public ::AddActorBasePacket {
public:
    std::vector<std::unique_ptr<DataItem>> mData;
    SynchedActorDataEntityWrapper*         mEntityData;
    uint64_t                               mId;
    ActorRuntimeID                         mRuntimeId;
    NetworkItemStackDescriptor             mItem;
    Vector3<float>                         mPos;
    Vector3<float>                         mVelocity;
    bool                                   mIsFromFishing;
};