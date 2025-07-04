
class ItemStackNetManagerScreen { /* Size=0x20 */
  /* 0x0008 */ OwnerPtrT<EntityRefTraits> mEntity;
  
  virtual ~ItemStackNetManagerScreen();
  ItemStackNetManagerScreen(EntityRegistryOwned&);
  const EntityContext& getEntity() const;
  EntityContext& getEntity();
  virtual void* __vecDelDtor(uint32_t);
};
