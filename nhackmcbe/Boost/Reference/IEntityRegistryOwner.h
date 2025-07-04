
class IEntityRegistryOwner { /* Size=0x8 */
  
  virtual OwnerPtrT<EntityRegistryRefTraits>& getEntityRegistry();
  IEntityRegistryOwner(const IEntityRegistryOwner&);
  IEntityRegistryOwner();
  IEntityRegistryOwner& operator=(const IEntityRegistryOwner&);
};
