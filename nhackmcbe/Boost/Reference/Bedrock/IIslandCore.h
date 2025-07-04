
class Bedrock::IIslandCore { /* Size=0x8 */
  
  virtual ~IIslandCore();
  virtual uint16_t getId();
  virtual bool start();
  virtual bool suspend();
  virtual bool resume();
  virtual bool stop();
  virtual void mainUpdate();
  virtual void processActivationArguments(const Bedrock::ActivationArguments&);
  IIslandCore(const Bedrock::IIslandCore&);
  IIslandCore();
  Bedrock::IIslandCore& operator=(const Bedrock::IIslandCore&);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
