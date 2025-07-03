struct SynchedActorDataComponent {
    SynchedActorData mData;
  public:
    SynchedActorDataComponent(void);
    SynchedActorDataComponent(SynchedActorDataComponent &&);
    SynchedActorDataComponent(const SynchedActorDataComponent &);
    SynchedActorDataComponent(SynchedActorData &&);
    SynchedActorDataComponent(const SynchedActorData &);
    SynchedActorDataComponent & operator=(SynchedActorDataComponent &&);
    SynchedActorDataComponent & operator=(const SynchedActorDataComponent &);
}
