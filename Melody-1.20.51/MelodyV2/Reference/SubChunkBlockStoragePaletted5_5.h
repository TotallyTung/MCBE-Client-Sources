
class SubChunkBlockStoragePaletted<5,5> : public ISubChunkBlockStoragePaletted { /* Size=0xbe8 */
  /* 0x0000: fields for ISubChunkBlockStoragePaletted */
  /* 0x0008 */ uint32_t mBlocks[683];
  /* 0x0ab8 */ AppendOnlyAtomicLookupTable<Block const *,32> mPalette;
  /* 0x0be0 */ bool mModDataFlag;
  static const uint64_t CAPACITY;
  static const uint64_t BLOCKS_PER_WORD;
  static const uint64_t BLOCK_MASK;
  
  SubChunkBlockStoragePaletted<5,5>(const BlockVolume&, int16_t, const gsl::span<Block const *,-1>&);
  SubChunkBlockStoragePaletted<5,5>(const Block&);
  SubChunkBlockStoragePaletted<5,5>(const SubChunkBlockStorage&);
  SubChunkBlockStoragePaletted<5,5>(IDataInput&, const BlockPalette&, bool);
  virtual bool isUniform(const Block&) const;
  virtual const Block& getBlock(uint16_t) const;
  virtual bool setBlock(uint16_t, const Block&);
  void setBlock(uint16_t, uint16_t);
  virtual SubChunkBlockStorage::Type getType() const;
  virtual uint64_t getBlockTypeCapacity() const;
  virtual const ISubChunkBlockStoragePaletted* asPalettedStorage() const;
  virtual ISubChunkBlockStoragePaletted* asPalettedStorage();
  std::bitset<32> findUsedIndices() const;
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
  virtual ~SubChunkBlockStoragePaletted<5,5>();
  virtual void* __vecDelDtor(uint32_t);
  
  static uint64_t packedWordsCount(uint64_t);
  static uint64_t getBlocksPerWord(uint64_t);
  static bool hasPadding();
  static std::bitset<32> _filterPalette(const std::function<bool __cdecl(Block const &)>&, gsl::span<Block const * const,-1>);
};
