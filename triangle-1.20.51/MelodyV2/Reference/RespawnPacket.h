
class RespawnPacket : public Packet { /* Size=0x40 */
  /* 0x0000: fields for Packet */
  /* 0x0028 */ Vec3 mPos;
  /* 0x0034 */ PlayerRespawnState mState;
  /* 0x0038 */ ActorRuntimeID mRuntimeId;
  
  RespawnPacket(RespawnPacket&);
  RespawnPacket(const RespawnPacket&);
  RespawnPacket(const Vec3&, const PlayerRespawnState&, ActorRuntimeID);
  RespawnPacket(const Vec3&, const PlayerRespawnState&);
  RespawnPacket();
  virtual MinecraftPacketIds getId() const;
  virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getName() const;
  virtual void write(BinaryStream&) const;
  virtual StreamReadResult read(ReadOnlyBinaryStream&);
  virtual ~RespawnPacket();
  RespawnPacket& operator=(RespawnPacket&);
  RespawnPacket& operator=(const RespawnPacket&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
