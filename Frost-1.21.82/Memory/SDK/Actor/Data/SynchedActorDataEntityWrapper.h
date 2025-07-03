#pragma once

struct SynchedActorDataComponent;
struct ActorDataFlagComponent;
struct ActorDataDirtyFlagsComponent;
class EntityContext;

class SynchedActorDataEntityWrapper {
public:
    /*gsl::not_null<SynchedActorDataComponent*>    mSynchedActorData;
    gsl::not_null<ActorDataFlagComponent*>       mActorDataFlag;
    gsl::not_null<ActorDataDirtyFlagsComponent*> mActorDataDirtyFlag;*/
    SynchedActorDataComponent*                   mSynchedActorData;
    ActorDataFlagComponent*                      mActorDataFlag;
    ActorDataDirtyFlagsComponent*                mActorDataDirtyFlag;
    WeakRef<EntityContext>                       mWeakEntity;
};