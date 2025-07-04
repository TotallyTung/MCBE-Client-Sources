
class BinaryStream : public ReadOnlyBinaryStream { /* Size=0x68 */
  /* 0x0000: fields for ReadOnlyBinaryStream */
  /* 0x0040 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> > mOwnedBuffer;
  /* 0x0060 */ std::basic_string<char,std::char_traits<char>,std::allocator<char> >& mBuffer;
  
  BinaryStream(const BinaryStream&);
  BinaryStream(const std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  BinaryStream(std::basic_string<char,std::char_traits<char>,std::allocator<char> >&, bool);
  BinaryStream(std::basic_string<char,std::char_traits<char>,std::allocator<char> >&);
  BinaryStream();
  virtual ~BinaryStream();
  void operator=(const BinaryStream&);
  void reserve(uint64_t);
  void reset();
  std::basic_string<char,std::char_traits<char>,std::allocator<char> > getAndReleaseData();
  void writeBool(bool);
  void writeByte(unsigned char);
  void writeUnsignedShort(uint16_t);
  void writeSignedShort(int16_t);
  void writeUnsignedInt(uint32_t);
  void writeSignedBigEndianInt(int32_t);
  void writeSignedInt(int32_t);
  void writeUnsignedInt64(uint64_t);
  void writeSignedInt64(int64_t);
  void writeUnsignedVarInt(uint32_t);
  void writeUnsignedVarInt64(uint64_t);
  void writeVarInt(int32_t);
  void writeVarInt64(int64_t);
  void writeDouble(double);
  void writeFloat(float);
  void writeFixedFloat(float, double);
  void writeNormalizedFloat(float);
  void writeString(gsl::basic_string_span<char const ,-1>);
  void writeStream(BinaryStream&);
  void writeUnsignedChar(unsigned char);
  void writeSignedBigEndianShort(int16_t);
  void writeByteArray(const char*, uint64_t);
  void write(const void*, uint64_t);
  void __local_vftable_ctor_closure();
  virtual void* __vecDelDtor(uint32_t);
};
