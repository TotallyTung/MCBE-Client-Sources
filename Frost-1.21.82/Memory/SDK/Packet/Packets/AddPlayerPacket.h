#pragma once

class AddPlayerPacket : public ::AddActorBasePacket {
public:
    std::vector<ActorLink>                 mLinks;
    std::string                            mName;
    mce::UUID                              mUuid;
    uint64_t                               mEntityId;
    ActorRuntimeID                         mRuntimeId;
    std::string                            mPlatformOnlineId;
    Vector3<float>                         mPos;
    Vector3<float>                         mVelocity;
    Vector2<float>                         mRot;
    float                                  mYHeadRot;
    std::vector<std::unique_ptr<DataItem>> mUnpack;
    LayeredAbilities                       mAbilities;
    std::string                            mDeviceId;
    BuildPlatform                          mBuildPlatform;
    GameType                               mPlayerGameType;
    NetworkItemStackDescriptor             mCarriedItem;
    SynchedActorDataEntityWrapper const*   mEntityData;
    PropertySyncData                       mSynchedProperties;
};