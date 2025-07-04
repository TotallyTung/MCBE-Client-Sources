
class PlayerAuthInputPacket : public Packet { /* Size=0xb0 */
  /* 0x0000: fields for Packet */
  /* 0x0028 */ Vec2 mRot;
  /* 0x0030 */ Vec3 mPos;
  /* 0x003c */ float mYHeadRot;
  /* 0x0040 */ Vec3 mPosDelta;
  /* 0x004c */ bool mCameraDeparted;
  /* 0x004d */ bool mThirdPersonPerspective;
  /* 0x0050 */ Vec2 mPlayerRotationToCamera;
  /* 0x0058 */ Vec2 mMove;
  /* 0x0060 */ Vec3 mGazeDir;
  /* 0x0070 */ std::bitset<37> mInputData;
  /* 0x0078 */ InputMode mInputMode;
  /* 0x007c */ ClientPlayMode mPlayMode;
  /* 0x0080 */ uint64_t mClientTick;
  /* 0x0088 */ std::unique_ptr<PackedItemUseLegacyInventoryTransaction,std::default_delete<PackedItemUseLegacyInventoryTransaction> > mItemUseTransaction;
  /* 0x0090 */ std::unique_ptr<ItemStackRequestData,std::default_delete<ItemStackRequestData> > mItemStackRequest;
  /* 0x0098 */ PlayerBlockActions mPlayerBlockActions;
  
  PlayerAuthInputPacket(PlayerAuthInputPacket&);
  PlayerAuthInputPacket();
  virtual ~PlayerAuthInputPacket();
  PlayerAuthInputPacket& operator=(PlayerAuthInputPacket&);
  PlayerAuthInputPacket takeCopy() const;
  virtual MinecraftPacketIds getId() const;
  virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getName() const;
  virtual void write(BinaryStream&) const;
  virtual StreamReadResult read(ReadOnlyBinaryStream&);
  bool getInput(PlayerAuthInputPacket::InputData) const;
  void setInput(PlayerAuthInputPacket::InputData, bool);
  void setFromComponent(PlayerActionComponent&);
  PlayerActionComponent readIntoComponent() const;
  std::basic_string<char,std::char_traits<char>,std::allocator<char> > getPacketDebugText(const char*) const;
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
