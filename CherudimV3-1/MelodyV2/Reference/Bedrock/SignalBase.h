
class Bedrock::SignalBase : public std::enable_shared_from_this<Bedrock::SignalBase> { /* Size=0x38 */
  /* 0x0008: fields for std::enable_shared_from_this<Bedrock::SignalBase> */
  /* 0x0018 */ std::shared_ptr<Bedrock::SignalRouteNode> mNode;
  /* 0x0028 */ uint64_t mCurrentPriority;
  /* 0x0030 */ bool mConsume;
  
  SignalBase(const Bedrock::SignalBase&);
  SignalBase();
  virtual ~SignalBase();
  const uint64_t& getPriority() const;
  virtual bool invokeHandler(const uint16_t&);
  void setNode(std::shared_ptr<Bedrock::SignalRouteNode>);
  void markForConsumption();
  Bedrock::SignalBase& operator=(const Bedrock::SignalBase&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
  
  static void _listenerJoinHandlerMap(Bedrock::SignalReceiver*, std::weak_ptr<Bedrock::ISignalHandlerMap>);
  static void _listenerJoinRoute(Bedrock::SignalReceiver*, std::weak_ptr<Bedrock::SignalRouteBase>);
};
