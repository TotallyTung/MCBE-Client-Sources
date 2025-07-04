
class ChestScreenController : public BlockContainerScreenController { /* Size=0xea8 */
  /* 0x0000: fields for BlockContainerScreenController */
  
  ChestScreenController(std::shared_ptr<ClientInstanceScreenModel>, Player&, const BlockPos&, const ActorUniqueID, BlockActorType);
  virtual ~ChestScreenController();
  virtual void _registerCoalesceOrder();
  virtual void _registerAutoPlaceOrder();
  void _registerBindings();
  std::shared_ptr<LevelContainerManagerModel> _getLevelContainerManagerModel();
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
