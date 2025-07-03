#pragma once

class AddActorBasePacket : public ::Packet {
public:

};

class AddActorPacket : public ::AddActorBasePacket {
public:
    std::vector<ActorLink>                 mLinks;
    Vector3<float>                         mPos;
    Vector3<float>                         mVelocity;
    Vector2<float>                         mRot;
    float                                  mYHeadRotation;
    float                                  mYBodyRotation;
    uint64_t                               mEntityId;
    ActorRuntimeID                         mRuntimeId;
    SynchedActorDataEntityWrapper*         mEntityData;
    std::vector<std::unique_ptr<DataItem>> mData;
    ActorDefinitionIdentifier              mType;
    PropertySyncData                       mSynchedProperties;
    std::vector<AttributeInstanceHandle>   mAttributeHandles;
    std::vector<SyncedAttribute>           mAttributes;
};