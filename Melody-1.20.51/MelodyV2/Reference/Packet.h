
class Packet { /* Size=0x28 */
  /* 0x0008 */ PacketPriority mPriority;
  /* 0x000c */ NetworkPeer::Reliability mReliability;
  /* 0x0010 */ unsigned char mClientSubId;
  /* 0x0011 */ bool mIsHandled;
  /* 0x0018 */ const IPacketHandlerDispatcher* mHandler;
  /* 0x0020 */ Compressibility mCompressible;
  static const bool SHARE_WITH_HANDLER;
  
  virtual ~Packet();
  Packet(const Packet&);
  Packet(Compressibility);
  Packet();
  virtual MinecraftPacketIds getId() const;
  virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getName() const;
  void writeWithHeader(unsigned char, BinaryStream&) const;
  bool readNoHeader(ReadOnlyBinaryStream&, const unsigned char&, ExtendedStreamReadResult&);
  bool readNoHeader(ReadOnlyBinaryStream&, const unsigned char&, StreamReadResult*);
  virtual void write(BinaryStream&) const;
  virtual StreamReadResult read(ReadOnlyBinaryStream&);
  virtual ExtendedStreamReadResult readExtended(ReadOnlyBinaryStream&);
  virtual bool disallowBatching() const;
  unsigned char getClientSubId() const;
  Compressibility getCompressible() const;
  void setClientSubId(unsigned char);
  PacketPriority getPriority() const;
  NetworkPeer::Reliability getReliability() const;
  bool getIsHandled() const;
  Packet& setReliableOrdered();
  Packet& setReliable();
  Packet& setUnreliableSequenced();
  Packet& setUnreliable();
  Packet& setPriority(PacketPriority);
  void handlePacket() const;
  void handle(const NetworkIdentifier&, NetEventCallback&, std::shared_ptr<Packet>&);
  Packet& operator=(const Packet&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
  
  static void setPacketReliability(int32_t, int32_t);
  static void setCacheSend(bool);
  static bool isCacheSend();
  static void setBatchSendTick(int32_t);
  static int32_t getBatchSendTick();
};
