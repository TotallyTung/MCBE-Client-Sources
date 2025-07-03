class ItemStackNetManagerBase {
  protected:
    const bool mIsEnabled;
    const bool mIsClientSide;
    Player &mPlayer;
    WeakRef<EntityRegistry> mLevelWeakRegistry;
  private:
    OwnerPtr<EntityRegistry> mEntityRegistry;
    std::unique_ptr<ItemStackNetManagerScreenStack, std::default_delete<ItemStackNetManagerScreenStack> > mScreenStack;
  protected:
    bool mCurrentRequestHasCraftAction;
    ItemStackLegacyRequestId mLegacyTransactionRequestId;

  public:
    ItemStackNetManagerBase(Player &, bool, bool);
    virtual ~ItemStackNetManagerBase(void);
    bool isClientSide(void) const;
    bool isScreenOpen(void) const;
    virtual bool isEnabled(void) const;
    virtual ItemStackRequestId getRequestId(void) const;
    virtual bool retainSetItemStackNetIdVariant(void) const;
    virtual bool allowInventoryTransactionManager(void) const;
    static LegacyRequestScope _tryBeginClientLegacyTransactionRequest(Player *);
  protected:
    virtual LegacyRequestScope _tryBeginClientLegacyTransactionRequest(void);
  public:
    virtual void onContainerScreenOpen(const ContainerScreenContext &);
    virtual void onContainerScreenClose(void);
    virtual SparseContainer * initOpenContainer(BlockSource &, ContainerEnumName, const ContainerWeakRef &);
    const ContainerScreenContext & getScreenContext(void) const;
    static bool setPlayerContainer(Player &, ContainerType, int, const ItemStack &, ItemStack &, const BaseContainerSetter &);
    ItemStackNetManagerScreenStack & _getScreenStack(void);
    const ItemStackNetManagerScreenStack & _getScreenStack(void) const;
  protected:
    bool _isRequestActionAllowed(const ItemStackRequestAction &);
    virtual void _addLegacyTransactionRequestSetItemSlot(ItemStackNetManagerScreen &, ContainerType, int);
    std::__cxx11::string _debugContext(std::__cxx11::string) const;
    virtual void _initScreen(ItemStackNetManagerScreen &);
    ItemStackNetManagerScreen * _getTopScreen(void);
  private:
    void _pushScreen(ContainerScreenContext);

    typedef gsl::final_action<std::function<void ()> > LegacyRequestScope;
    typedef std::function<void (const ItemStack &)> BaseContainerSetter;
}
