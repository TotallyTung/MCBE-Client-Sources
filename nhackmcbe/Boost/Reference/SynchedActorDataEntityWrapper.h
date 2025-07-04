class SynchedActorDataEntityWrapper {
  private:
    gsl::not_null<SynchedActorDataComponent*> mData;

  public:
    SynchedActorDataEntityWrapper(EntityContext &);
    virtual ~SynchedActorDataEntityWrapper(void);
    SynchedActorDataEntityWrapper & operator=(const SynchedActorDataEntityWrapper &);
    SynchedActorDataEntityWrapper & operator=(SynchedActorDataEntityWrapper &&);
    SynchedActorData::TypeInt8 getInt8(SynchedActorData::ID) const;
    SynchedActorData::TypeShort getShort(SynchedActorData::ID) const;
    SynchedActorData::TypeInt getInt(SynchedActorData::ID) const;
    SynchedActorData::TypeInt64 getInt64(SynchedActorData::ID) const;
    SynchedActorData::TypeFloat getFloat(SynchedActorData::ID) const;
    const std::__cxx11::string & getString(SynchedActorData::ID) const;
    const CompoundTag & getCompoundTag(SynchedActorData::ID) const;
    BlockPos getPosition(SynchedActorData::ID) const;
    SynchedActorData::TypeVec3 getVec3(SynchedActorData::ID) const;
    bool hasData(SynchedActorData::ID) const;
    virtual bool getStatusFlag(ActorFlags) const;
    virtual void setStatusFlag(ActorFlags, bool);
    void markDirty(DataItem &);
    void markDirty(SynchedActorData::ID);
    bool isDirty(void) const;
    SynchedActorData::DataList packDirty(void);
    SynchedActorData::DataList packAll(void) const;
    void forEachDataItem(const std::function<void (const std::unique_ptr<DataItem, std::default_delete<DataItem> > &)> &) const;
    std::size_t getValidDataItemCount(void) const;
    bool _assignValues(const SynchedActorDataEntityWrapper &, Actor *);
    bool assignValues(const SynchedActorData::DataList &, Actor *);
    SynchedActorDataComponent _clone(void) const;
    SynchedActorDataReader reader(void) const;
    SynchedActorDataWriter writer(void);
  private:
    gsl::not_null<SynchedActorData*> _get(void);
    gsl::not_null<SynchedActorData const*> _get(void) const;
}
