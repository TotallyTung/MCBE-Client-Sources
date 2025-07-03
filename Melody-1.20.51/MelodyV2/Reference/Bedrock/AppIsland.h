
class Bedrock::AppIsland : public Bedrock::IIslandCore { /* Size=0x10 */
  /* 0x0000: fields for Bedrock::IIslandCore */
  /* 0x0008 */ std::unique_ptr<Bedrock::SignalReceiver,std::default_delete<Bedrock::SignalReceiver> > mSignalRcvr;
  
  AppIsland();
  virtual ~AppIsland();
  virtual uint16_t getId();
  virtual bool start();
  virtual bool suspend();
  virtual bool resume();
  virtual bool stop();
  virtual void mainUpdate();
  virtual void processActivationArguments(const Bedrock::ActivationArguments&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
