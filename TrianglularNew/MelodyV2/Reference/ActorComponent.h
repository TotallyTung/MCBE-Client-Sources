
class ActorComponent : public IEntityComponent { /* Size=0x8 */
  /* 0x0000: fields for IEntityComponent */
  /* 0x0000 */ Actor* mActor;
  
  ActorComponent(ActorComponent&);
  ~ActorComponent();
  ActorComponent& operator=(ActorComponent&);
  void initialize(Actor&);
  Actor& getActor();
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
