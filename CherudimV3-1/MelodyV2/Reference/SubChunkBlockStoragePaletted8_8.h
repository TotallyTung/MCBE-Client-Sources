
class SubChunkBlockStoragePaletted<8,8> : public ISubChunkBlockStoragePaletted { /* Size=0x1838 */
  /* 0x0000: fields for ISubChunkBlockStoragePaletted */
  /* 0x0008 */ uint32_t mBlocks[1024];
  /* 0x1008 */ AppendOnlyAtomicLookupTable<Block const *,256> mPalette;
  /* 0x1830 */ bool mModDataFlag;
  static const uint64_t CAPACITY;
  static const uint64_t BLOCKS_PER_WORD;
  static const uint64_t BLOCK_MASK;
  
  SubChunkBlockStoragePaletted<8,8>(const BlockVolume&, int16_t, const gsl::span<Block const *,-1>&);
  SubChunkBlockStoragePaletted<8,8>(const Block&);
  SubChunkBlockStoragePaletted<8,8>(const SubChunkBlockStorage&);
  SubChunkBlockStoragePaletted<8,8>(IDataInput&, const BlockPalette&, bool);
  virtual bool isUniform(const Block&) const;
  virtual const Block& getBlock(uint16_t) const;
  virtual bool setBlock(uint16_t, const Block&);
  void setBlock(uint16_t, uint16_t);
  virtual SubChunkBlockStorage::Type getType() const;
  virtual uint64_t getBlockTypeCapacity() const;
  virtual const ISubChunkBlockStoragePaletted* asPalettedStorage() const;
  virtual ISubChunkBlockStoragePaletted* asPalettedStorage();
  std::bitset<256> findUsedIndices() const;
  virtual gsl::span<unsigned int const ,-1> getBlocks() const;
  virtual gsl::span<unsigned int,-1> getBlocks();
  virtual uint64_t getBitsPerBlock() const;
  virtual gsl::span<Block const * const,-1> getPaletteSnapshot() const;
  virtual std::unique_ptr<SubChunkBlockStorage,std::default_delete<SubChunkBlockStorage> > makePrunedCopy(SubChunkBlockStorage::PruneType) const;
  virtual bool hasModFlag() const;
  virtual void setModFlag(bool);
  virtual void fetchBlocksInCylinder(const BlockPos&, const BlockPos&, uint32_t, uint32_t, const std::function<bool __cdecl(Block const &)>&, std::vector<BlockFetchResult,std::allocator<BlockFetchResult> >&) const;
  virtual void fetchBlocksInBox(const BlockPos&, const BoundingBox&, const std::function<bool __cdecl(Block const &)>&, std::vector<BlockFetchResult,std::allocator<BlockFetchResult> >&) const;
  void _zeroIndicesGreaterEqualThan(uint16_t);
  virtual void _serialize(IDataOutput&, bool) const;
  int16_t _findPaletteID(const Block&) const;
  virtual ~SubChunkBlockStoragePaletted<8,8>();
  virtual void* __vecDelDtor(uint32_t);
  
  static uint64_t packedWordsCount(uint64_t);
  static uint64_t getBlocksPerWord(uint64_t);
  static bool hasPadding();
  static std::bitset<256> _filterPalette(const std::function<bool __cdecl(Block const &)>&, gsl::span<Block const * const,-1>);
};
