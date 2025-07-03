class SynchedActorData {
  using DataList = std::vector<std::unique_ptr<DataItem, std::default_delete<DataItem> >, std::allocator<std::unique_ptr<DataItem, std::default_delete<DataItem>>>>;

  public:
    static TypeInt FLAG_LIMIT;
    static TypeInt MAX_STRING_DATA_LENGTH;
    static TypeInt EOF_MARKER;
    static const std::__cxx11::string DIFF_ITEMS_SIZE;
    static const std::__cxx11::string DIFF_ITEM_NULL;
    static const std::__cxx11::string DIFF_ITEM;
  private:
    DataList mItemsArray;
    ID minIdxDirty;
    ID maxIdxDirty;
    static TypeInt TYPE_MASK;
    static TypeInt TYPE_SHIFT;
    static TypeInt MAX_ID_VALUE;

  public:
    SynchedActorData(void);
    SynchedActorData(const SynchedActorData &);
    SynchedActorData(SynchedActorData &&);
    virtual ~SynchedActorData(void);
    SynchedActorData & operator=(const SynchedActorData &);
    SynchedActorData & operator=(SynchedActorData &&);
    bool operator==(const SynchedActorData &) const;
    static std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > > getDiff(const SynchedActorData &, const SynchedActorData &);
    TypeInt8 getInt8(ID) const;
    TypeShort getShort(ID) const;
    TypeInt getInt(ID) const;
    TypeInt64 getInt64(ID) const;
    TypeFloat getFloat(ID) const;
    const std::__cxx11::string & getString(ID) const;
    const CompoundTag & getCompoundTag(ID) const;
    BlockPos getPosition(ID) const;
    TypeVec3 getVec3(ID) const;
    bool hasData(ID) const;
    void markDirty(DataItem &);
    void markDirty(ID);
    bool isDirty(void) const;
    virtual bool getStatusFlag(ActorFlags) const;
    virtual void setStatusFlag(ActorFlags, bool);
    DataList packDirty(void);
    DataList packAll(void) const;
    void forEachDataItem(const std::function<void (const std::unique_ptr<DataItem, std::default_delete<DataItem> > &)> &) const;
    std::size_t getValidDataItemCount(void) const;
    bool _assignValues(const SynchedActorData &, Actor *);
    bool assignValues(const DataList &, Actor *);
    SynchedActorData _clone(void) const;
  private:
    void _setDirty(ID);
    DataItem & _get(ID);
    DataItem * _find(ID) const;
    void _resizeToContain(ID);
    static ActorDataIDs _getIDForFlag(ActorFlags);
  public:
    bool getFlag<long>(ID, TypeInt) const;
    void setFlag<long>(ID, TypeInt);
    void clearFlag<long>(ID, TypeInt);
    bool set<signed char>(DataItem *, const TypeInt8 &);
    bool set<short>(DataItem *, const TypeShort &);
    bool set<int>(DataItem *, const TypeInt &);
    bool set<float>(DataItem *, const TypeFloat &);
    bool set<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >(DataItem *, const std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > &);
    bool set<CompoundTag>(DataItem *, const CompoundTag &);
    bool set<BlockPos>(DataItem *, const BlockPos &);
    bool set<long>(DataItem *, const TypeInt64 &);
    bool set<Vec3>(DataItem *, const TypeVec3 &);
    static std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > & dataItemCast<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >(DataItem &);
    static CompoundTag & dataItemCast<CompoundTag>(DataItem &);

  private:
    typedef std::vector<std::unique_ptr<DataItem, std::default_delete<DataItem> >, std::allocator<std::unique_ptr<DataItem, std::default_delete<DataItem> > > > DataList;
    typedef DataItem::ID ID;
    typedef int8_t TypeInt8;
    typedef short TypeShort;
    typedef int TypeInt;
    typedef int64_t TypeInt64;
    typedef float TypeFloat;
    typedef Vec3 TypeVec3;
}
