
class Bedrock::SignalReceiver { /* Size=0x58 */
  /* 0x0008 */ std::vector<moodycamel::ConcurrentQueue<std::shared_ptr<Bedrock::SignalBase>,moodycamel::ConcurrentQueueDefaultTraits>,std::allocator<moodycamel::ConcurrentQueue<std::shared_ptr<Bedrock::SignalBase>,moodycamel::ConcurrentQueueDefaultTraits> > > mSignalQueues;
  /* 0x0020 */ std::vector<std::weak_ptr<Bedrock::ISignalHandlerMap>,std::allocator<std::weak_ptr<Bedrock::ISignalHandlerMap> > > mHandlerMapVector;
  /* 0x0038 */ std::vector<std::weak_ptr<Bedrock::SignalRouteBase>,std::allocator<std::weak_ptr<Bedrock::SignalRouteBase> > > mRouteVector;
  /* 0x0050 */ uint16_t mReceiverId;
  static const uint64_t kNumSignalQueues;
  
  SignalReceiver(const Bedrock::SignalReceiver&);
  SignalReceiver(const uint16_t&);
  virtual ~SignalReceiver();
  const uint16_t& getReceiverId() const;
  bool handleNextSignal();
  void queueSignal(std::shared_ptr<Bedrock::SignalBase>, std::shared_ptr<Bedrock::SignalRouteNode>);
  void joinHandlerMap(std::weak_ptr<Bedrock::ISignalHandlerMap>);
  bool leaveHandlerMap(std::weak_ptr<Bedrock::ISignalHandlerMap>);
  void joinRoute(std::weak_ptr<Bedrock::SignalRouteBase>);
  bool leaveRoute(std::weak_ptr<Bedrock::SignalRouteBase>);
  std::shared_ptr<Bedrock::SignalBase> acquireNextSignal(const uint64_t);
  bool handleSignal(std::shared_ptr<Bedrock::SignalBase>);
  bool _handleNextSignal(const uint64_t);
  Bedrock::SignalReceiver& operator=(const Bedrock::SignalReceiver&);
  void __autoclassinit2(uint64_t);
  virtual void* __vecDelDtor(uint32_t);
};
