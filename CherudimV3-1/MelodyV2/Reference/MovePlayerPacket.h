
class MovePlayerPacket : public Packet { /* Size=0x68 */
  /* 0x0000: fields for Packet */
  /* 0x0028 */ ActorRuntimeID mPlayerID;
  /* 0x0030 */ Vec3 mPos;
  /* 0x003c */ Vec2 mRot;
  /* 0x0044 */ float mYHeadRot;
  /* 0x0048 */ Player::PositionMode mResetPosition;
  /* 0x0049 */ bool mOnGround;
  /* 0x0050 */ ActorRuntimeID mRidingID;
  /* 0x0058 */ int32_t mCause;
  /* 0x005c */ int32_t mSourceEntityType;
  /* 0x0060 */ uint64_t mTick;
  
  MovePlayerPacket(MovePlayerPacket&);
  MovePlayerPacket(const MovePlayerPacket&);
  MovePlayerPacket(Player&, const Vec3&);
  MovePlayerPacket(Player&, Player::PositionMode, int32_t, int32_t);
  MovePlayerPacket();
  virtual MinecraftPacketIds getId() const;
  virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getName() const;
  virtual void write(BinaryStream&) const;
  virtual StreamReadResult read(ReadOnlyBinaryStream&);
  virtual ~MovePlayerPacket();
  MovePlayerPacket& operator=(MovePlayerPacket&);
  MovePlayerPacket& operator=(const MovePlayerPacket&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
