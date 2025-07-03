
class BlockContainerScreenController : public ContainerScreenController { /* Size=0xea8 */
  /* 0x0000: fields for ContainerScreenController */
  /* 0x0e98 */ std::shared_ptr<ContainerManagerController> mBlockContainerManagerController;
  
  BlockContainerScreenController(std::shared_ptr<ClientInstanceScreenModel>, Player&, const BlockPos&, const ActorUniqueID);
  virtual ~BlockContainerScreenController();
  virtual bool _isStillValid() const;
  virtual void _registerCoalesceOrder();
  virtual void _registerAutoPlaceOrder();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
