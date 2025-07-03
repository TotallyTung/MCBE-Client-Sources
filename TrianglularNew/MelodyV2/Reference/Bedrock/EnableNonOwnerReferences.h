
class Bedrock::EnableNonOwnerReferences { /* Size=0x10 */
  /* 0x0000 */ std::shared_ptr<Bedrock::EnableNonOwnerReferences::ControlBlock> mControlBlock;
  
  EnableNonOwnerReferences(Bedrock::EnableNonOwnerReferences&);
  EnableNonOwnerReferences(const Bedrock::EnableNonOwnerReferences&);
  EnableNonOwnerReferences();
  ~EnableNonOwnerReferences();
  Bedrock::EnableNonOwnerReferences& operator=(const Bedrock::EnableNonOwnerReferences&);
  Bedrock::EnableNonOwnerReferences& operator=(Bedrock::EnableNonOwnerReferences&);
  void __autoclassinit2(uint64_t);
  void* __vecDelDtor(uint32_t);
};
