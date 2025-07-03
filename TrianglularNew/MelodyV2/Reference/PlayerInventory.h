class PlayerInventory : public ContainerSizeChangeListener, private ContainerContentChangeListener {
  private:
    int mSelected;
    ItemStack mInfiniteItem;
    ContainerID mSelectedContainerId;
    std::unique_ptr<Inventory, std::default_delete<Inventory> > mInventory;
    std::vector<ItemStack, std::allocator<ItemStack> > mComplexItems;
    std::weak_ptr<HudContainerManagerModel> mHudContainerManager;

  public:
    PlayerInventory(std::unique_ptr<Inventory, std::default_delete<Inventory> >);
    virtual ~PlayerInventory(void);
    void init(std::weak_ptr<HudContainerManagerModel>);
    void serverInitItemStackIds(int, int, Container::ItemStackNetIdChangedCallback);
    void serverInitItemStackId(int);
    void addListener(ContainerContentChangeListener *);
    void removeListener(ContainerContentChangeListener *);
    void setContainerChanged(int);
    virtual void containerSizeChanged(int);
    virtual void containerContentChanged(int);
    bool add(ItemStack &, bool);
    bool canAdd(const ItemStack &) const;
    int getSlotWithItem(const ItemStack &, bool, bool) const;
    int getFirstEmptySlot(void) const;
    int getEmptySlotsCount(void) const;
    std::vector<ItemStack const*, std::allocator<ItemStack const*> > getSlots(void) const;
    std::vector<ItemStack, std::allocator<ItemStack> > getSlotCopies(ContainerID) const;
    const std::vector<ItemStack, std::allocator<ItemStack> > & getComplexItems(ContainerID) const;
    void setContainerSize(int, ContainerID);
    int getContainerSize(ContainerID) const;
    int getHotbarSize(void) const;
    int getItemCount(const ItemDescriptor &);
    int getItemCount(const std::function<bool (const ItemStack &)>);
    bool hasResource(int);
    bool getAndRemoveResource(ItemStack &, bool, bool);
    bool removeResource(int);
    bool removeResource(const ItemStack &);
    int removeResource(const ItemStack &, bool, bool, int);
    void swapSlots(int, int);
    void clearSlot(int, ContainerID);
    int clearInventory(int);
    void clearInventoryWithDefault(bool);
    void clearVanishEnchantedItemsOnDeath(void);
    void load(const ListTag &, const SemVersion &, Level &);
    std::unique_ptr<ListTag, std::default_delete<ListTag> > save(void);
    bool dropSlot(int, bool, bool, ContainerID, bool);
    void dropAllOnDeath(bool);
    void tick(void);
    void setItem(int, const ItemStack &, ContainerID, bool);
    const ItemStack & getItem(int, ContainerID) const;
    void removeItem(int, int, ContainerID);
    PlayerInventory::SlotData getSelectedSlot(void) const;
    ContainerID getSelectedContainerId(void);
    bool selectSlot(int, ContainerID);
    const ItemStack & getSelectedItem(void);
    void setSelectedItem(const ItemStack &);
    const std::vector<ContainerID, std::allocator<ContainerID> > & getAllContainerIds(void);
    const Container & getContainer(void) const;
    Container & getContainer(void);
    std::weak_ptr<HudContainerManagerModel> _getHudContainerManagerModel(void);
    virtual void createTransactionContext(std::function<void (Container &, int, const ItemStack &, const ItemStack &)>, std::function<void ()>);
    void setupDefaultInventory(void);
  private:
    std::shared_ptr<HudContainerManagerModel> _getHudContainer(void) const;
}
