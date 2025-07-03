class IContainerManager {
  public:
    IContainerManager(void);
    virtual ~IContainerManager(void);
    virtual ContainerID getContainerId(void) const;
    virtual void setContainerId(ContainerID);
    virtual ContainerType getContainerType(void) const;
    virtual void setContainerType(ContainerType);
    virtual void serverInitItemStackIds(void);
    virtual std::vector<ItemStack, std::allocator<ItemStack> > getItemCopies(void) const;
    virtual void setSlot(int, const ItemStack &, bool);
    virtual const ItemStack & getSlot(int) const;
    virtual void setData(int, int);
    virtual void broadcastChanges(void);
}
