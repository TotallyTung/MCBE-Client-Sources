class ItemStack : public ItemStackBase {
  public:
    static const ItemStack EMPTY_ITEM;
  private:
    ItemStackNetIdVariant mNetIdVariant;

  public:
    ItemStack(void);
    ItemStack(const BlockLegacy &, int);
    ItemStack(const Block &, int, const CompoundTag *);
    ItemStack(const ItemInstance &);
    ItemStack(const RecipeIngredient &);
    ItemStack(string_span, int, int, const CompoundTag *);
    ItemStack(const Item &, int, int, const CompoundTag *);
    ItemStack(const ItemStack &);
    ItemStack & operator=(const ItemStack &);
    virtual void reinit(const BlockLegacy &, int);
    virtual void reinit(const Item &, int, int);
    virtual void reinit(const string_span, int, int);
    float getDestroySpeed(const Block &) const;
    bool useOn(Actor &, int, int, int, FacingID, const Vec3 &);
    ItemStack & use(Player &);
    bool inventoryTick(Level &, Actor &, int, bool);
    void mineBlock(const Block &, int, int, int, Mob *);
    bool sameItemAndAuxAndBlockData(const ItemStack &) const;
    ItemUseMethod useTimeDepleted(Level *, Player *);
    ItemStack clone(void) const;
    ItemStack getStrippedNetworkItem(void) const;
    void useAsFuel(void);
    static ItemStack fromTag(const CompoundTag &);
    static ItemStack fromTag(const CompoundTag &, Level &);
    static ItemStack fromDescriptor(const NetworkItemStackDescriptor &, BlockPalette &, bool);
    void releaseUsing(Player *, int);
    int getMaxUseDuration(void) const;
    void playSoundIncrementally(Mob &) const;
    const ItemStackNetIdVariant & getItemStackNetIdVariant(void) const;
    bool hasItemStackNetId(void) const;
    bool hasClientRequestId(void) const;
    bool hasLegacyClientRequestId(void) const;
    const ItemStackNetId * tryGetItemStackNetId(void) const;
    const ItemStackRequestId * tryGetItemStackRequestId(void) const;
    const ItemStackLegacyRequestId * tryGetLegacyRequestId(void) const;
    void serverInitNetId(void);
    void clientInitNetId(const ItemStackNetId &);
    void serverInitRequestId(const ItemStackRequestId &);
    void clientInitRequestId(const ItemStackRequestId &);
    void clientInitLegacyRequestId(const ItemStackLegacyRequestId &);
    bool matchesNetIdVariant(const ItemStack &) const;
    bool matchesAndNetIdVariantMatches(const ItemStack &) const;
    virtual void setNull(std::optional<std::__cxx11::basic_string<char, std::char_traits<char>, std::allocator<char> > >);
    void _assignNetIdVariant(const ItemStack &) const;
    Brightness getLightEmission(void) const;
    virtual std::__cxx11::string toString(void) const;
    virtual std::__cxx11::string toDebugString(void) const;
}
