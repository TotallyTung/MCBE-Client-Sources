
class TextPacket : public Packet { /* Size=0xd0 */
  /* 0x0000: fields for Packet */
  /* 0x0028 */ TextPacketType mType;
  /* 0x0030 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mAuthor;
  /* 0x0050 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mMessage;
  /* 0x0070 */ std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > > params;
  /* 0x0088 */ bool mLocalize;
  /* 0x0090 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mXuid;
  /* 0x00b0 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mPlatformId;
  
  TextPacket(const TextPacket&);
  TextPacket(TextPacketType, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >&, bool, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  TextPacket();
  virtual ~TextPacket();
  virtual MinecraftPacketIds getId() const;
  virtual std::basic_string<char,std::char_traits<char>,std::allocator<char> > getName() const;
  virtual void write(BinaryStream&) const;
  virtual StreamReadResult read(ReadOnlyBinaryStream&);
  TextPacket& operator=(const TextPacket&);
  void __autoclassinit2(uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
  
  static TextPacket createRaw(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createChat(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >&);
  static TextPacket createChat(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createTranslatedChat(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createTranslated(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >&);
  static TextPacket createTextObjectMessage(const ResolvedTextObject&);
  static TextPacket createTextObjectWhisperMessage(const ResolvedTextObject&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createSystemMessage(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createWhisper(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createAnnouncement(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createTranslatedAnnouncement(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  static TextPacket createJukeboxPopup(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, const std::vector<std::basic_string<char,std::char_traits<char>,std::allocator<char> >,std::allocator<std::basic_string<char,std::char_traits<char>,std::allocator<char> > > >&);
};
